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

    virtual void initAfterCreateFromFactory(const ActorInitInfo& info, const ActorInitInfo&,
                                            const sead::Matrix34f*, bool);
    virtual void initDemoActor(const ActorInitInfo& info, const ActorInitInfo&,
                               const sead::Matrix34f*, bool);
    virtual void initDemoActorSerial(const ActorInitInfo& info, const ActorInitInfo&,
                                     const sead::Matrix34f*);
    virtual void startAction(s32 actionId);
    virtual void resetDynamics();

    void initCommon(const ActorInitInfo& info, const ActorInitInfo&, const sead::Matrix34f*, bool);
    void startDemo(const sead::Matrix34f& mtx);
    bool isExistAction(s32 actionId) const;
    const char* getDemoActionName(s32 actionId) const;
    void startActionByName(const char* actionName);
    void hideModelBySwitch();
    void showModelBySwitch();
    void endDemo();
    void exeDelay();
    void exeAction();

private:
    void* _108 = nullptr;
    sead::Vector4f _110[2];
    sead::Quatf _130;
    void* _140 = nullptr;
    s32 _148 = -1;
    s32 _14c = -1;
    u16 _150 = 0;
    bool _152 = true;
    void* _158[2] = {nullptr, nullptr};
    bool _168 = false;
};

static_assert(sizeof(DemoActor) == 0x170);
}  // namespace al
