#pragma once

#include "minikit.h"

def_class(GSprite): public std::enable_shared_from_this<GSprite> {
    
public:
    GSprite(float x, float y, float width, float height);
    GSprite();
    
    void born();
    void die();
    
    void moveTo(float x, float y, float z);
    void moveTo(float x, float y);
    
    void setX(float x);
    void setY(float y);
    void setZ(float z);
    
    float x();
    float y();
    float z();
    
    void setSize(float width, float height);
    
    void setWidth (float width );
    void setHeight(float height);
    
    float width ();
    float height();
    
    float minX();
    float maxX();
    float minY();
    float maxY();
    
    void setBackgroundColor(MColor color);
    
    void setDrawHandler    (std::function<void (float w, float h)> handler);
    void setHitBeginHandler(std::function<void (float x, float y)> handler);
    void setHitMoveHandler (std::function<void (float x, float y)> handler);
    void setHitEndHandler  (std::function<void (float x, float y)> handler);
    
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
    float _x = 0;
    float _y = 0;
    float _z = 0;
    
    float _width  = 0;
    float _height = 0;
    
    MColor _backgroundColor = MColor_WhiteColor;
    
    std::function<void (float w, float h)> _drawHandler;
    std::function<void (float x, float y)> _hitBeginHandler;
    std::function<void (float x, float y)> _hitMoveHandler;
    std::function<void (float x, float y)> _hitEndHandler;
};
