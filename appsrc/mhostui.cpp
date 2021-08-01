#include "mhostui.h"

struct Triangle {
    MColor color = 0;

    float x[3] = {0};
    float y[3] = {0};
};

struct Image {
    MImageRef image;

    float x      = 0;
    float y      = 0;
    float width  = 0;
    float height = 0;
};

struct Label {
    MStringRef string;

    MColor  color    = 0;
    float   fontSize = 0;
    MHAlign hAlign   = 0;
    MVAlign vAlign   = 0;

    float x      = 0;
    float y      = 0;
    float width  = 0;
    float height = 0;
};

struct TextBox {
    MStringRef string;

    bool enabled = false;
    bool enter   = false;
};

struct Window {
    bool loaded = false;
    bool shown  = false;
    
    MWindowEvent event = 0;
    
    float width  = 0;
    float height = 0;
    float touchX = 0;
    float touchY = 0;
    MKey  key    = 0;

    MStringRef selectedString;
    MImageRef  selectedImage ;
    MColor     selectedColor      = 0;
    float      selectedFontSize   = 0;
    MHAlign    selectedHAlign     = 0;
    MVAlign    selectedVAlign     = 0;
    float      selectedPointX[3]  = {0};
    float      selectedPointY[3]  = {0};

    std::vector<Triangle> triangles;
    std::vector<Image> images;
    std::vector<Label> labels;
    TextBox textBox;

    std::vector<MLambdaRef> listeners;
};

static Window *GetWindow() {
    static Window *window = new Window;
    return window;
}

static Triangle *TriangleAt(int index) {
    Window *window = GetWindow();
    return &window->triangles[index];
}

static Image *ImageAt(int index) {
    Window *window = GetWindow();
    return &window->images[index];
}

static Label *LabelAt(int index) {
    Window *window = GetWindow();
    return &window->labels[index];
}

static TextBox *GetTextBox() {
    Window *window = GetWindow();
    return &window->textBox;
}

static void SendEvent(Window *window, MWindowEvent event) {
    window->event = event;
    for (const MLambdaRef &listener : window->listeners) {
        MLambdaCall(listener.get());
    }
}

static float sPixelDensity = 1.f;

static float PointFromPixel(float pixel) { return pixel / sPixelDensity; }
static float PixelFromPoint(float point) { return point * sPixelDensity; }

void _MWindowSetPixelDensity(float density) {
    sPixelDensity = density;
}

void _MWindowOnLoad() {
    Window *window = GetWindow();
    
    if (!window->loaded) {
        window->loaded = true;
        SendEvent(window, MWindowEvent_Load);
    }
}

void _MWindowOnShow() {
    Window *window = GetWindow();
    
    if (window->loaded && !window->shown) {
        window->shown = true;
        SendEvent(window, MWindowEvent_Show);
    }
}

void _MWindowOnHide() {
    Window *window = GetWindow();
    
    if (window->shown) {
        window->shown = false;
        SendEvent(window, MWindowEvent_Hide);
    }
}

void _MWindowOnResize(_MPixel width, _MPixel height) {
    float pointW = PointFromPixel(width );
    float pointH = PointFromPixel(height);
    
    Window *window = GetWindow();
    if (window->width == pointW && window->height == pointH) {
        return;
    }
    
    window->width  = pointW;
    window->height = pointH;
    if (window->loaded) {
        SendEvent(window, MWindowEvent_Resize);
    }
}

void _MWindowOnDraw() {
    Window *window = GetWindow();
    
    window->triangles.clear();
    window->images.clear();
    window->labels.clear();
    
    SendEvent(window, MWindowEvent_Draw);
}

static void WindowOnTouch(MWindowEvent event, _MPixel x, _MPixel y) {
    float pointX = PointFromPixel(x);
    float pointY = PointFromPixel(y);
    
    Window *window = GetWindow();

    window->touchX = pointX;
    window->touchY = pointY;
    SendEvent(window, event);
}

void _MWindowOnTouchBegin(_MPixel x, _MPixel y) { WindowOnTouch(MWindowEvent_TouchBegin, x, y); }
void _MWindowOnTouchMove (_MPixel x, _MPixel y) { WindowOnTouch(MWindowEvent_TouchMove , x, y); }
void _MWindowOnTouchEnd  (_MPixel x, _MPixel y) { WindowOnTouch(MWindowEvent_TouchEnd  , x, y); }

void _MWindowOnTextBox(MString *string, bool enter) {
    Window *window = GetWindow();

    window->textBox.string = m_make_shared string;
    window->textBox.enter = enter;
    SendEvent(window, MWindowEvent_TextBox);
}

void _MWindowOnKeyDown(MKey key) {
    Window *window = GetWindow();

    window->key = key;
    SendEvent(window, MWindowEvent_KeyDown);
}

int _MWindowTriangleCount() {
    Window *window = GetWindow();
    return (int)window->triangles.size();
}

_MPixel _MWindowTriangleVertexX(int index, int v) { return PixelFromPoint(TriangleAt(index)->x[v]); }
_MPixel _MWindowTriangleVertexY(int index, int v) { return PixelFromPoint(TriangleAt(index)->y[v]); }

MColor _MWindowTriangleColor(int index) {
    return TriangleAt(index)->color;
}

int _MWindowImageCount() {
    Window *window = GetWindow();
    return (int)window->images.size();
}

MImage *_MWindowImageObject(int index) {
    return ImageAt(index)->image.get();
}

