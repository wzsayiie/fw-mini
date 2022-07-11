#pragma once

#include "mgeometry.h"

//key:

m_enum(MKey) {
    None  = 0,

    Back  = 0x08,
    Tab   = 0x09,
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

m_class(MWindow, MObject) {
public:
    static constexpr float UpdateEverySeconds = 0.1f;
    static constexpr const char *TitleName = "Mini";

public:
    static void setMainWindow(const MWindow::ptr &window);
    static MWindow *mainWindow();

public:
    M_HOST_CALL void load();
    M_HOST_CALL void show();
    M_HOST_CALL void hide();

    M_HOST_CALL void resizePixel(float pixelW, float pixelH);
    M_HOST_CALL void draw();

    M_HOST_CALL void touchBeginPixel(float pixelX, float pixelY);
    M_HOST_CALL void touchMovePixel (float pixelX, float pixelY);
    M_HOST_CALL void touchEndPixel  (float pixelX, float pixelY);
    M_HOST_CALL void mouseMovePixel (float pixelX, float pixelY);
    
    M_HOST_CALL void mouseWheel(float delta);
    M_HOST_CALL void key(MKey key);
    M_HOST_CALL void write(const std::string &text, MKey key);

    //text box control.
    M_HOST_CALL bool        checkWritingUpdated();
    M_HOST_CALL bool        checkWritingEnabled();
    M_HOST_CALL std::string checkWritingRawText();

    bool        loaded();
    bool        shown();
    MSize::ptr  size();
    MPoint::ptr mousePosition();

    void setWritingEnabled(bool enabled, const std::string &rawText);
    bool writingEnabled();

protected: public:
    virtual void onLoad();
    virtual void onShow();
    virtual void onHide();

    virtual void onResize(float width, float height);
    virtual void onDraw  (float width, float height);

    virtual void onTouchBegin(float x, float y);
    virtual void onTouchMove (float x, float y);
    virtual void onTouchEnd  (float x, float y);
    virtual void onMouseMove (float x, float y);
    
    virtual void onMouseWheel(float delta);
    virtual void onKey(MKey key);
    virtual void onWrite(const std::string &text, MKey key);

private:
    static MWindow::ptr sMainWindow;

private:
    bool        mLoaded         = false;
    bool        mShown          = false;
    MSize::ptr  mSize           ;
    MPoint::ptr mMousePosition  ;
    bool        mWritingUpdated = false;
    bool        mWritingEnabled = false;
    std::string mWritingRawText ;
};
