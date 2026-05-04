#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class DemoActor : public LiveActor {
public:
    DemoActor(const char* name);

    void init(const ActorInitInfo& info) override;
    void control() override;

    virtual void initDemoActor(const ActorInitInfo&, const ActorInitInfo&, const sead::Matrix34f*,
                               bool);
    virtual void initDemoActorSerial(const ActorInitInfo&, const ActorInitInfo&,
                                     const sead::Matrix34f*);
    virtual void initAfterCreateFromFactory(const ActorInitInfo&, const ActorInitInfo&,
                                            const sead::Matrix34f*, bool);
    virtual void startAction(s32);

    virtual void resetDynamics();

    void initCommon(const ActorInitInfo&, const ActorInitInfo&, const sead::Matrix34f*, bool);
    void startDemo(const sead::Matrix34f&);
    bool isExistAction(s32) const;
    const char* getDemoActionName(s32) const;
    void startActionByName(const char* actionName);
    void hideModelBySwitch();
    void showModelBySwitch();
    void endDemo();
    void exeDelay();
    void exeAction();

private:
    u8 _108[0x60];
    bool _168 = false;
};

static_assert(sizeof(DemoActor) == 0x170);
}  // namespace al
