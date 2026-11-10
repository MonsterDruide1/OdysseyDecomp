#pragma once

#include <math/seadVector.h>

#include "Library/Camera/CameraSubTargetBase.h"
#include "Library/Camera/IUseCamera.h"

namespace al {
class CameraDirector;
struct CameraPoseInfo;
class CameraTicket;
class LiveActor;
struct ActorInitInfo;
}  // namespace al

class KoopaCameraCtrl : public al::CameraSubTargetBase, public al::IUseCamera {
public:
    KoopaCameraCtrl(const al::LiveActor* actor, const al::ActorInitInfo& info);
    KoopaCameraCtrl(const al::LiveActor* actor, const al::LiveActor* actor2,
                    const al::ActorInitInfo& info, const sead::Vector3f& trans);

    const char* getTargetName() const override;
    void calcTrans(sead::Vector3f* trans) const override;
    void update() override;
    al::CameraDirector* getCameraDirector() const override;

    void startDemoCamera(const char* animName);
    void endDemoCamera();
    bool tryEndDemoCamera();
    void initFireBeamStartCamera(const al::ActorInitInfo& info);
    void tryStartFireBeamStartCamera(const sead::Vector3f& vec1, const sead::Vector3f& vec2);
    void resetAllCameraSubTarget();
    void switchKoopaSubTarget();
    void switchKoopaAndCapSubTarget();
    void startTargetInterpole(s32 targetType, s32 step);
    void updateTargetTrans();
    bool isExistDemoCameraAnim(const char* animName) const;
    bool isPlayingDemoCameraAnim(const char* animName) const;
    void invalidateDemoCameraAngleSwing();
    void validateDemoCameraAngleSwing();

private:
    al::CameraDirector* mCameraDirector = nullptr;
    al::CameraTicket* mCameraTicket = nullptr;
    const al::CameraPoseInfo* mCameraPoseInfo = nullptr;
    al::CameraTicket* mFireBeamStartTicket = nullptr;
    sead::Vector3f mTrans = {0.0f, 0.0f, 0.0f};
    bool mIsUseSubTarget = false;
    sead::Vector3f mTargetTrans = {0.0f, 0.0f, 0.0f};
    s32 mTargetType = 0;
    s32 mInterpoleStep = 0;
    s32 mInterpoleStepCounter = 0;
    sead::Vector3f mPreTargetTrans = {0.0f, 0.0f, 0.0f};
    const al::LiveActor* mActor;
    const al::LiveActor* mActor2 = nullptr;
};

static_assert(sizeof(KoopaCameraCtrl) == 0x88);
