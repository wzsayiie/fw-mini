#pragma once

#include "mevent.h"
#include "mgeometry.h"

m_class(MWindow, MObject) {
public:
    static constexpr float UpdateEverySeconds = 0.1f;

public:
    static void setMainWindow(const MWindow::ptr &window);
    static MWindow *mainWindow();

public:
    M_HOST_CALL void load();
    M_HOST_CALL void show();
    M_HOST_CALL void hide();

    M_HOST_CALL void resizePixel(float pixelW, float pixelH);
    M_HOST_CALL void draw();

    M_HOST_CALL void touch     (const MTouch::ptr      &evt, const MKbKey::ptr &key);
    M_HOST_CALL void mouseMove (const MMouseMove::ptr  &evt);
    M_HOST_CALL void mouseWheel(const MMouseWheel::ptr &evt);
    M_HOST_CALL void kbKey     (const MKbKey::ptr      &evt);
    M_HOST_CALL void writing   (const MWriting::ptr    &evt);

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
    virtual void onMouseWheel(float x, float y, float delta);
    virtual void onKbKey     (MKbKeyCode code);
    virtual void onWriting   (const std::string &text);

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
