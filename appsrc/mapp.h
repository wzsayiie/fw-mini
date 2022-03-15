#pragma once

#include "mobject.h"

declare_reflectable_enum(MAppEvent)
enum class MAppEvent {
    Launch  = 1,
    Update  = 2,
    Command = 3,
};

declare_reflectable_class(MApp)
class MApp : public MExtends<MApp, MObject> {
public:
    static constexpr const float UpdateEverySeconds = 0.1f;

public:
    M_NEED_HOST_CALL void launch ();
    M_NEED_HOST_CALL void update ();
    M_NEED_HOST_CALL void command(const std::string &line);

public:
    void addListener   (MAppEvent event, const MFunction<void ()>::ptr &listener);
    void removeListener(MAppEvent event, const MFunction<void ()>::ptr &listener);

    const std::string &currentCommand();

private:
    std::set<MFunction<void ()>::ptr> mLaunchListeners ;
    std::set<MFunction<void ()>::ptr> mUpdateListeners ;
    std::set<MFunction<void ()>::ptr> mCommandListeners;

    std::string mCommand;
};

MApp *MGetApp();

#define mapp_launch(fcn) ; static _MAppLaunchRegistrar _unused_##fcn(fcn); void fcn()
struct _MAppLaunchRegistrar {
    _MAppLaunchRegistrar(void (*fcn)());
};
