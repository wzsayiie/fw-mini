#include "mhostui.h"

//------------------------------------------------------------------------------
//graphs:

def_struct(Graph) {
    virtual ~Graph() {}
    virtual _MGraph type() = 0;
};

def_struct(ClipGraph) : Graph {
    
    _MGraph type() override { return _MGraph_Clip; }
    
    float x      = 0;
    float y      = 0;
    float width  = 0;
    float height = 0;
};

def_struct(TriangleGraph) : Graph {
    
    _MGraph type() override { return _MGraph_Triangle; }
    
    MColor color = 0;
    float  x0    = 0;
    float  y0    = 0;
    float  x1    = 0;
    float  y1    = 0;
    float  x2    = 0;
    float  y2    = 0;
};

def_struct(ImageGraph) : Graph {
    
    _MGraph type() override { return _MGraph_Image; }
    
    MImageRef image;
    
    float x      = 0;
    float y      = 0;
    float width  = 0;
    float height = 0;
};

def_struct(LabelGraph) : Graph {
    
    _MGraph type() override { return _MGraph_Label; }
    
    MStringRef string;
    
    MColor  color    = 0;
    float   fontSize = 0;
    MHAlign hAlign   = 0;
    MVAlign vAlign   = 0;
    float   x        = 0;
    float   y        = 0;
    float   width    = 0;
    float   height   = 0;
};

//------------------------------------------------------------------------------
//draw pen:

def_struct(DrawPen) {
    MStringRef string;
    MImageRef  image ;
    
    MColor  color    = 0;
    float   fontSize = 0;
    MHAlign hAlign   = 0;
    MVAlign vAlign   = 0;
    float   x0       = 0;
    float   y0       = 0;
    float   x1       = 0;
    float   y1       = 0;
    float   x2       = 0;
    float   y2       = 0;
};

//------------------------------------------------------------------------------
//text box:

def_struct(TextBox) {
    MStringRef rawString;
    MStringRef string   ;

    bool updated = false;
    bool enabled = false;
    bool enter   = false;
};

//------------------------------------------------------------------------------
//host window:

struct HostWindow {
    MWindowEvent event = 0;
    
    float width  = 0;
    float height = 0;

    bool  loaded = false;
    bool  shown  = false;

    float touchX     = 0;
    float touchY     = 0;
    float mouseX     = 0;
    float mouseY     = 0;
    MKey  activeKey  = 0;
    float wheelDelta = 0;

    std::vector<GraphRef> graphs;
    DrawPenRef drawPen;
    TextBoxRef textBox;
    
    std::vector<MLambdaRef> listeners;
};

//assist functions:

static HostWindow *GetWindow() {
    static HostWindow *window = nullptr;
    if (!window) {
        window = new HostWindow;
        
        window->drawPen = DrawPenRef(new DrawPen);
        window->textBox = TextBoxRef(new TextBox);
    }
    return window;
}

static ClipGraph     *ClipGraphAt    (int i) { return (ClipGraph     *)GetWindow()->graphs[i].get(); }
static TriangleGraph *TriangleGraphAt(int i) { return (TriangleGraph *)GetWindow()->graphs[i].get(); }
static ImageGraph    *ImageGraphAt   (int i) { return (ImageGraph    *)GetWindow()->graphs[i].get(); }
static LabelGraph    *LabelGraphAt   (int i) { return (LabelGraph    *)GetWindow()->graphs[i].get(); }

static DrawPen *GetDrawPen() { return GetWindow()->drawPen.get(); }
static TextBox *GetTextBox() { return GetWindow()->textBox.get(); }

static void SendEvent(HostWindow *window, MWindowEvent event) {
    window->event = event;

    //NOTE: create a copy of "listeners" to iterate.
    //because it's possible to modify "listeners" during the traversal.
    std::vector<MLambdaRef> copyListeners = window->listeners;

    for (const MLambdaRef &listener : copyListeners) {
        MLambdaCall(listener.get());
    }
}

//interface functions:

static float sPixelDensity = 1.f;

static float   PointFromPixel(_MPixel pixel) { return pixel / sPixelDensity; }
static _MPixel PixelFromPoint(float   point) { return point * sPixelDensity; }

void _MWindowSetPixelDensity(float density) {
    sPixelDensity = density;
}

void _MWindowOnLoad() {
    HostWindow *window = GetWindow();
    
    if (!window->loaded) {
        window->loaded = true;
        SendEvent(window, MWindowEvent_Load);
    }
}

void _MWindowOnShow() {
    HostWindow *window = GetWindow();
    
    if (window->loaded && !window->shown) {
        window->shown = true;
        SendEvent(window, MWindowEvent_Show);
    }
}

void _MWindowOnHide() {
    HostWindow *window = GetWindow();
    
    if (window->shown) {
        window->shown = false;
        SendEvent(window, MWindowEvent_Hide);
    }
}

