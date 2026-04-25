#pragma once

#include <gfx/seadCamera.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>
#include <nn/album/album_types.h>

#include "Library/Nerve/NerveExecutor.h"

namespace al {

class CameraInterpole;
class CameraParamTransfer;
class CameraShaker;
class CameraStartParamCtrl;
class CameraStopJudge;
class CameraSwitcher;
class CameraTicket;
class CameraViewFlag;
class CameraViewInfo;
class PauseCameraCtrl;
class Projection;
struct CameraObjectRequestInfo;
class SceneCameraInfo;
class CameraTurnInfo;

class CameraPoseUpdater : public NerveExecutor {
public:
    CameraPoseUpdater(SceneCameraInfo*, s32 viewIdx);

    void init(const CameraParamTransfer* paramTransfer, const CameraStopJudge* stopJudge,
              const CameraStartParamCtrl* startParamCtrl);
    f32 getNearClipDistance() const;
    void update();
    bool trySwitchCamera();
    bool isActiveInterpole() const;
    void startInterpole(s32);
    void requestCancelInterpole();
    bool calcCameraPoseWithoutInterpole(sead::LookAtCamera*) const;
    void startSnapshotMode(bool) const;
    void endSnapshotMode();
    bool isSnapShotOrientationRotate90() const;
    bool isSnapShotOrientationRotate270() const;
    void exeActive();
    void exeDeactive();
    void exeStop();
    void exePause();
    void exeSnapShot();
    void endSnapShot();
    void exeSnapShotNoUpdate();
    bool isCurrentCameraPriority(s32 priority) const;
    bool isInvalidChangeSubjectiveCamera() const;
    bool isCurrentCameraZooming() const;
    bool isCurrentCameraEnableRotateByPad() const;
    bool tryReceiveCameraRequestFromObject(const CameraObjectRequestInfo& info);
    bool tryRequestCameraTurnToDirection(const CameraTurnInfo& info);

    CameraShaker* getShaker() const { return mShaker; }

    void setNearClipDistance(f32 distance) { mNearClipDistance = distance; }

    void setFarClipDistance(f32 distance) { mFarClipDistance = distance; }

    void setAspect(f32 aspect) { mAspect = aspect; }

private:
    nn::album::ImageOrientation mSnapShotOrientation;
    SceneCameraInfo* mSceneCamInfo;
    void* _20;
    CameraViewInfo* mViewInfo;
    CameraViewFlag* mViewFlag;
    bool mIsMainView;
    const s32 mViewIdx;
    sead::LookAtCamera mLookAtCamera;
    CameraTicket* mTicket;
    Projection* mProjection;
    sead::Vector2f _b0;
    f32 mNearClipDistance;
    f32 mFarClipDistance;
    f32 mAspect;
    f32 mFovyDegree;
    CameraSwitcher* mSwitcher;
    CameraStartParamCtrl* mStartParamCtrl;
    CameraStopJudge* mStopJudge;
    CameraParamTransfer* mParamTransfer;
    PauseCameraCtrl* mCtrlPausePtr;
    CameraInterpole* mInterpole;
    CameraShaker* mShaker;
};

static_assert(sizeof(CameraPoseUpdater) == 0x100);

}  // namespace al
