#pragma once

#include "cobject.h"
#include "mgeometry.h"
#include "mgeneric.h"
#include "mgraphics.h"

template<class Type, int N> class c_gradient_sampler {
public:
    void append(double from, double duration, const Type *vec) {
        if (!_anim) {
            _anim = new anim_t;
            _anim->from = from;
            //use "_vec" as the begin value.
            memcpy(_anim->begin.v, _vec.v, sizeof(vector_t));
        }
        
        node_t node;
        node.duration = duration;
        memcpy(node.end.v, vec, sizeof(vector_t));
        
        _anim->nodes.push_back(node);
    }
    
    void reset(const Type *vec) {
        memcpy(_vec.v, vec, sizeof(vector_t));
        delete _anim;
        _anim = nullptr;
    }
    
    const Type *now(double tick) {
        //is not animating.
        if (!_anim) {
            return _vec.v;
        }
        
        double   inc   = tick - _anim->from;
        vector_t begin = _anim->begin;
        
        auto stage = _anim->nodes.begin();
        while (stage != _anim->nodes.end()) {
            if (inc <= stage->duration) {
                break;
            }
            
            inc  -= stage->duration;
            begin = stage->end;
            
            ++stage;
        }
        
        //animation is end.
        if (stage == _anim->nodes.end()) {
            _vec = _anim->nodes.back().end;
            
            delete _anim;
            _anim = nullptr;
            
            return _vec.v;
        }
        
        double scale = inc / stage->duration;
        for (int i = 0; i < N; ++i) {
            _anim->cur.v[i] = begin.v[i] + (stage->end.v[i] - begin.v[i]) * scale;
        }
        return _anim->cur.v;
    }
    
    const Type *end() const {
        if (_anim) {
            return _anim->nodes.back().end.v;
        }
        return _vec.v;
    }
    
private:
    struct vector_t {
        Type v[N] = {0};
    };
    
    struct node_t {
        double duration = 0;
        vector_t end = {0};
    };
    
    struct anim_t {
        double   from = 0;
        vector_t begin;
        vector_t cur;
        
        std::vector<node_t> nodes;
    };
    
private:
    vector_t _vec;
    anim_t *_anim = nullptr;
};

m_class(CLayer, CObject) {
public:
    static bool inAnimatingAction();
    
public:
    CLayer();
    
public:
    void addSublayer(const CLayer::ptr &sublayer);
    void removeFromSuperlayer();
    MVector<CLayer::ptr>::ptr sublayers();
    CLayer::ptr superlayer();
    
    void setDrawer     (const MFunction<void (float w, float h)>::ptr &drawer);
    void setCoverDrawer(const MFunction<void (float w, float h)>::ptr &drawer);
    
    MFunction<void (float w, float h)>::ptr drawer     ();
    MFunction<void (float w, float h)>::ptr coverDrawer();
    
    void setVisible(bool               visible);
    void setFrame  (const MRect::ptr  &frame  );
    void setAlpha  (float              alpha  );
    void setColor  (const MColor::ptr &color  );
    
    bool        visible();
    MRect::ptr  frame  ();
    MColor::ptr color  ();
    float       alpha  ();
    
    void animate(double duration, const MFunction<void ()>::ptr &nextAction);
    void draw();
    
private:
    static std::vector<double> sDurations;
    
private:
    struct DrawBuffer {
        MRect::ptr rect ;
        MRect::ptr clip ;
        float      alpha;
    };
    
    void drawAt(const DrawBuffer &buffer, double tick);
    
private:
    void on_dispose() override;
    
private:
    MVector<CLayer::ptr>::ptr mSublayers ;
    std::weak_ptr<CLayer>     mSuperlayer;
    
    MFunction<void (float w, float h)>::ptr mDrawer     ;
    MFunction<void (float w, float h)>::ptr mCoverDrawer;
    
    c_gradient_sampler<bool , 1> mVisible;
    c_gradient_sampler<float, 4> mFrame  ;
    c_gradient_sampler<float, 4> mColor  ;
    c_gradient_sampler<float, 1> mAlpha  ;
};
