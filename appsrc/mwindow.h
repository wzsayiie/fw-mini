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

    static void setMainWindow(const MWindow::ptr &window);
    static MWindow *mainWindow();

public:
    M_HOST_CALL_FUNCTION void load();
    M_HOST_CALL_FUNCTION void show();
    M_HOST_CALL_FUNCTION void hide();

    M_HOST_CALL_FUNCTION void resizePixel(float pixelW, float pixelH);
    M_HOST_CALL_FUNCTION void draw();

    M_HOST_CALL_FUNCTION void touchBeginPixel(float pixelX, float pixelY);
    M_HOST_CALL_FUNCTION void touchMovePixel (float pixelX, float pixelY);
    M_HOST_CALL_FUNCTION void touchEndPixel  (float pixelX, float pixelY);
    M_HOST_CALL_FUNCTION void mouseMovePixel (float pixelX, float pixelY);
    M_HOST_CALL_FUNCTION void mouseWheel     (float delta);

    M_HOST_CALL_FUNCTION void keyDown(MKey key);

    M_HOST_CALL_FUNCTION void write(const std::string &text, bool enter);

    //text box control.
    M_HOST_CALL_FUNCTION bool        checkWritingUpdated();
    M_HOST_CALL_FUNCTION bool        checkWritingEnabled();
    M_HOST_CALL_FUNCTION std::string checkWritingRawText();

    bool       loaded();
    bool       shown ();
    MSize::ptr size  ();

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
    static MWindow::ptr sMainWindow;

private:
    bool mLoaded = false;
    bool mShown  = false;

    dash::auto_assign_ptr<MSize> mSize;

    bool        mWritingUpdated = false;
    bool        mWritingEnabled = false;
    std::string mWritingRawText ;
};
