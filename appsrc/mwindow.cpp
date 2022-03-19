#include "mwindow.h"
#include "mgraphics.h"

//key:
//

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

//window:
//

define_reflectable_class_const(MWindow, UpdateEverySeconds)

MWindow::ptr MWindow::sMainWindow;

define_reflectable_class_function(MWindow, setMainWindow, "args:window")
void MWindow::setMainWindow(const MWindow::ptr &window) {
    //NOTE: a window has its own state and lifecycle, only can be assigned once.
    if (!sMainWindow) {
        sMainWindow = window;
    }
}

define_reflectable_class_function(MWindow, mainWindow)
MWindow *MWindow::mainWindow() {
    if (!sMainWindow) {
        sMainWindow = MWindow::create();
    }
    return sMainWindow.get();
}

define_reflectable_class_function(MWindow, load)
void MWindow::load() {
    mLoaded = true;
    onLoad();
}

define_reflectable_class_function(MWindow, show)
void MWindow::show() {
    mShown = true;
    onShow();
}

define_reflectable_class_function(MWindow, hide)
void MWindow::hide() {
    mShown = false;
    onHide();
}

define_reflectable_class_function(MWindow, resizePixel, "args:widthPx,heightPx;")
void MWindow::resizePixel(float widthPx, float heightPx) {
    float width  = m_pt_from_px widthPx ;
    float height = m_pt_from_px heightPx;

    mSize->setWidth (width );
    mSize->setHeight(height);

    //NOTE: "resiePixel" may be called before "load".
    //but need to ensure that "onLoad" is the first window event.
    if (mLoaded) {
        onResize(width, height);
    }
}

define_reflectable_class_function(MWindow, draw)
void MWindow::draw() {
    onDraw();
}

define_reflectable_class_function(MWindow, touchBeginPixel, "args:xPx,yPx;")
void MWindow::touchBeginPixel(float xPx, float yPx) {
    float x = m_pt_from_px xPx;
    float y = m_pt_from_px yPx;
    onTouchBegin(x, y);
}

define_reflectable_class_function(MWindow, touchMovePixel, "args:xPx,yPx;")
void MWindow::touchMovePixel(float xPx, float yPx) {
    float x = m_pt_from_px xPx;
    float y = m_pt_from_px yPx;
    onTouchMove(x, y);
}

define_reflectable_class_function(MWindow, touchEndPixel, "args:xPx,yPx;")
void MWindow::touchEndPixel(float xPx, float yPx) {
    float x = m_pt_from_px xPx;
    float y = m_pt_from_px yPx;
    onTouchEnd(x, y);
}

define_reflectable_class_function(MWindow, mouseMovePixel, "args:xPx,yPx;")
void MWindow::mouseMovePixel(float xPx, float yPx) {
    float x = m_pt_from_px xPx;
    float y = m_pt_from_px yPx;
    onMouseMove(x, y);
}

define_reflectable_class_function(MWindow, mouseWheel, "args:delta")
void MWindow::mouseWheel(float delta) {
    onMouseWheel(delta);
}

define_reflectable_class_function(MWindow, keyDown, "args:key")
void MWindow::keyDown(MKey key) {
    onKeyDown(key);
}

define_reflectable_class_function(MWindow, write, "args:text,enter;")
void MWindow::write(const std::string &text, bool enter) {
    onWrite(text, enter);
}

define_reflectable_class_function(MWindow, checkWritingUpdated)
bool MWindow::checkWritingUpdated() {
    if (mWritingUpdated) {
        mWritingUpdated = false;
        return true;
    }
    return false;
}

define_reflectable_class_function(MWindow, checkWritingEnabled)
bool MWindow::checkWritingEnabled() {
    return mWritingEnabled;
}

define_reflectable_class_function(MWindow, checkWritingRawText)
std::string MWindow::checkWritingRawText() {
    return mWritingRawText;
}

define_reflectable_class_function(MWindow, loaded)
bool MWindow::loaded() {
    return mLoaded;
}

define_reflectable_class_function(MWindow, shown)
bool MWindow::shown() {
    return mShown;
}

define_reflectable_class_function(MWindow, size)
MSize::ptr MWindow::size() {
    return mSize->copy();
}

define_reflectable_class_function(MWindow, setWritingEnabled, "args:enabled,rawText;")
void MWindow::setWritingEnabled(bool enabled, const std::string &rawText) {
    if (mWritingEnabled && enabled && rawText != mWritingRawText) {
        //change writing text.
        mWritingRawText = rawText;
        mWritingUpdated = true;

    } else if (!mWritingEnabled && enabled) {
        //enable writing.
        mWritingEnabled = true;
        mWritingRawText = rawText;
        mWritingUpdated = true;

    } else if (mWritingEnabled && !enabled) {
        //disable writing.
        mWritingEnabled = false;
        mWritingUpdated = true;
    }
}

define_reflectable_class_function(MWindow, writingEnabled)
bool MWindow::writingEnabled() {
    return mWritingEnabled;
}

define_reflectable_class_function(MWindow, onLoad)
void MWindow::onLoad() {
    implement_injectable_function((void))
}

define_reflectable_class_function(MWindow, onShow)
void MWindow::onShow() {
    implement_injectable_function((void))
}

define_reflectable_class_function(MWindow, onHide)
void MWindow::onHide() {
    implement_injectable_function((void))
}

define_reflectable_class_function(MWindow, onResize, "args:width,height;")
void MWindow::onResize(float width, float height) {
    implement_injectable_function((void), width, height)
}

define_reflectable_class_function(MWindow, onDraw)
void MWindow::onDraw() {
    implement_injectable_function((void))
}

define_reflectable_class_function(MWindow, onTouchBegin, "args:x,y;")
void MWindow::onTouchBegin(float x, float y) {
    implement_injectable_function((void), x, y)
}

define_reflectable_class_function(MWindow, onTouchMove, "args:x,y;")
void MWindow::onTouchMove(float x, float y) {
    implement_injectable_function((void), x, y)
}

define_reflectable_class_function(MWindow, onTouchEnd, "args:x,y;")
void MWindow::onTouchEnd(float x, float y) {
    implement_injectable_function((void), x, y)
}

define_reflectable_class_function(MWindow, onMouseMove, "args:x,y;")
void MWindow::onMouseMove(float x, float y) {
    implement_injectable_function((void), x, y)
}

define_reflectable_class_function(MWindow, onMouseWheel, "args:delta;")
void MWindow::onMouseWheel(float delta) {
    implement_injectable_function((void), delta)
}

define_reflectable_class_function(MWindow, onKeyDown, "args:key;")
void MWindow::onKeyDown(MKey key) {
    implement_injectable_function((void), (int)key)
}

define_reflectable_class_function(MWindow, onWrite, "args:text,enter;")
void MWindow::onWrite(const std::string &text, bool enter) {
    implement_injectable_function((void), text, enter)
}