#include "gsprite.h"
#include <algorithm>
#include "gcamera.h"

static def_singleton(sSprites     , std::set<GSpriteRef>);
static def_singleton(sSpriteCount , int);
static def_singleton(sHitingSprite, GSpriteRef);

static bool SmallerZForward(GSpriteRef a, GSpriteRef b) { return a->z() <  b->z(); }
static bool BiggerZForward (GSpriteRef a, GSpriteRef b) { return a->z() >= b->z(); }

static std::vector<GSpriteRef> GetSprites(bool (*cmp)(GSpriteRef, GSpriteRef)) {
    std::vector<GSpriteRef> copy(sSprites().begin(), sSprites().end());
    std::sort(copy.begin(), copy.end(), cmp);
    return copy;
}

static void GetWindowPos(float worldX, float worldY, float *outWindowX, float *outWindowY) {
    *outWindowX = MWindowWidth () / 2 - GCamera().focusX() + worldX;
    *outWindowY = MWindowHeight() / 2 + GCamera().focusY() - worldY;
}

static void GetWorldPos(float windowX, float windowY, float *outWorldX, float *outWorldY) {
    *outWorldX = GCamera().focusX() - MWindowWidth () / 2 + windowX;
    *outWorldY = GCamera().focusY() + MWindowHeight() / 2 - windowY;
}

static void DrawSprite(GSpriteRef sprite) {
    if (sprite->width() <= 0 || sprite->height() <= 0) {
        return;
    }

    float windowX = 0;
    float windowY = 0;
    //NOTE: the origin of the window coordinate is in the upper left corner.
    GetWindowPos(sprite->minX(), sprite->maxY(), &windowX, &windowY);

    if (windowX + sprite->width () < 0 || MWindowWidth () < windowX ||
        windowY + sprite->height() < 0 || MWindowHeight() < windowY )
    {
        return;
    }
    
    MContextSetOffset(windowX, windowY);
    sprite->_Draw(sprite->width(), sprite->height());
}

static void OnDraw() {
    std::vector<GSpriteRef> sprites = GetSprites(SmallerZForward);
    for (auto &sprite : sprites) {
        DrawSprite(sprite);
    }
}

static void OnHitBegin() {
    float worldX = 0;
    float worldY = 0;
    GetWorldPos(MWindowTouchX(), MWindowTouchY(), &worldX, &worldY);

    std::vector<GSpriteRef> sprites = GetSprites(BiggerZForward);
    for (auto &sprite : sprites) {
        if (sprite->width() <= 0 || sprite->height() <= 0) {
            continue;
        }

        if (worldX < sprite->minX() || sprite->maxX() < worldX ||
            worldY < sprite->minY() || sprite->maxY() < worldY )
        {
            continue;
        }

        sHitingSprite() = sprite;
        sprite->_HitBegin(
            - sprite->minX() + worldX,
              sprite->maxY() - worldY
        );

        break;
    }
}

static void OnHitMove() {
    if (!sHitingSprite()) {
        return;
    }

    float worldX = 0;
    float worldY = 0;
    GetWorldPos(MWindowTouchX(), MWindowTouchY(), &worldX, &worldY);
    
    sHitingSprite()->_HitMove(
        - sHitingSprite()->minX() + worldX,
          sHitingSprite()->maxY() - worldY
    );
}

static void OnHitEnd() {
    if (!sHitingSprite()) {
        return;
    }

    float worldX = 0;
    float worldY = 0;
    GetWorldPos(MWindowTouchX(), MWindowTouchY(), &worldX, &worldY);
    
    sHitingSprite()->_HitEnd(
        - sHitingSprite()->minX() + worldX,
          sHitingSprite()->maxY() - worldY
    );
    
    sHitingSprite().reset();
}

static void OnInitialize() MAPP_LAUNCH(OnInitialize, MAppLaunchPriority_Scene) {
    MLambdaRef lambda = m_cast_lambda []() {
        MWindowEvent event = MWindowCurrentEvent();
        
        switch (event) {
            case MWindowEvent_Draw      : OnDraw    (); break;
            case MWindowEvent_TouchBegin: OnHitBegin(); break;
            case MWindowEvent_TouchMove : OnHitMove (); break;
            case MWindowEvent_TouchEnd  : OnHitEnd  (); break;
        }
    };
    MWindowAddListener(lambda.get());
}

GSprite::GSprite(float x, float y, float width, float height) {
    mX = x;
    mY = y;
    
    //the last sprite is on the top.
    mZ = ++sSpriteCount();
    
    mWidth  = width ;
    mHeight = height;
}

GSprite::GSprite() {
    mZ = ++sSpriteCount();
}

void GSprite::born() {
    sSprites().insert(shared_from_this());
}

void GSprite::die() {
    sSprites().erase(shared_from_this());
}

void GSprite::moveTo(float x, float y, float z) {
    mX = x;
    mY = y;
    mZ = z;
}

void GSprite::moveTo(float x, float y) {
    mX = x;
    mY = y;
}

void GSprite::setX(float x) { mX = x; }
void GSprite::setY(float y) { mY = y; }
void GSprite::setZ(float z) { mZ = z; }

float GSprite::x() { return mX; }
float GSprite::y() { return mY; }
float GSprite::z() { return mZ; }

void GSprite::setSize(float width, float height) {
    mWidth  = width ;
    mHeight = height;
}

void GSprite::setWidth (float width ) { mWidth  = width ; }
void GSprite::setHeight(float height) { mHeight = height; }

float GSprite::width () { return mWidth ; }
float GSprite::height() { return mHeight; }

float GSprite::minX() { return mX - mWidth  / 2; }
float GSprite::maxX() { return mX + mWidth  / 2; }
float GSprite::minY() { return mY - mHeight / 2; }
float GSprite::maxY() { return mY + mHeight / 2; }

void GSprite::setBackgroundColor(MColor color) {
    mBackgroundColor = color;
}

void GSprite::setDrawHandler    (std::function<void (float w, float h)> h) { mDrawHandler     = h; }
void GSprite::setHitBeginHandler(std::function<void (float x, float y)> h) { mHitBeginHandler = h; }
void GSprite::setHitMoveHandler (std::function<void (float x, float y)> h) { mHitMoveHandler  = h; }
void GSprite::setHitEndHandler  (std::function<void (float x, float y)> h) { mHitEndHandler   = h; }

template<typename F> void Call(const F &fn, float a, float b) {
    if (fn) {
        fn(a, b);
    }
}

void GSprite::_Draw(float w, float h) {
    MContextSelectColor(mBackgroundColor);
    MContextDrawRect(0, 0, w, h);
    
    Call(mDrawHandler, w, h);
    OnDraw(w, h);
}

void GSprite::_HitBegin(float x, float y) { Call(mHitBeginHandler, x, y); OnHitBegin(x, y); }
void GSprite::_HitMove (float x, float y) { Call(mHitMoveHandler , x, y); OnHitMove (x, y); }
void GSprite::_HitEnd  (float x, float y) { Call(mHitEndHandler  , x, y); OnHitEnd  (x, y); }
