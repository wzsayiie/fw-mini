#include "mhostui.h"

#include <vector>

struct TriangleItem {
    MColor Color = 0;

    float X[3] = {0};
    float Y[3] = {0};
};

struct LabelItem {
    MStringRef String;

    MColor  Color    = 0;
    float   FontSize = 0;
    MAligns Aligns   = 0;

    float X      = 0;
    float Y      = 0;
    float Width  = 0;
    float Height = 0;
};

struct TextBoxItem {

    bool   Dirty    = false;
    bool   Enabled  = false;
    MColor Color    = 0;
    float  FontSize = 0;

    float X      = 0;
    float Y      = 0;
    float Width  = 0;
    float Height = 0;

    MStringRef String;
    bool Enter = false;
};

const int MaxSelectablePointNumber = 3;

struct WindowBuffer {
    bool  Loaded = false;
    float Width  = 0;
    float Height = 0;
    float TouchX = 0;
    float TouchY = 0;
    MKey  Key    = 0;

    MStringRef SelectedString;
    MColor     SelectedColor      = 0;
    float      SelectedFontSize   = 0;
    MAligns    SelectedAligns     = 0;
    int        SelectedPointCount = 0;
    float      SelectedPointX[MaxSelectablePointNumber] = {0};
    float      SelectedPointY[MaxSelectablePointNumber] = {0};

    std::vector<TriangleItem> Triangles;
    std::vector<LabelItem> Labels;
    TextBoxItem TextBox;

    std::vector<MLambdaRef> Listeners;
};

static WindowBuffer *GetWindowBuffer() {
    static WindowBuffer *buffer = nullptr;
    if (!buffer) {
        buffer = new WindowBuffer;
    }
    return buffer;
}

static void SendEvent(WindowBuffer *buffer, MWindowEvent event) {
    for (const MLambdaRef &listener : buffer->Listeners) {
        MLambdaCall(listener.get(), NULL);
    }
}

void _MWindowOnLoad() {
    WindowBuffer *buffer = GetWindowBuffer();
    buffer->Loaded = true;
    SendEvent(buffer, MWindowEvent_Load);
}

void _MWindowOnShow() {
    WindowBuffer *buffer = GetWindowBuffer();
    SendEvent(buffer, MWindowEvent_Show);
}

void _MWindowOnHide() {
    WindowBuffer *buffer = GetWindowBuffer();
    SendEvent(buffer, MWindowEvent_Hide);
}

void _MWindowOnResize(float width, float height) {
    WindowBuffer *buffer = GetWindowBuffer();
    buffer->Width  = width ;
    buffer->Height = height;

    if (buffer->Loaded) {
        SendEvent(buffer, MWindowEvent_Resize);
    }
}

void _MWindowOnUpdate() {
    WindowBuffer *buffer = GetWindowBuffer();
    SendEvent(buffer, MWindowEvent_Update);
}

void _MWindowOnDraw() {
    WindowBuffer *buffer = GetWindowBuffer();

    buffer->Triangles.clear();
    buffer->Labels.clear();
    SendEvent(buffer, MWindowEvent_Draw);
}

static void WindowOnTouch(MWindowEvent event, float x, float y) {
    WindowBuffer *buffer = GetWindowBuffer();

    buffer->TouchX = x;
    buffer->TouchY = y;
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

    buffer->TextBox.String = MMakeShared(string);
    buffer->TextBox.Enter = enter;
    SendEvent(buffer, MWindowEvent_TextBox);
}

void _MWindowOnKeyDown(MKey key) {
    WindowBuffer *buffer = GetWindowBuffer();

    buffer->Key = key;
    SendEvent(buffer, MWindowEvent_KeyDown);
}

int _MWindowTriangleCount() {
    WindowBuffer *buffer = GetWindowBuffer();
    return (int)buffer->Triangles.size();
}

static TriangleItem *TriangleAt(int index) {
    WindowBuffer *buffer = GetWindowBuffer();
    return &buffer->Triangles[index];
}

float _MWindowTriangleVertexX(int index, int vertexIndex) {
    return TriangleAt(index)->X[vertexIndex];
}

float _MWindowTriangleVertexY(int index, int vertexIndex) {
    return TriangleAt(index)->Y[vertexIndex];
}

MColor _MWindowTriangleColor(int index) {
    return TriangleAt(index)->Color;
}

int _MWindowLabelCount() {
    WindowBuffer *buffer = GetWindowBuffer();
    return (int)buffer->Labels.size();
}

static LabelItem *LabelAt(int index) {
    WindowBuffer *buffer = GetWindowBuffer();
    return &buffer->Labels[index];
}

MString *_MWindowLabelString(int index) {
    return LabelAt(index)->String.get();
}

MColor _MWindowLabelColor(int index) {
    return LabelAt(index)->Color;
}

float _MWindowLabelFontSize(int index) {
    return LabelAt(index)->FontSize;
}

MAligns _MWindowLabelAligns(int index) {
    return LabelAt(index)->Aligns;
}

float _MWindowLabelX(int index) {
    return LabelAt(index)->X;
}

float _MWindowLabelY(int index) {
    return LabelAt(index)->Y;
}

float _MWindowLabelWidth(int index) {
    return LabelAt(index)->Width;
}

float _MWindowLabelHeight(int index) {
    return LabelAt(index)->Height;
}