_MPixel _MWindowImageX(int index) { return PixelFromPoint(ImageAt(index)->x); }
_MPixel _MWindowImageY(int index) { return PixelFromPoint(ImageAt(index)->y); }

_MPixel _MWindowImageWidth (int index) { return PixelFromPoint(ImageAt(index)->width ); }
_MPixel _MWindowImageHeight(int index) { return PixelFromPoint(ImageAt(index)->height); }

int _MWindowLabelCount() {
    Window *window = GetWindow();
    return (int)window->labels.size();
}

MString *_MWindowLabelString(int index) {
    return LabelAt(index)->string.get();
}

MColor _MWindowLabelColor(int index) {
    return LabelAt(index)->color;
}

_MPixel _MWindowLabelFontSize(int index) {
    float size = LabelAt(index)->fontSize;
    return PixelFromPoint(size);
}

MHAlign _MWindowLabelHAlign(int index) { return LabelAt(index)->hAlign; }
MVAlign _MWindowLabelVAlign(int index) { return LabelAt(index)->vAlign; }

_MPixel _MWindowLabelX(int index) { return PixelFromPoint(LabelAt(index)->x); }
_MPixel _MWindowLabelY(int index) { return PixelFromPoint(LabelAt(index)->y); }

_MPixel _MWindowLabelWidth (int index) { return PixelFromPoint(LabelAt(index)->width ); }
_MPixel _MWindowLabelHeight(int index) { return PixelFromPoint(LabelAt(index)->height); }

bool _MWindowTextBoxEnabled() {
    return GetTextBox()->enabled;
}

void MWindowAddListener(MLambda *listener) {
    if (listener) {
        Window *window = GetWindow();
        window->listeners.push_back(m_make_shared listener);
    }
}

MWindowEvent MWindowCurrentEvent() {
    return GetWindow()->event;
}

float MWindowWidth () { return GetWindow()->width ; }
float MWindowHeight() { return GetWindow()->height; }

bool MWindowLoaded() { return GetWindow()->loaded; }
bool MWindowShown () { return GetWindow()->shown ; }

float MWindowTouchX() { return GetWindow()->touchX; }
float MWindowTouchY() { return GetWindow()->touchY; }

MKey MWindowActiveKey() {
    return GetWindow()->key;
}

void MWindowSelectString(MString *string) {
    Window *window = GetWindow();
    window->selectedString = m_make_shared string;
}

void MWindowSelectImage(MImage *image) {
    Window *window = GetWindow();
    window->selectedImage = m_make_shared image;
}

void MWindowSelectColor(MColor color) {
    Window *window = GetWindow();
    window->selectedColor = color;
}

void MWindowSelectFontSize(float size) {
    Window *window = GetWindow();
    window->selectedFontSize = size;
}

void MWindowSelectHAlign(MHAlign align) {
    Window *window = GetWindow();
    window->selectedHAlign = align;
}

void MWindowSelectVAlign(MVAlign align) {
    Window *window = GetWindow();
    window->selectedVAlign = align;
}

static void SelectPoint(int index, float x, float y) {
    Window *window = GetWindow();

    window->selectedPointX[index]  = x;
    window->selectedPointY[index]  = y;
}

void MWindowSelectPoint0(float x, float y) { SelectPoint(0, x, y); }
void MWindowSelectPoint1(float x, float y) { SelectPoint(1, x, y); }
void MWindowSelectPoint2(float x, float y) { SelectPoint(2, x, y); }

void MWindowDrawTriangle() {
    Window *window = GetWindow();

    Triangle triangle;
    
    triangle.color = window->selectedColor;

    triangle.x[0] = window->selectedPointX[0];
    triangle.y[0] = window->selectedPointY[0];
    triangle.x[1] = window->selectedPointX[1];
    triangle.y[1] = window->selectedPointY[1];
    triangle.x[2] = window->selectedPointX[2];
    triangle.y[2] = window->selectedPointY[2];

    window->triangles.push_back(triangle);
}

void MWindowDrawImage() {
    Window *window = GetWindow();
    if (!window->selectedImage) {
        return;
    }

    float x0 = window->selectedPointX[0];
    float y0 = window->selectedPointY[0];
    float x1 = window->selectedPointX[1];
    float y1 = window->selectedPointY[1];

    Image image;
    {
        image.image  = window->selectedImage;
        image.x      = x0;
        image.y      = y0;
        image.width  = x1 - x0;
        image.height = y1 - y0;
    }
    
    window->selectedImage.reset();

    window->images.push_back(image);
}

void MWindowDrawLabel() {
    Window *window = GetWindow();
    if (!window->selectedString) {
        return;
    }

    float x0 = window->selectedPointX[0];
    float y0 = window->selectedPointY[0];
    float x1 = window->selectedPointX[1];
    float y1 = window->selectedPointY[1];

    Label label;
    {
        label.string   = window->selectedString  ;
        label.color    = window->selectedColor   ;
        label.fontSize = window->selectedFontSize;
        label.hAlign   = window->selectedHAlign  ;
        label.vAlign   = window->selectedVAlign  ;
    
        label.x      = x0;
        label.y      = y0;
        label.width  = x1 - x0;
        label.height = y1 - y0;
    }
    
    window->selectedString.reset();

    window->labels.push_back(label);
}

void MWindowEnableTextBox(bool enabled) {
    TextBox *textBox = GetTextBox();
    textBox->enabled = enabled;
}

MString *MWindowTextBoxString() {
    return GetTextBox()->string.get();
}

bool MWindowTextBoxEnter() {
    return GetTextBox()->enter;
}
