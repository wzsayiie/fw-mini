#include "mwindow.h"
#include "mgraphics.h"

define_reflectable_class_const(MWindow, UpdateEverySeconds)
define_reflectable_class_const(MWindow, TitleName)

MWindow::ptr MWindow::sMainWindow;

define_reflectable_class_function(MWindow, setMainWindow, "setter;args:window")
void MWindow::setMainWindow(const MWindow::ptr &window) {
    //NOTE: a window has its own state and lifecycle, only can be assigned once.
    if (!sMainWindow) {
        sMainWindow = window;
    }
}

define_reflectable_class_function(MWindow, mainWindow, "getter")
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
    //on desktop platforms, a windows have various states such as "focus", "losing focus", and "minimizing".
    //simplifying them as the two states of "shown" and "hidden" requires attention to fault tolerance.
    if (!mShown) {
        mShown = true;
        onShow();
    }
}

define_reflectable_class_function(MWindow, hide)
void MWindow::hide() {
    if (mShown) {
        mShown = false;
        onHide();
    }
}

define_reflectable_class_function(MWindow, resizePixel, "args:pixelW,pixelH")
void MWindow::resizePixel(float pixelW, float pixelH) {
    float width  = m_pt_from_px pixelW;
    float height = m_pt_from_px pixelH;

    mSize = MSize::from(width, height);

    //NOTE: "resizePixel" may be called before "load".
    //but need to ensure that "onLoad" is the first window event.
    if (mLoaded) {
        onResize(width, height);
    }
}

define_reflectable_class_function(MWindow, draw)
void MWindow::draw() {
    onDraw(mSize->width(), mSize->height());
}

define_reflectable_class_function(MWindow, touch, "args:evt,key")
void MWindow::touch(const MTouch::ptr &evt, const MKbKey::ptr &key) {
    if (evt->step() == MTouchStep::Begin) {
        //touch begin event can be accompanied by keyboard modifiers,
        //by this way applications can be more expressive.
        MResetCurrentKbKey(key);
        MResetCurrentTouch(evt);

        onTouchBegin(evt->x(), evt->y());

        MResetCurrentKbKey(nullptr);
        MResetCurrentTouch(nullptr);
        
    } else if (evt->step() == MTouchStep::Move) {
        MResetCurrentTouch(evt);
        onTouchMove(evt->x(), evt->y());
        MResetCurrentTouch(nullptr);
        
    } else if (evt->step() == MTouchStep::End) {
        MResetCurrentTouch(evt);
        onTouchEnd(evt->x(), evt->y());
        MResetCurrentTouch(nullptr);
    }
}

define_reflectable_class_function(MWindow, mouseMove, "args:evt")
void MWindow::mouseMove(const MMouseMove::ptr &evt) {
    MResetCurrentMouseMove(evt);
    onMouseMove(evt->x(), evt->y());
    MResetCurrentMouseMove(nullptr);
}

define_reflectable_class_function(MWindow, mouseWheel, "args:evt")
void MWindow::mouseWheel(const MMouseWheel::ptr &evt) {
    MResetCurrentMouseWheel(evt);
    onMouseWheel(evt->x(), evt->y(), evt->delta());
    MResetCurrentMouseWheel(nullptr);
}

define_reflectable_class_function(MWindow, kbKey, "args:evt")
void MWindow::kbKey(const MKbKey::ptr &evt) {
    MResetCurrentKbKey(evt);
    onKbKey(evt->code());
    MResetCurrentKbKey(nullptr);
}

define_reflectable_class_function(MWindow, writing, "args:evt")
void MWindow::writing(const MWriting::ptr &evt) {
    MResetCurrentWriting(evt);
    onWriting(evt->text());
    MResetCurrentWriting(nullptr);
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

define_reflectable_class_function(MWindow, loaded, "getter")
bool MWindow::loaded() {
    return mLoaded;
}

define_reflectable_class_function(MWindow, shown, "getter")
bool MWindow::shown() {
    return mShown;
}

define_reflectable_class_function(MWindow, size, "getter")
MSize::ptr MWindow::size() {
    return mSize ? mSize : MSize::zero();
}

define_reflectable_class_function(MWindow, mousePosition, "getter")
MPoint::ptr MWindow::mousePosition() {
    return mMousePosition ? mMousePosition : MPoint::zero();
}

define_reflectable_class_function(MWindow, setWritingEnabled, "args:enabled,rawText")
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

define_reflectable_class_function(MWindow, onLoad, "virtual")
define_reflectable_class_function(MWindow, onShow, "virtual")
define_reflectable_class_function(MWindow, onHide, "virtual")

void MWindow::onLoad() { implement_injectable_function(void) }
void MWindow::onShow() { implement_injectable_function(void) }
void MWindow::onHide() { implement_injectable_function(void) }

define_reflectable_class_function(MWindow, onResize, "virtual;args:width,height")
define_reflectable_class_function(MWindow, onDraw  , "virtual;args:width,height")

void MWindow::onResize(float width, float height) { implement_injectable_function(void, width, height) }
void MWindow::onDraw  (float width, float height) { implement_injectable_function(void, width, height) }

define_reflectable_class_function(MWindow, onTouchBegin, "virtual;args:x,y")
define_reflectable_class_function(MWindow, onTouchMove , "virtual;args:x,y")
define_reflectable_class_function(MWindow, onTouchEnd  , "virtual;args:x,y")
define_reflectable_class_function(MWindow, onMouseMove , "virtual;args:x,y")

void MWindow::onTouchBegin(float x, float y) { implement_injectable_function(void, x, y) }
void MWindow::onTouchMove (float x, float y) { implement_injectable_function(void, x, y) }
void MWindow::onTouchEnd  (float x, float y) { implement_injectable_function(void, x, y) }
void MWindow::onMouseMove (float x, float y) { implement_injectable_function(void, x, y) }

define_reflectable_class_function(MWindow, onMouseWheel, "virtual;args:x,y,delta")
void MWindow::onMouseWheel(float x, float y, float delta) {
    implement_injectable_function(void, delta)
}

define_reflectable_class_function(MWindow, onKbKey, "virtual;args:code")
void MWindow::onKbKey(MKbKeyCode code) {
    implement_injectable_function(void, code)
}

define_reflectable_class_function(MWindow, onWriting, "virtual;args:text")
void MWindow::onWriting(const std::string &text) {
    implement_injectable_function(void, text)
}