void _MWindowOnResize(_MPixel width, _MPixel height) {
    float wPoint = PointFromPixel(width );
    float hPoint = PointFromPixel(height);
    
    HostWindow *window = GetWindow();
    if (window->width == wPoint && window->height == hPoint) {
        return;
    }
    
    window->width  = wPoint;
    window->height = hPoint;
    if (window->loaded) {
        SendEvent(window, MWindowEvent_Resize);
    }
}

void _MWindowOnDraw() {
    HostWindow *window = GetWindow();
    
    window->graphs.clear();
    SendEvent(window, MWindowEvent_Draw);
}

static void WindowOnTouch(MWindowEvent event, _MPixel x, _MPixel y) {
    float xPoint = PointFromPixel(x);
    float yPoint = PointFromPixel(y);
    
    HostWindow *window = GetWindow();

    window->touchX = xPoint;
    window->touchY = yPoint;
    SendEvent(window, event);
}

void _MWindowOnTouchBegin(_MPixel x, _MPixel y) { WindowOnTouch(MWindowEvent_TouchBegin, x, y); }
void _MWindowOnTouchMove (_MPixel x, _MPixel y) { WindowOnTouch(MWindowEvent_TouchMove , x, y); }
void _MWindowOnTouchEnd  (_MPixel x, _MPixel y) { WindowOnTouch(MWindowEvent_TouchEnd  , x, y); }

void _MWindowOnMouseMove(_MPixel x, _MPixel y) {
    float xPoint = PointFromPixel(x);
    float yPoint = PointFromPixel(y);

    HostWindow *window = GetWindow();

    window->mouseX = xPoint;
    window->mouseY = yPoint;
    SendEvent(window, MWindowEvent_MouseMove);
}

void _MWindowOnMouseWheel(float delta) {
    HostWindow *window = GetWindow();
    
    window->wheelDelta = delta;
    SendEvent(window, MWindowEvent_MouseWheel);
}

void _MWindowOnTextBox(MString *string, bool enter) {
    HostWindow *window = GetWindow();

    window->textBox->string = m_make_shared string;
    window->textBox->enter = enter;
    SendEvent(window, MWindowEvent_TextBox);
}

void _MWindowOnKeyDown(MKey key) {
    HostWindow *window = GetWindow();

    window->activeKey = key;
    SendEvent(window, MWindowEvent_KeyDown);
}

int _MWindowGraphCount() {
    return (int)GetWindow()->graphs.size();
}

_MGraph _MWindowGraphType(int index) {
    return GetWindow()->graphs[index]->type();
}

_MPixel  _MWindowClipGraphX        (int i) { return PixelFromPoint(ClipGraphAt(i)->x     ); }
_MPixel  _MWindowClipGraphY        (int i) { return PixelFromPoint(ClipGraphAt(i)->y     ); }
_MPixel  _MWindowClipGraphWidth    (int i) { return PixelFromPoint(ClipGraphAt(i)->width ); }
_MPixel  _MWindowClipGraphHeight   (int i) { return PixelFromPoint(ClipGraphAt(i)->height); }

_MPixel  _MWindowTriangleGraphX0   (int i) { return PixelFromPoint(TriangleGraphAt(i)->x0); }
_MPixel  _MWindowTriangleGraphY0   (int i) { return PixelFromPoint(TriangleGraphAt(i)->y0); }
_MPixel  _MWindowTriangleGraphX1   (int i) { return PixelFromPoint(TriangleGraphAt(i)->x1); }
_MPixel  _MWindowTriangleGraphY1   (int i) { return PixelFromPoint(TriangleGraphAt(i)->y1); }
_MPixel  _MWindowTriangleGraphX2   (int i) { return PixelFromPoint(TriangleGraphAt(i)->x2); }
_MPixel  _MWindowTriangleGraphY2   (int i) { return PixelFromPoint(TriangleGraphAt(i)->y2); }
MColor   _MWindowTriangleGraphColor(int i) { return TriangleGraphAt(i)->color             ; }

MImage  *_MWindowImageGraphObject  (int i) { return ImageGraphAt(i)->image.get()           ; }
_MPixel  _MWindowImageGraphX       (int i) { return PixelFromPoint(ImageGraphAt(i)->x     ); }
_MPixel  _MWindowImageGraphY       (int i) { return PixelFromPoint(ImageGraphAt(i)->y     ); }
_MPixel  _MWindowImageGraphWidth   (int i) { return PixelFromPoint(ImageGraphAt(i)->width ); }
_MPixel  _MWindowImageGraphHeight  (int i) { return PixelFromPoint(ImageGraphAt(i)->height); }

MString *_MWindowLabelGraphString  (int i) { return LabelGraphAt(i)->string.get()            ; }
MColor   _MWindowLabelGraphColor   (int i) { return LabelGraphAt(i)->color                   ; }
_MPixel  _MWindowLabelGraphFontSize(int i) { return PixelFromPoint(LabelGraphAt(i)->fontSize); }
MHAlign  _MWindowLabelGraphHAlign  (int i) { return LabelGraphAt(i)->hAlign                  ; }
MVAlign  _MWindowLabelGraphVAlign  (int i) { return LabelGraphAt(i)->vAlign                  ; }
_MPixel  _MWindowLabelGraphX       (int i) { return PixelFromPoint(LabelGraphAt(i)->x     )  ; }
_MPixel  _MWindowLabelGraphY       (int i) { return PixelFromPoint(LabelGraphAt(i)->y     )  ; }
_MPixel  _MWindowLabelGraphWidth   (int i) { return PixelFromPoint(LabelGraphAt(i)->width )  ; }
_MPixel  _MWindowLabelGraphHeight  (int i) { return PixelFromPoint(LabelGraphAt(i)->height)  ; }

