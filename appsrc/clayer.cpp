#include "clayer.h"
#include "mcontext.h"
#include "mscheduler.h"
#include "rdefine.h"

std::vector<double> CLayer::sDurations;

define_reflectable_class_function(CLayer, inAnimatingAction, "getter")
bool CLayer::inAnimatingAction() {
    return !sDurations.empty();
}

CLayer::CLayer() {
    mSublayers = MVector<CLayer::ptr>::create();
    
    //default values:
    bool visible = true;
    mVisible.reset(&visible);
    
    float alpha = 1;
    mAlpha.reset(&alpha);
}

define_reflectable_class_function(CLayer, addSublayer, "args:sublayer")
void CLayer::addSublayer(const CLayer::ptr &sublayer) {
    if (!sublayer) {
        return;
    }
    if (sublayer.get() == this) {
        return;
    }

    CLayer::ptr oldSuper = sublayer->mSuperlayer.lock();
    if (oldSuper.get() == this) {
        return;
    }
    
    //remove from old superview.
    if (oldSuper) {
        auto &brothers = oldSuper->mSublayers;
        brothers->erase(
            std::remove(brothers->begin(), brothers->end(), sublayer),
            brothers->end()
        );
    }
    
    //add to new superview.
    mSublayers->push_back(sublayer);
    sublayer->mSuperlayer = me();
}

define_reflectable_class_function(CLayer, removeFromSuperlayer)
void CLayer::removeFromSuperlayer() {
    CLayer::ptr nowSuper = mSuperlayer.lock();
    if (!nowSuper) {
        return;
    }
    
    auto &brothers = nowSuper->mSublayers;
    brothers->erase(
        std::remove(brothers->begin(), brothers->end(), me()),
        brothers->end()
    );
    
    mSuperlayer.reset();
}

define_reflectable_class_function(CLayer, sublayers, "getter")
MVector<CLayer::ptr>::ptr CLayer::sublayers() {
    return mSublayers;
}

define_reflectable_class_function(CLayer, superlayer, "getter")
CLayer::ptr CLayer::superlayer() {
    return mSuperlayer.lock();
}

define_reflectable_class_function(CLayer, setDrawer, "setter;args:drawer")
void CLayer::setDrawer(const MFunction<void (float w, float h)>::ptr &drawer) {
    mDrawer = drawer;
}

define_reflectable_class_function(CLayer, setCoverDrawer, "setter;args:drawer")
void CLayer::setCoverDrawer(const MFunction<void (float w, float h)>::ptr &drawer) {
    mCoverDrawer = drawer;
}

define_reflectable_class_function(CLayer, drawer, "getter")
MFunction<void (float w, float h)>::ptr CLayer::drawer() {
    return mDrawer;
}

define_reflectable_class_function(CLayer, coverDrawer, "getter")
MFunction<void (float w, float h)>::ptr CLayer::coverDrawer() {
    return mCoverDrawer;
}

define_reflectable_class_function(CLayer, setVisible, "setter;args:visible")
void CLayer::setVisible(bool visible) {
    if (!sDurations.empty()) {
        mVisible.append(MScheduler::instance()->secondsTick(), sDurations.back(), &visible);
    } else {
        mVisible.reset(&visible);
    }
}

define_reflectable_class_function(CLayer, setFrame, "setter;args:frame")
void CLayer::setFrame(const MRect::ptr &frame) {
    float v[4] = {0};
    if (frame) {
        v[0] = frame->x();
        v[1] = frame->y();
        v[2] = frame->width ();
        v[3] = frame->height();
    }
    
    if (!sDurations.empty()) {
        mFrame.append(MScheduler::instance()->secondsTick(), sDurations.back(), v);
    } else {
        mFrame.reset(v);
    }
}

define_reflectable_class_function(CLayer, setAlpha, "setter;args:alpha")
void CLayer::setAlpha(float alpha) {
    if (!sDurations.empty()) {
        mAlpha.append(MScheduler::instance()->secondsTick(), sDurations.back(), &alpha);
    } else {
        mAlpha.reset(&alpha);
    }
}

