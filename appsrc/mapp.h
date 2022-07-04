#pragma once

#include "mobject.h"

//app:

m_class(MApp, MObject) {
public:
    static constexpr float UpdateEverySeconds = 0.1f;

public:
    static MApp *instance();

public:
    M_HOST_CALL void launch();
    M_HOST_CALL void update();

    void addLaunchListener(int priority, const MFunction<void ()>::ptr &listener);
    void addUpdateListener(int priority, const MFunction<void ()>::ptr &listener);

    void removeLaunchListener(const MFunction<void ()>::ptr &listener);
    void removeUpdateListener(const MFunction<void ()>::ptr &listener);

private:
    struct ListenerItem {
        MFunction<void ()>::ptr listener;
        int priority;
    };

private:
    void addListener(
        std::vector<ListenerItem> *items, int priority, const MFunction<void ()>::ptr &listener);
    void removeListener(
        std::vector<ListenerItem> *items, const MFunction<void ()>::ptr &listener);

private:
    std::vector<ListenerItem> mLaunchListeners;
    std::vector<ListenerItem> mUpdateListeners;
};

//launch registrar:

#define m_app_launch(fcn, ...)                                          \
/**/    ;                                                               \
/**/    static _MAppLaunchRegistrar _unused_##fcn(fcn, ##__VA_ARGS__);  \
/**/    void fcn()

struct d_exportable _MAppLaunchRegistrar {
    _MAppLaunchRegistrar(void (*fcn)(), int priority = 0) noexcept;
};