bool     _MWindowTextBoxUpdated  () { return GetTextBox()->updated; }
bool     _MWindowTextBoxEnabled  () { return GetTextBox()->enabled; }
MString *_MWindowTextBoxRawString() { return GetTextBox()->rawString.get(); }

void MWindowAddListener(MLambda *listener) {
    if (listener) {
        HostWindow *window = GetWindow();
        window->listeners.push_back(m_make_shared listener);
    }
}

MWindowEvent MWindowCurrentEvent() {
    return GetWindow()->event;
}

float MWindowWidth     () { return GetWindow()->width     ; }
float MWindowHeight    () { return GetWindow()->height    ; }
bool  MWindowLoaded    () { return GetWindow()->loaded    ; }
bool  MWindowShown     () { return GetWindow()->shown     ; }
float MWindowTouchX    () { return GetWindow()->touchX    ; }
float MWindowTouchY    () { return GetWindow()->touchY    ; }
float MWindowMouseX    () { return GetWindow()->mouseX    ; }
float MWindowMouseY    () { return GetWindow()->mouseY    ; }
float MWindowWheelDelta() { return GetWindow()->wheelDelta; }
MKey  MWindowActiveKey () { return GetWindow()->activeKey ; }

void MWindowSelectString(MString *string) { GetDrawPen()->string = m_make_shared string; }
void MWindowSelectImage (MImage  *image ) { GetDrawPen()->image  = m_make_shared image ; }

void MWindowSelectColor   (MColor  color) { GetDrawPen()->color    = color; }
void MWindowSelectFontSize(float   size ) { GetDrawPen()->fontSize = size ; }
void MWindowSelectHAlign  (MHAlign align) { GetDrawPen()->hAlign   = align; }
void MWindowSelectVAlign  (MVAlign align) { GetDrawPen()->vAlign   = align; }

void MWindowSelectPoint0(float x, float y) { DrawPen *p = GetDrawPen(); p->x0 = x; p->y0 = y; }
void MWindowSelectPoint1(float x, float y) { DrawPen *p = GetDrawPen(); p->x1 = x; p->y1 = y; }
void MWindowSelectPoint2(float x, float y) { DrawPen *p = GetDrawPen(); p->x2 = x; p->y2 = y; }

void MWindowDrawPushClip() {
}

void MWindowDrawPopClip() {
}

void MWindowDrawAbsClip() {
}

void MWindowDrawTriangle() {
    HostWindow *window = GetWindow();
    DrawPen *pen = window->drawPen.get();

    TriangleGraphRef triangle(new TriangleGraph); {
        
        triangle->color = pen->color;
        triangle->x0    = pen->x0;
        triangle->y0    = pen->y0;
        triangle->x1    = pen->x1;
        triangle->y1    = pen->y1;
        triangle->x2    = pen->x2;
        triangle->y2    = pen->y2;
    }
    window->graphs.push_back(triangle);
}

void MWindowDrawImage() {
    HostWindow *window = GetWindow();
    DrawPen *pen = window->drawPen.get();

    if (!pen->image) {
        return;
    }
    
    ImageGraphRef image(new ImageGraph); {
        
        image->image  = pen->image;
        image->x      = pen->x0;
        image->y      = pen->y0;
        image->width  = pen->x1 - pen->x0;
        image->height = pen->y1 - pen->y0;
    }
    window->graphs.push_back(image);
    
    pen->image.reset();
}

void MWindowDrawLabel() {
    HostWindow *window = GetWindow();
    DrawPen *pen = window->drawPen.get();
    
    if (!pen->string) {
        return;
    }
    
    LabelGraphRef label(new LabelGraph); {
        
        label->string   = pen->string;
        label->color    = pen->color;
        label->fontSize = pen->fontSize;
        label->hAlign   = pen->hAlign;
        label->vAlign   = pen->vAlign;
        label->x        = pen->x0;
        label->y        = pen->y0;
        label->width    = pen->x1 - pen->x0;
        label->height   = pen->y1 - pen->y0;
    }
    window->graphs.push_back(label);
    
    pen->string.reset();
}

void MWindowSetTextBoxUpdated  (bool     updated ) { GetTextBox()->updated   = updated; }
void MWindowSetTextBoxEnabled  (bool     enabled ) { GetTextBox()->enabled   = enabled; }
void MWindowSetTextBoxRawString(MString *original) { GetTextBox()->rawString = m_make_shared original; }

MString *MWindowTextBoxString() { return GetTextBox()->string.get(); }
bool     MWindowTextBoxEnter () { return GetTextBox()->enter; }
