#include "mwindow.h"
#include "mgraphics.h"

define_reflectable_enum_const(MKey, Back )
define_reflectable_enum_const(MKey, Enter)
define_reflectable_enum_const(MKey, Space)
define_reflectable_enum_const(MKey, Left )
define_reflectable_enum_const(MKey, Up   )
define_reflectable_enum_const(MKey, Right)
define_reflectable_enum_const(MKey, Down )
define_reflectable_enum_const(MKey, A    )
define_reflectable_enum_const(MKey, D    )
define_reflectable_enum_const(MKey, S    )
define_reflectable_enum_const(MKey, W    )

define_reflectable_const(MWindowUpdateIntervalSeconds)

define_reflectable_class_function(MWindow, load)
void MWindow::load() {
    mLoaded = true;
    OnLoad();
}

define_reflectable_class_function(MWindow, show)
void MWindow::show() {
    mShown = true;
    OnShow();
}

define_reflectable_class_function(MWindow, hide)
void MWindow::hide() {
    mShown = false;
    OnHide();
}

define_reflectable_class_function(MWindow, resizePixel, "args:widthPx,heightPx;")
void MWindow::resizePixel(float widthPx, float heightPx) {
    float width  = pt_from_px widthPx ;
    float height = pt_from_px heightPx;
    OnResize(width, height);
}

define_reflectable_class_function(MWindow, draw)
void MWindow::draw() {
    OnDraw();
}

define_reflectable_class_function(MWindow, touchBeginPixel, "args:xPx,yPx;")
void MWindow::touchBeginPixel(float xPx, float yPx) {
    float x = pt_from_px xPx;
    float y = pt_from_px yPx;
    OnTouchBegin(x, y);
}

define_reflectable_class_function(MWindow, touchMovePixel, "args:xPx,yPx;")
void MWindow::touchMovePixel(float xPx, float yPx) {
    float x = pt_from_px xPx;
    float y = pt_from_px yPx;
    OnTouchMove(x, y);
}

define_reflectable_class_function(MWindow, touchEndPixel, "args:xPx,yPx;")
void MWindow::touchEndPixel(float xPx, float yPx) {
    float x = pt_from_px xPx;
    float y = pt_from_px yPx;
    OnTouchEnd(x, y);
}

define_reflectable_class_function(MWindow, mouseMovePixel, "args:xPx,yPx;")
void MWindow::mouseMovePixel(float xPx, float yPx) {
    float x = pt_from_px xPx;
    float y = pt_from_px yPx;
    OnMouseMove(x, y);
}

define_reflectable_class_function(MWindow, mouseWheel, "args:delta")
void MWindow::mouseWheel(float delta) {
    OnMouseWheel(delta);
}

define_reflectable_class_function(MWindow, keyDown, "args:key")
void MWindow::keyDown(MKey key) {
    OnKeyDown(key);
}

define_reflectable_class_function(MWindow, write, "args:text,enter;")
void MWindow::write(const std::string &text, bool enter) {
    OnWrite(text, enter);
}

define_reflectable_class_function(MWindow, checkWriting)
bool MWindow::checkWriting() {
    if (mUpdateWriting) {
        mUpdateWriting = false;
        return true;
    } else {
        return false;
    }
}

define_reflectable_class_function(MWindow, loaded)
bool MWindow::loaded() {
    return mLoaded;
}

define_reflectable_class_function(MWindow, shown)
bool MWindow::shown() {
    return mShown;
}

define_reflectable_class_function(MWindow, enableWriting, "args:enabled,text;")
void MWindow::enableWriting(bool enabled, const std::string &text) {
    if (mWritingEnabled && enabled && text != mWritingText) {
        //change writing text.
        mWritingText = text;
        mUpdateWriting = true;

    } else if (!mWritingEnabled && enabled) {
        //open writing.
        mWritingEnabled = true;
        mWritingText = text;
        mUpdateWriting = true;

    } else if (mWritingEnabled && !enabled) {
        //close writing.
        mWritingEnabled = false;
        mUpdateWriting = true;
    }
}

define_reflectable_class_function(MWindow, writingText)
const std::string &MWindow::writingText() {
    return mWritingText;
}

define_reflectable_class_function(MWindow, writingEnabled)
bool MWindow::writingEnabled() {
    return mWritingEnabled;
}

void MWindow::OnLoad() {
    implement_injected_function((void))
}

void MWindow::OnShow() {
    implement_injected_function((void))
}

void MWindow::OnHide() {
    implement_injected_function((void))
}

void MWindow::OnResize(float width, float height) {
    implement_injected_function((void), width, height)
}

void MWindow::OnDraw() {
    implement_injected_function((void))
}

void MWindow::OnTouchBegin(float x, float y) {
    implement_injected_function((void), x, y)
}

void MWindow::OnTouchMove(float x, float y) {
    implement_injected_function((void), x, y)
}

void MWindow::OnTouchEnd(float x, float y) {
    implement_injected_function((void), x, y)
}

void MWindow::OnMouseMove(float x, float y) {
    implement_injected_function((void), x, y)
}

void MWindow::OnMouseWheel(float delta) {
    implement_injected_function((void), delta)
}

void MWindow::OnKeyDown(MKey key) {
    implement_injected_function((void), (int)key)
}

void MWindow::OnWrite(const std::string &text, bool enter) {
    implement_injected_function((void), text, enter)
}

static dash::lazy_var<MWindow::ptr> sMainWindow;

void MSetMainWindow(const MWindow::ptr &window) {
    if (!window) {
        return;
    }

    if (*sMainWindow) {
        if ((*sMainWindow)->loaded()) {
            window->load();
        }
        if ((*sMainWindow)->shown()) {
            window->show();
        }
        if ((*sMainWindow)->writingEnabled()) {
            const std::string &text = (*sMainWindow)->writingText();
            window->enableWriting(true, text);
        }
    }
    *sMainWindow = window;
}

MWindow *MGetMainWindow() {
    if (!*sMainWindow) {
        *sMainWindow = MWindow::create();
    }
    return sMainWindow->get();
}