define_reflectable_class_function(CLayer, setColor, "setter;args:color")
void CLayer::setColor(const MColor::ptr &color) {
    float v[4] = {0};
    if (color) {
        v[0] = color->redCom  ();
        v[1] = color->greenCom();
        v[2] = color->blueCom ();
        v[3] = color->alphaCom();
    }
    
    if (!sDurations.empty()) {
        mColor.append(MScheduler::instance()->secondsTick(), sDurations.back(), v);
    } else {
        mColor.reset(v);
    }
}

define_reflectable_class_function(CLayer, visible, "getter")
bool CLayer::visible() {
    const bool *v = mVisible.end();
    return *v;
}

define_reflectable_class_function(CLayer, frame, "getter")
MRect::ptr CLayer::frame() {
    const float *v = mFrame.end();
    return MRect::from(v[0], v[1], v[2], v[3]);
}

define_reflectable_class_function(CLayer, color, "getter")
MColor::ptr CLayer::color() {
    const float *v = mColor.end();
    return MColor::fromComs(v[0], v[1], v[2], v[3]);
}

define_reflectable_class_function(CLayer, alpha, "getter")
float CLayer::alpha() {
    return *mAlpha.end();
}

define_reflectable_class_function(CLayer, animate, "args:duration,nextAction")
void CLayer::animate(double duration, const MFunction<void ()>::ptr &nextAction) {
    if (duration <= 0) {
        return;
    }
    
    sDurations.push_back(duration);
    
    if (nextAction) {
        nextAction->call();
    }
    
    sDurations.pop_back();
}

define_reflectable_class_function(CLayer, draw)
void CLayer::draw() {
    double tick = MScheduler::instance()->secondsTick();
    
    const float *v = mFrame.now(tick);
    auto rect = MRect::from(v[0], v[1], v[2], v[3]);
    
    DrawBuffer buffer;
    buffer.rect  = rect;
    buffer.clip  = rect;
    buffer.alpha = *mAlpha.now(tick);
    
    drawAt(buffer, tick);
}

void CLayer::drawAt(const DrawBuffer &buffer, double tick) {
    //if a layer is not visible, all its sublayers are also not visible.
    if (!*mVisible.now(tick)) {
        return;
    }
    
    //draw self:
    const MRect::ptr &clip  = buffer.clip ;
    const MRect::ptr &rect  = buffer.rect ;
    float             alpha = buffer.alpha;
    
    MContextSetClip  (clip->x(), clip->y(), clip->width(), clip->height());
    MContextSetOffset(rect->x(), rect->y());
    MContextSetAlpha (alpha);
    
    MColorRGBA color; {
        const float *v = mColor.now(tick);
        
        color.red   = (uint8_t)(v[0] * 255);
        color.green = (uint8_t)(v[1] * 255);
        color.blue  = (uint8_t)(v[2] * 255);
        color.alpha = (uint8_t)(v[3] * 255);
    }
    MContextSelectRGBA(color.rgba);
    MContextDrawRect(0, 0, rect->width(), rect->height());
    
    if (mDrawer) {
        mDrawer->call(rect->width(), rect->height());
    }
    
    //draw sublayers.
    for (const auto &sublayer : *mSublayers) {
        const float *v = sublayer->mFrame.now(tick);
        
        DrawBuffer subBuffer;
        subBuffer.rect  = MRect::from(v[0] + rect->x(), v[1] + rect->y(), v[2], v[3]);
        subBuffer.clip  = rect->intersects(subBuffer.rect);
        subBuffer.alpha = *sublayer->mAlpha.now(tick) * alpha;
        
        sublayer->drawAt(subBuffer, tick);
    }
    
    //draw cover.
    if (mCoverDrawer) {
        MContextSetClip  (clip->x(), clip->y(), clip->width(), clip->height());
        MContextSetOffset(rect->x(), rect->y());
        MContextSetAlpha (alpha);
        
        mCoverDrawer->call(rect->width(), rect->height());
    }
}

void CLayer::on_dispose() {
    super::dispose();
    
    mDrawer = nullptr;
    mCoverDrawer = nullptr;
    for (const auto &it : *mSublayers) {
        it->dispose();
    }
}