static TextBoxItem *GetTextBox() {
    WindowBuffer *buffer = GetWindowBuffer();
    return &buffer->TextBox;
}

bool _MWindowTextBoxDirty() {
    return GetTextBox()->Dirty;
}

bool _MWindowTextBoxEnabled() {
    return GetTextBox()->Enabled;
}

MColor _MWindowTextBoxColor() {
    return GetTextBox()->Color;
}

float _MWindowTextBoxFontSize() {
    return GetTextBox()->FontSize;
}

float _MWindowTextBoxX() {
    return GetTextBox()->X;
}

float _MWindowTextBoxY() {
    return GetTextBox()->Y;
}

float _MWindowTextBoxWidth() {
    return GetTextBox()->Width;
}

float _MWindowTextBoxHeight() {
    return GetTextBox()->Height;
}

void _MWindowSetTextBoxClean() {
    TextBoxItem *textBox = GetTextBox();
    textBox->Dirty = false;
}

void MWindowAddListener(MLambda *listener) {
    if (listener) {
        WindowBuffer *buffer = GetWindowBuffer();
        buffer->Listeners.push_back(MMakeShared(listener));
    }
}

float MWindowWidth() {
    return GetWindowBuffer()->Width;
}

float MWindowHeight() {
    return GetWindowBuffer()->Height;
}

float MWindowTouchX() {
    return GetWindowBuffer()->TouchX;
}

float MWindowTouchY() {
    return GetWindowBuffer()->TouchY;
}

MKey MWindowActiveKey() {
    return GetWindowBuffer()->Key;
}

void MWindowSelectString(MString *string) {
    WindowBuffer *buffer = GetWindowBuffer();
    buffer->SelectedString = MMakeShared(string);
}

void MWindowSelectColor(MColor color) {
    WindowBuffer *buffer = GetWindowBuffer();
    buffer->SelectedColor = color;
}

void MWindowSelectFontSize(float size) {
    WindowBuffer *buffer = GetWindowBuffer();
    buffer->SelectedFontSize = size;
}

void MWindowSelectAligns(MAligns aligns) {
    WindowBuffer *buffer = GetWindowBuffer();
    buffer->SelectedAligns = aligns;
}

void MWindowPinPoint(float x, float y) {
    WindowBuffer *buffer = GetWindowBuffer();

    buffer->SelectedPointX[0]  = x;
    buffer->SelectedPointY[0]  = y;
    buffer->SelectedPointCount = 1;
}

void MWindowAddPoint(float x, float y) {
    WindowBuffer *buffer = GetWindowBuffer();

    if ((buffer->SelectedPointCount) >= MaxSelectablePointNumber) {
        return;
    }

    int n = (buffer->SelectedPointCount)++;
    buffer->SelectedPointX[n] = x;
    buffer->SelectedPointY[n] = y;
}

void MWindowDrawTriangle() {
    WindowBuffer *buffer = GetWindowBuffer();

    if (buffer->SelectedPointCount < 3) {
        return;
    }

    TriangleItem triangle;
    triangle.Color = buffer->SelectedColor;
    triangle.X[0]  = buffer->SelectedPointX[0];
    triangle.Y[0]  = buffer->SelectedPointY[0];
    triangle.X[1]  = buffer->SelectedPointX[1];
    triangle.Y[1]  = buffer->SelectedPointY[1];
    triangle.X[2]  = buffer->SelectedPointX[2];
    triangle.Y[2]  = buffer->SelectedPointY[2];

    buffer->Triangles.push_back(triangle);
}

void MWindowDrawLabel() {
    WindowBuffer *buffer = GetWindowBuffer();

    if (buffer->SelectedPointCount < 2) {
        return;
    }

    float x0 = buffer->SelectedPointX[0];
    float y0 = buffer->SelectedPointY[0];
    float x1 = buffer->SelectedPointX[1];
    float y1 = buffer->SelectedPointY[1];

    LabelItem label;
    label.String   = buffer->SelectedString  ;
    label.Color    = buffer->SelectedColor   ;
    label.FontSize = buffer->SelectedFontSize;
    label.Aligns   = buffer->SelectedAligns  ;
    label.X        = x0;
    label.Y        = y0;
    label.Width    = x1 - x0;
    label.Height   = y1 - y0;

    buffer->Labels.push_back(label);
}

void MWindowEnableTextBox(bool enabled) {
    TextBoxItem *textBox = GetTextBox();

    if (textBox->Enabled != enabled) {
        textBox->Enabled = enabled;
        textBox->Dirty = true;
    }
}

void MWindowSetTextBoxColor(MColor color) {
    TextBoxItem *textBox = GetTextBox();

    if (textBox->Color != color) {
        textBox->Color = color;
        textBox->Dirty = true;
    }
}

void MWindowSetTextBoxFrame(float x, float y, float w, float h) {
    TextBoxItem *textBox = GetTextBox();

    if (textBox->X      != x ||
        textBox->Y      != y ||
        textBox->Width  != w ||
        textBox->Height != h)
    {
        textBox->X      = x;
        textBox->Y      = y;
        textBox->Width  = w;
        textBox->Height = h;

        textBox->Dirty = true;
    }
}

MString *MWindowTextBoxString() {
    return GetTextBox()->String.get();
}

bool MWindowTextBoxEnter() {
    return GetTextBox()->Enter;
}
