#include "mhostui.h"

struct Triangle {
    MColor color = 0;

    float x[3] = {0};
    float y[3] = {0};
};

struct Label {
    MStringRef string;

    MColor  color    = 0;
    float   fontSize = 0;
    MAligns aligns   = 0;

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
    MColor     selectedColor      = 0;
    float      selectedFontSize   = 0;
    MAligns    selectedAligns     = 0;
    float      selectedPointX[3]  = {0};
    float      selectedPointY[3]  = {0};

    std::vector<Triangle> triangles;
    std::vector<Label> labels;
    TextBox textBox;

    std::vector<MLambdaRef> listeners;
};

static Window *GetWindow() {
    static Window *window = nullptr;
    if (!window) {
        window = new Window;
    }
    return window;
}

static Triangle *TriangleAt(int index) {
    Window *window = GetWindow();
    return &window->triangles[index];
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
        MLambdaCall(listener.get(), nullptr);
    }
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

void _MWindowOnResize(float width, float height) {
    Window *window = GetWindow();
    
    if (window->width == width && window->height == height) {
        return;
    }
    
    window->width  = width ;
    window->height = height;

    if (window->loaded) {
        SendEvent(window, MWindowEvent_Resize);
    }
}

void _MWindowOnDraw() {
    Window *window = GetWindow();

    window->triangles.clear();
    window->labels.clear();
    SendEvent(window, MWindowEvent_Draw);
}

static void WindowOnTouch(MWindowEvent event, float x, float y) {
    Window *window = GetWindow();

    window->touchX = x;
    window->touchY = y;
    SendEvent(window, event);
}

void _MWindowOnTouchBegin(float x, float y) {
    WindowOnTouch(MWindowEvent_TouchBegin, x, y);
}

void _MWindowOnTouchMove(float x, float y) {
    WindowOnTouch(MWindowEvent_TouchMove, x, y);
}

void _MWindowOnTouchEnd(float x, float y) {
    WindowOnTouch(MWindowEvent_TouchEnd, x, y);
}

void _MWindowOnTextBox(MString *string, bool enter) {
    Window *window = GetWindow();

    window->textBox.string = MMakeShared(string);
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

float _MWindowTriangleVertexX(int index, int vertexIndex) {
    return TriangleAt(index)->x[vertexIndex];
}

float _MWindowTriangleVertexY(int index, int vertexIndex) {
    return TriangleAt(index)->y[vertexIndex];
}

MColor _MWindowTriangleColor(int index) {
    return TriangleAt(index)->color;
}

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

float _MWindowLabelFontSize(int index) {
    return LabelAt(index)->fontSize;
}

MAligns _MWindowLabelAligns(int index) {
    return LabelAt(index)->aligns;
}

float _MWindowLabelX(int index) {
    return LabelAt(index)->x;
}

float _MWindowLabelY(int index) {
    return LabelAt(index)->y;
}

float _MWindowLabelWidth(int index) {
    return LabelAt(index)->width;
}

float _MWindowLabelHeight(int index) {
    return LabelAt(index)->height;
}

bool _MWindowTextBoxEnabled() {
    return GetTextBox()->enabled;
}

void MWindowAddListener(MLambda *listener) {
    if (listener) {
        Window *window = GetWindow();
        window->listeners.push_back(MMakeShared(listener));
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
    window->selectedString = MMakeShared(string);
}

void MWindowSelectColor(MColor color) {
    Window *window = GetWindow();
    window->selectedColor = color;
}

void MWindowSelectFontSize(float size) {
    Window *window = GetWindow();
    window->selectedFontSize = size;
}

void MWindowSelectAligns(MAligns aligns) {
    Window *window = GetWindow();
    window->selectedAligns = aligns;
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

void MWindowDrawLabel() {
    Window *window = GetWindow();

    float x0 = window->selectedPointX[0];
    float y0 = window->selectedPointY[0];
    float x1 = window->selectedPointX[1];
    float y1 = window->selectedPointY[1];

    Label label;

    label.string   = window->selectedString  ;
    label.color    = window->selectedColor   ;
    label.fontSize = window->selectedFontSize;
    label.aligns   = window->selectedAligns  ;
    
    label.x      = x0;
    label.y      = y0;
    label.width  = x1 - x0;
    label.height = y1 - y0;

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
