#pragma once

#include "mobject.h"

//app:

minikit_class(MApp, MObject) {
public:
    static constexpr float UpdateEverySeconds = 0.1f;

public:
    static MApp *instance();

public:
    MINIKIT_HOST_CALL void launch();
    MINIKIT_HOST_CALL void update();

    void addLaunchListener(const MFunction<void ()>::ptr &listener);
    void addUpdateListener(const MFunction<void ()>::ptr &listener);
    void removeListener   (const MFunction<void ()>::ptr &listener);

private:
    std::set<MFunction<void ()>::ptr> mLaunchListeners;
    std::set<MFunction<void ()>::ptr> mUpdateListeners;
};

//launch registrar:

#define m_app_launch(fcn)                               \
/**/    ;                                               \
/**/    static _MAppLaunchRegistrar _unused_##fcn(fcn); \
/**/    void fcn()

struct dash_exportable _MAppLaunchRegistrar {
    _MAppLaunchRegistrar(void (*fcn)()) noexcept;
};
