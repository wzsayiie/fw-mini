#include "mhostui.h"

//------------------------------------------------------------------------------
//graphs:

typedef std::shared_ptr<struct AnyGraph     > AnyGraphRef     ;
typedef std::shared_ptr<struct TriangleGraph> TriangleGraphRef;
typedef std::shared_ptr<struct ImageGraph   > ImageGraphRef   ;
typedef std::shared_ptr<struct LabelGraph   > LabelGraphRef   ;

struct AnyGraph {
    virtual ~AnyGraph() {}
    virtual _MGraph type() = 0;
};

struct TriangleGraph : AnyGraph {
    
    _MGraph type() override { return _MGraph_Triangle; }
    
    MColor color = 0;
    float  x0    = 0;
    float  y0    = 0;
    float  x1    = 0;
    float  y1    = 0;
    float  x2    = 0;
    float  y2    = 0;
};

struct ImageGraph : AnyGraph {
    
    _MGraph type() override { return _MGraph_Image; }
    
    MImageRef image;
    
    float x      = 0;
    float y      = 0;
    float width  = 0;
    float height = 0;
};

struct LabelGraph : AnyGraph {
    
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
//draw select:

typedef std::shared_ptr<struct DrawSelect> DrawSelectRef;

struct DrawSelect {
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

typedef std::shared_ptr<struct TextBox> TextBoxRef;

struct TextBox {
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
    float touchX = 0;
    float touchY = 0;
    MKey  key    = 0;

    std::vector<AnyGraphRef> graphs;
    DrawSelectRef select;
    TextBoxRef textBox;
    
    std::vector<MLambdaRef> listeners;
};

//assist functions:

static HostWindow *GetWindow() {
    static HostWindow *window = nullptr;
    if (!window) {
        window = new HostWindow;
        
        window->select  = DrawSelectRef(new DrawSelect);
        window->textBox = TextBoxRef(new TextBox);
    }
    return window;
}

static TriangleGraph *TriangleGraphAt(int i) { return (TriangleGraph *)GetWindow()->graphs[i].get(); }
static ImageGraph    *ImageGraphAt   (int i) { return (ImageGraph    *)GetWindow()->graphs[i].get(); }
static LabelGraph    *LabelGraphAt   (int i) { return (LabelGraph    *)GetWindow()->graphs[i].get(); }

static DrawSelect *GetSelect () { return GetWindow()->select .get(); }
static TextBox    *GetTextBox() { return GetWindow()->textBox.get(); }

static void SendEvent(HostWindow *window, MWindowEvent event) {
    window->event = event;
    for (const MLambdaRef &listener : window->listeners) {
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

void _MWindowOnTextBox(MString *string, bool enter) {
    HostWindow *window = GetWindow();

    window->textBox->string = m_make_shared string;
    window->textBox->enter = enter;
    SendEvent(window, MWindowEvent_TextBox);
}

void _MWindowOnKeyDown(MKey key) {
    HostWindow *window = GetWindow();

    window->key = key;
    SendEvent(window, MWindowEvent_KeyDown);
}

int _MWindowGraphCount() {
    return (int)GetWindow()->graphs.size();
}

_MGraph _MWindowGraphType(int index) {
    return GetWindow()->graphs[index]->type();
}

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

float MWindowWidth    () { return GetWindow()->width ; }
float MWindowHeight   () { return GetWindow()->height; }
bool  MWindowLoaded   () { return GetWindow()->loaded; }
bool  MWindowShown    () { return GetWindow()->shown ; }
float MWindowTouchX   () { return GetWindow()->touchX; }
float MWindowTouchY   () { return GetWindow()->touchY; }
MKey  MWindowActiveKey() { return GetWindow()->key   ; }

void MWindowSelectString(MString *string) { GetSelect()->string = m_make_shared string; }
void MWindowSelectImage (MImage  *image ) { GetSelect()->image  = m_make_shared image ; }

void MWindowSelectColor   (MColor  color) { GetSelect()->color    = color; }
void MWindowSelectFontSize(float   size ) { GetSelect()->fontSize = size ; }
void MWindowSelectHAlign  (MHAlign align) { GetSelect()->hAlign   = align; }
void MWindowSelectVAlign  (MVAlign align) { GetSelect()->vAlign   = align; }

void MWindowSelectPoint0(float x, float y) { DrawSelect *s = GetSelect(); s->x0 = x; s->y0 = y; }
void MWindowSelectPoint1(float x, float y) { DrawSelect *s = GetSelect(); s->x1 = x; s->y1 = y; }
void MWindowSelectPoint2(float x, float y) { DrawSelect *s = GetSelect(); s->x2 = x; s->y2 = y; }

void MWindowDrawTriangle() {
    HostWindow *window = GetWindow();
    DrawSelect *select = window->select.get();

    TriangleGraphRef triangle(new TriangleGraph); {
        
        triangle->color = select->color;
        triangle->x0    = select->x0;
        triangle->y0    = select->y0;
        triangle->x1    = select->x1;
        triangle->y1    = select->y1;
        triangle->x2    = select->x2;
        triangle->y2    = select->y2;
    }
    window->graphs.push_back(triangle);
}

void MWindowDrawImage() {
    HostWindow *window = GetWindow();
    DrawSelect *select = window->select.get();

    if (!select->image) {
        return;
    }
    
    ImageGraphRef image(new ImageGraph); {
        
        image->image  = select->image;
        image->x      = select->x0;
        image->y      = select->y0;
        image->width  = select->x1 - select->x0;
        image->height = select->y1 - select->y0;
    }
    window->graphs.push_back(image);
    
    select->image.reset();
}

void MWindowDrawLabel() {
    HostWindow *window = GetWindow();
    DrawSelect *select = window->select.get();
    
    if (!select->string) {
        return;
    }
    
    LabelGraphRef label(new LabelGraph); {
        
        label->string   = select->string;
        label->color    = select->color;
        label->fontSize = select->fontSize;
        label->hAlign   = select->hAlign;
        label->vAlign   = select->vAlign;
        label->x        = select->x0;
        label->y        = select->y0;
        label->width    = select->x1 - select->x0;
        label->height   = select->y1 - select->y0;
    }
    window->graphs.push_back(label);
    
    select->string.reset();
}

void MWindowSetTextBoxUpdated  (bool     updated ) { GetTextBox()->updated   = updated; }
void MWindowSetTextBoxEnabled  (bool     enabled ) { GetTextBox()->enabled   = enabled; }
void MWindowSetTextBoxRawString(MString *original) { GetTextBox()->rawString = m_make_shared original; }

MString *MWindowTextBoxString() { return GetTextBox()->string.get(); }
bool     MWindowTextBoxEnter () { return GetTextBox()->enter; }
