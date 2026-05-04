#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class DemoActionList;
class DemoDynamicsResetInfoHolder;
class JointSpringControllerHolder;

class DemoActor : public LiveActor {
public:
    DemoActor(const char* name);

    virtual void initDemoActor(const ActorInitInfo& initInfo, const ActorInitInfo& createInfo,
                               const sead::Matrix34f* baseMtx, bool isSerial);
    void initCommon(const ActorInitInfo& initInfo, const ActorInitInfo& createInfo,
                    const sead::Matrix34f* baseMtx, bool isSerial);
    virtual void initDemoActorSerial(const ActorInitInfo& initInfo, const ActorInitInfo& createInfo,
                                     const sead::Matrix34f* baseMtx);

    void init(const ActorInitInfo& info) override;
    virtual void initAfterCreateFromFactory(const ActorInitInfo& initInfo,
                                            const ActorInitInfo& createInfo,
                                            const sead::Matrix34f* baseMtx, bool isSerial);
    void control() override;

    void startDemo(const sead::Matrix34f& baseMtx);
    virtual void startAction(s32 actionIndex);
    bool isExistAction(s32 actionIndex) const;
    const char* getDemoActionName(s32 actionIndex) const;
    void startActionByName(const char* actionName);
    virtual void resetDynamics();
    void hideModelBySwitch();
    void showModelBySwitch();
    void endDemo();
    void exeDelay();
    void exeAction();

private:
    DemoActionList* mDemoActionList = nullptr;
    sead::Matrix34f mPlacementBaseMtx = sead::Matrix34f::ident;
    s32 mActionDelay = 0;
    s32 mActionIndex = 0;
    s32 mShowModelStartFrame = -1;
    s32 mShowModelEndFrame = -1;
    bool mIsSerial = false;
    bool mIsDemoSceneActor = false;
    bool mIsValidDynamics = true;
    JointSpringControllerHolder* mJointSpringControllerHolder = nullptr;
    DemoDynamicsResetInfoHolder* mDemoDynamicsResetInfoHolder = nullptr;
    bool mIsFollowCamera = false;
};

static_assert(sizeof(DemoActor) == 0x170);
}  // namespace al
