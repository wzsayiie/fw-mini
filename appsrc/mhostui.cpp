#include "mhostui.h"

#include <vector>

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

struct WindowBuffer {
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

static WindowBuffer *GetWindowBuffer() {
    static WindowBuffer *buffer = nullptr;
    if (!buffer) {
        buffer = new WindowBuffer;
    }
    return buffer;
}

static Triangle *TriangleAt(int index) {
    WindowBuffer *buffer = GetWindowBuffer();
    return &buffer->triangles[index];
}

static Label *LabelAt(int index) {
    WindowBuffer *buffer = GetWindowBuffer();
    return &buffer->labels[index];
}

static TextBox *GetTextBox() {
    WindowBuffer *buffer = GetWindowBuffer();
    return &buffer->textBox;
}

static void SendEvent(WindowBuffer *buffer, MWindowEvent event) {
    buffer->event = event;
    for (const MLambdaRef &listener : buffer->listeners) {
        MLambdaCall(listener.get(), nullptr);
    }
}

void _MWindowOnLoad() {
    WindowBuffer *buffer = GetWindowBuffer();
    
    if (!buffer->loaded) {
        buffer->loaded = true;
        SendEvent(buffer, MWindowEvent_Load);
    }
}

void _MWindowOnShow() {
    WindowBuffer *buffer = GetWindowBuffer();
    
    if (buffer->loaded && !buffer->shown) {
        buffer->shown = true;
        SendEvent(buffer, MWindowEvent_Show);
    }
}

void _MWindowOnHide() {
    WindowBuffer *buffer = GetWindowBuffer();
    
    if (buffer->shown) {
        buffer->shown = false;
        SendEvent(buffer, MWindowEvent_Hide);
    }
}

void _MWindowOnResize(float width, float height) {
    WindowBuffer *buffer = GetWindowBuffer();
    
    if (buffer->width == width && buffer->height == height) {
        return;
    }
    
    buffer->width  = width ;
    buffer->height = height;

    if (buffer->loaded) {
        SendEvent(buffer, MWindowEvent_Resize);
    }
}

void _MWindowOnDraw() {
    WindowBuffer *buffer = GetWindowBuffer();

    buffer->triangles.clear();
    buffer->labels.clear();
    SendEvent(buffer, MWindowEvent_Draw);
}

static void WindowOnTouch(MWindowEvent event, float x, float y) {
    WindowBuffer *buffer = GetWindowBuffer();

    buffer->touchX = x;
    buffer->touchY = y;
    SendEvent(buffer, event);
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
    WindowBuffer *buffer = GetWindowBuffer();

    buffer->textBox.string = MMakeShared(string);
    buffer->textBox.enter = enter;
    SendEvent(buffer, MWindowEvent_TextBox);
}

void _MWindowOnKeyDown(MKey key) {
    WindowBuffer *buffer = GetWindowBuffer();

    buffer->key = key;
    SendEvent(buffer, MWindowEvent_KeyDown);
}

int _MWindowTriangleCount() {
    WindowBuffer *buffer = GetWindowBuffer();
    return (int)buffer->triangles.size();
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
    WindowBuffer *buffer = GetWindowBuffer();
    return (int)buffer->labels.size();
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
        WindowBuffer *buffer = GetWindowBuffer();
        buffer->listeners.push_back(MMakeShared(listener));
    }
}

MWindowEvent MWindowCurrentEvent() {
    return GetWindowBuffer()->event;
}

float MWindowWidth () { return GetWindowBuffer()->width ; }
float MWindowHeight() { return GetWindowBuffer()->height; }

bool MWindowLoaded() { return GetWindowBuffer()->loaded; }
bool MWindowShown () { return GetWindowBuffer()->shown ; }

float MWindowTouchX() { return GetWindowBuffer()->touchX; }
float MWindowTouchY() { return GetWindowBuffer()->touchY; }

MKey MWindowActiveKey() {
    return GetWindowBuffer()->key;
}

void MWindowSelectString(MString *string) {
    WindowBuffer *buffer = GetWindowBuffer();
    buffer->selectedString = MMakeShared(string);
}

void MWindowSelectColor(MColor color) {
    WindowBuffer *buffer = GetWindowBuffer();
    buffer->selectedColor = color;
}

void MWindowSelectFontSize(float size) {
    WindowBuffer *buffer = GetWindowBuffer();
    buffer->selectedFontSize = size;
}

void MWindowSelectAligns(MAligns aligns) {
    WindowBuffer *buffer = GetWindowBuffer();
    buffer->selectedAligns = aligns;
}

static void SelectPoint(int index, float x, float y) {
    WindowBuffer *buffer = GetWindowBuffer();

    buffer->selectedPointX[index]  = x;
    buffer->selectedPointY[index]  = y;
}

void MWindowSelectPoint0(float x, float y) { SelectPoint(0, x, y); }
void MWindowSelectPoint1(float x, float y) { SelectPoint(1, x, y); }
void MWindowSelectPoint2(float x, float y) { SelectPoint(2, x, y); }

void MWindowDrawTriangle() {
    WindowBuffer *buffer = GetWindowBuffer();

    Triangle triangle;
    
    triangle.color = buffer->selectedColor;

    triangle.x[0] = buffer->selectedPointX[0];
    triangle.y[0] = buffer->selectedPointY[0];
    triangle.x[1] = buffer->selectedPointX[1];
    triangle.y[1] = buffer->selectedPointY[1];
    triangle.x[2] = buffer->selectedPointX[2];
    triangle.y[2] = buffer->selectedPointY[2];

    buffer->triangles.push_back(triangle);
}

void MWindowDrawLabel() {
    WindowBuffer *buffer = GetWindowBuffer();

    float x0 = buffer->selectedPointX[0];
    float y0 = buffer->selectedPointY[0];
    float x1 = buffer->selectedPointX[1];
    float y1 = buffer->selectedPointY[1];

    Label label;

    label.string   = buffer->selectedString  ;
    label.color    = buffer->selectedColor   ;
    label.fontSize = buffer->selectedFontSize;
    label.aligns   = buffer->selectedAligns  ;
    
    label.x      = x0;
    label.y      = y0;
    label.width  = x1 - x0;
    label.height = y1 - y0;

    buffer->labels.push_back(label);
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
