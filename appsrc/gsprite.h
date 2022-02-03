#pragma once

#include "minikit.h"

def_class(GSprite): public std::enable_shared_from_this<GSprite> {
    
public:
    static GSpriteRef createSprite(float x, float y, float width, float height);
    static GSpriteRef createSprite();
    
    void _create();
    void destory();
    
    void moveTo(float x, float y, float z);
    void moveTo(float x, float y);
    void setX  (float x);
    void setY  (float y);
    void setZ  (float z);
    
    float x();
    float y();
    float z();
    
    void setSize  (float width , float height);
    void setWidth (float width );
    void setHeight(float height);
    
    float width ();
    float height();
    
    float minX();
    float maxX();
    float minY();
    float maxY();
    
    void setBackgroundColor(MColor color);
    
    void setDrawHandler    (const std::function<void (float w, float h)> &handler);
    void setHitBeginHandler(const std::function<void (float x, float y)> &handler);
    void setHitMoveHandler (const std::function<void (float x, float y)> &handler);
    void setHitEndHandler  (const std::function<void (float x, float y)> &handler);
    
    void _Draw    (float w, float h);
    void _HitBegin(float x, float y);
    void _HitMove (float x, float y);
    void _HitEnd  (float x, float y);
    
protected:
    virtual void OnDraw    (float w, float h) {}
    virtual void OnHitBegin(float x, float y) {}
    virtual void OnHitMove (float x, float y) {}
    virtual void OnHitEnd  (float x, float y) {}
    
private:
    float mX = 0;
    float mY = 0;
    float mZ = 0;
    
    float mWidth  = 0;
    float mHeight = 0;
    
    MColor mBackgroundColor = MColor_WhiteColor;
    
    std::function<void (float w, float h)> mDrawHandler;
    std::function<void (float x, float y)> mHitBeginHandler;
    std::function<void (float x, float y)> mHitMoveHandler;
    std::function<void (float x, float y)> mHitEndHandler;
};
