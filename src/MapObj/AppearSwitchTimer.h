#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>

#include "Library/Nerve/NerveExecutor.h"

namespace al {
struct ActorInitInfo;
class AddDemoInfo;
class AreaObj;
class CameraTicket;
class IUseAudioKeeper;
class IUseStageSwitch;
class IUseCamera;
class LiveActor;
}  // namespace al
class Shine;

class AppearSwitchTimer : public al::NerveExecutor {
public:
    AppearSwitchTimer();
    void init(const al::ActorInitInfo& info, const al::IUseAudioKeeper* audioKeeper,
              al::IUseStageSwitch* stageSwitch, al::IUseCamera* camera, al::LiveActor* actor);
    void onSwitch();
    bool isSwitchOn();
    void offSwitch();
    bool isDemoPlaying();
    void exeWaitAppearDemoStart();
    void exeWaitAppearDemoCameraInterpoling();
    void exeAppearDemo();
    void procAppearDitherAnim(s32 offset);
    void exeOnWait();
    s32 getLastFrame();
    void exeOnWaitBlink();
    void exeOffWait();

private:
    al::LiveActor* mHost = nullptr;
    const al::IUseAudioKeeper* mAudioKeeper = nullptr;
    al::IUseStageSwitch* mStageSwitch = nullptr;
    al::IUseCamera* mCamera = nullptr;
    sead::PtrArray<al::LiveActor>* mTargetActors = nullptr;
    Shine* mShine = nullptr;
    al::AreaObj* mTimerValidArea = nullptr;
    al::CameraTicket* mCameraTicket = nullptr;
    s32 mDemoCameraFrame = -1;
    al::AddDemoInfo* mDemoInfo = nullptr;
    s32 mCameraStartInterpFrame = 45;
    s32 mCameraEndInterpFrame = 30;
    s32 mValidFrame = 600;
    bool mIsSkipCameraStartInterp = false;
    bool mIsSkipCameraEndInterp = false;
    bool mIsInvalidHideRouteGuide = false;
};

static_assert(sizeof(AppearSwitchTimer) == 0x70);
