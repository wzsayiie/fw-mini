#pragma once

#include "mobject.h"

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

const float MWindowUpdateIntervalSeconds = 0.1f;

declare_reflectable_class(MWindow)
class MWindow : public MExtends<MWindow, MBaseObject> {
public:
    //these functions need to be called by host:
    void load();
    void show();
    void hide();

    void resizePixel(float widthPx, float heightPx);
    void draw();

    void touchBeginPixel(float xPx, float yPx);
    void touchMovePixel (float xPx, float yPx);
    void touchEndPixel  (float xPx, float yPx);
    void mouseMovePixel (float xPx, float yPx);
    void mouseWheel     (float delta);

    void keyDown(MKey key);

    void write(const std::string &text, bool enter);
    bool checkWriting();

public:
    bool loaded();
    bool shown();

    void enableWriting(bool enabled, const std::string &text);
    const std::string &writingText();
    bool writingEnabled();

    virtual void OnLoad();
    virtual void OnShow();
    virtual void OnHide();

    virtual void OnResize(float width, float height);
    virtual void OnDraw();

    virtual void OnTouchBegin(float x, float y);
    virtual void OnTouchMove (float x, float y);
    virtual void OnTouchEnd  (float x, float y);
    virtual void OnMouseMove (float x, float y);
    virtual void OnMouseWheel(float delta);

    virtual void OnKeyDown(MKey key);

    virtual void OnWrite(const std::string &text, bool enter);

private:
    bool mLoaded = false;
    bool mShown  = false;

    bool mUpdateWriting  = false;
    bool mWritingEnabled = false;
    std::string mWritingText;
};

void MSetMainWindow(const MWindow::ptr &window);
MWindow *MGetMainWindow();
