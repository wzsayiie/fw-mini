#pragma once

#include "mgeometry.h"

//key:
//

declare_reflectable_enum(MKey)
enum class MKey {
    Back  = 0x08,
    Enter = 0x0D,
    Space = 0x20,

    Left  = 0x25,
    Up    = 0x26,
    Right = 0x27,
    Down  = 0x28,

    A     = 'A' ,
    D     = 'D' ,
    S     = 'S' ,
    W     = 'W' ,
};

//window:
//

declare_reflectable_class(MWindow)
class MWindow : public MExtends<MWindow, MObject> {
public:
    static constexpr const float UpdateEverySeconds = 0.1f;

public:
    M_NEED_HOST_CALL void load();
    M_NEED_HOST_CALL void show();
    M_NEED_HOST_CALL void hide();

    M_NEED_HOST_CALL void resizePixel(float widthPx, float heightPx);
    M_NEED_HOST_CALL void draw();

    M_NEED_HOST_CALL void touchBeginPixel(float xPx, float yPx);
    M_NEED_HOST_CALL void touchMovePixel (float xPx, float yPx);
    M_NEED_HOST_CALL void touchEndPixel  (float xPx, float yPx);
    M_NEED_HOST_CALL void mouseMovePixel (float xPx, float yPx);
    M_NEED_HOST_CALL void mouseWheel     (float delta);

    M_NEED_HOST_CALL void keyDown(MKey key);

    M_NEED_HOST_CALL void write(const std::string &text, bool enter);

    //text box control.
    M_NEED_HOST_CALL bool        checkWritingUpdated();
    M_NEED_HOST_CALL bool        checkWritingEnabled();
    M_NEED_HOST_CALL std::string checkWritingRawText();

public:
    bool loaded();
    bool shown ();

    MSize::ptr size();

    void setWritingEnabled(bool enabled, const std::string &rawText);
    bool writingEnabled();

//protected:
    virtual void onLoad();
    virtual void onShow();
    virtual void onHide();

    virtual void onResize(float width, float height);
    virtual void onDraw();

    virtual void onTouchBegin(float x, float y);
    virtual void onTouchMove (float x, float y);
    virtual void onTouchEnd  (float x, float y);
    virtual void onMouseMove (float x, float y);
    virtual void onMouseWheel(float delta);

    virtual void onKeyDown(MKey key);

    virtual void onWrite(const std::string &text, bool enter);

private:
    bool mLoaded = false;
    bool mShown  = false;

    dash::auto_assign_ptr<MSize> mSize;

    bool        mWritingUpdated = false;
    bool        mWritingEnabled = false;
    std::string mWritingRawText ;
};

void MSetMainWindow(const MWindow::ptr &window);
MWindow *MGetMainWindow();
