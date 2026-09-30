#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>

namespace al {
struct ActorInitInfo;
class DemoActor;
class DemoCamera;
class LiveActor;
class PlacementInfo;

class DesignActorInfo {
public:
    DesignActorInfo(LiveActor* actor, const ActorInitInfo& info, const ActorInitInfo& demoInfo,
                    const sead::Matrix34f& baseMtx);

private:
    LiveActor* mActor = nullptr;
    sead::Matrix34f mBaseMtx = sead::Matrix34f::ident;
};

static_assert(sizeof(DesignActorInfo) == 0x38);

class DemoActorHolder {
public:
    DemoActorHolder(bool isDemoScene);

    void initPlacementDemoActor(const PlacementInfo& placementInfo, const ActorInitInfo& info,
                                const sead::Matrix34f* baseMtx);
    void initPlacementDesignActor(const PlacementInfo& placementInfo, const ActorInitInfo& info,
                                  const sead::Matrix34f* baseMtx);
    void appear();
    void kill();
    void setEndCameraInterpolateFrame(s32 frame);
    void startAction(s32 actionIndex);
    void startSequence();
    void updateSequence();
    void updateGraphics();
    bool isActionEndCamera(s32 actionIndex) const;
    bool isEndSequence() const;
    s32 getDemoCameraNum() const;
    bool isExistCamera() const;
    DemoActor* tryFindDemoActor(const char* actorName);
    const char* getCurrentDemoActionName() const;
    s32 getCurrentDemoFrame() const;
    s32 getCurrentDemoFrameMax() const;
    DemoActor* getDemoActor(s32 index) const;

    bool isActiveSequence() const { return mIsActiveSequence; }

private:
    sead::Matrix34f mBaseMtx = sead::Matrix34f::ident;
    DemoActor** mDemoActors = nullptr;
    DemoCamera* mDemoCamera = nullptr;
    void* _40 = nullptr;
    s32 mDemoActorNum = 0;
    bool mIsActiveSequence = false;
    bool mIsDemoScene = false;
    u8 _4e[2] = {};
    s32 mCurrentDemoCameraIndex = 0;
    u8 _54[4] = {};
    DesignActorInfo** mDesignActorInfos = nullptr;
    s32 mDesignActorNum = 0;
};

static_assert(sizeof(DemoActorHolder) == 0x68);
}  // namespace al
