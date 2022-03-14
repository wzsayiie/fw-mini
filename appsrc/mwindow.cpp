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

define_reflectable_const(MWindowUpdateEverySeconds)

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

    mSize->setWidth (width );
    mSize->setHeight(height);

    //NOTE: "resiePixel" may be called before "load".
    //but need to ensure that "onLoad" is the first window event.
    if (mLoaded) {
        OnResize(width, height);
    }
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

define_reflectable_class_function(MWindow, OnLoad)
void MWindow::OnLoad() {
    implement_injectable_function((void))
}

define_reflectable_class_function(MWindow, OnShow)
void MWindow::OnShow() {
    implement_injectable_function((void))
}

define_reflectable_class_function(MWindow, OnHide)
void MWindow::OnHide() {
    implement_injectable_function((void))
}

define_reflectable_class_function(MWindow, OnResize, "args:width,height;")
void MWindow::OnResize(float width, float height) {
    implement_injectable_function((void), width, height)
}

define_reflectable_class_function(MWindow, OnDraw)
void MWindow::OnDraw() {
    implement_injectable_function((void))
}

define_reflectable_class_function(MWindow, OnTouchBegin, "args:x,y;")
void MWindow::OnTouchBegin(float x, float y) {
    implement_injectable_function((void), x, y)
}

define_reflectable_class_function(MWindow, OnTouchMove, "args:x,y;")
void MWindow::OnTouchMove(float x, float y) {
    implement_injectable_function((void), x, y)
}

define_reflectable_class_function(MWindow, OnTouchEnd, "args:x,y;")
void MWindow::OnTouchEnd(float x, float y) {
    implement_injectable_function((void), x, y)
}

define_reflectable_class_function(MWindow, OnMouseMove, "args:x,y;")
void MWindow::OnMouseMove(float x, float y) {
    implement_injectable_function((void), x, y)
}

define_reflectable_class_function(MWindow, OnMouseWheel, "args:delta;")
void MWindow::OnMouseWheel(float delta) {
    implement_injectable_function((void), delta)
}

define_reflectable_class_function(MWindow, OnKeyDown, "args:key;")
void MWindow::OnKeyDown(MKey key) {
    implement_injectable_function((void), (int)key)
}

define_reflectable_class_function(MWindow, OnWrite, "args:text,enter;")
void MWindow::OnWrite(const std::string &text, bool enter) {
    implement_injectable_function((void), text, enter)
}

static dash::lazy_var<MWindow::ptr> sMainWindow;

define_reflectable_function(MSetMainWindow, "args:window;")
void MSetMainWindow(const MWindow::ptr &window) {
    if (!window) {
        return;
    }

    if (sMainWindow) {
        //window size.
        MSize::ptr size = sMainWindow->size();
        window->resizePixel(
            px_from_pt size->width (),
            px_from_pt size->height()
        );

        //loaded and shown flags.
        if (sMainWindow->loaded()) { window->load(); }
        if (sMainWindow->shown ()) { window->show(); }

        //text box state.
        if (sMainWindow->writingEnabled()) {
            window->setWritingEnabled(true, "");
        }
    }
    sMainWindow = window;
}

define_reflectable_function(MGetMainWindow)
MWindow *MGetMainWindow() {
    if (!sMainWindow) {
        sMainWindow = MWindow::create();
    }
    return sMainWindow;
}
