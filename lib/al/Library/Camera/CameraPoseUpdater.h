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
struct CameraViewInfo;
class CameraTurnInfo;
struct CameraObjectRequestInfo;
class PauseCameraCtrl;
class Projection;
class SceneCameraInfo;
class SceneCameraViewCtrl;
struct OrthoProjectionInfo;

class CameraPoseUpdater : public NerveExecutor {
public:
    CameraPoseUpdater(SceneCameraInfo* sceneCamInfo, s32 viewIdx);
    ~CameraPoseUpdater() override;

    void init(const CameraParamTransfer* paramTransfer, const CameraStopJudge* stopJudge,
              CameraStartParamCtrl* startParamCtrl);
    void update();
    bool trySwitchCamera();

    void exeActive();
    void exeDeactive();
    void exeStop();
    void exePause();
    void exeSnapShot();
    void endSnapShot();
    void exeSnapShotNoUpdate();

    bool isActiveInterpole() const;
    void startInterpole(s32 step);
    void requestCancelInterpole();
    bool calcCameraPoseWithoutInterpole(sead::LookAtCamera*) const;

    void startSnapShotMode(bool lockCamera);
    void endSnapShotMode();
    bool isSnapShotOrientationRotate90() const;
    bool isSnapShotOrientationRotate270() const;

    bool isCurrentCameraPriority(s32 priority) const;
    bool isCurrentCameraZooming() const;
    bool isCurrentCameraEnableRotateByPad() const;
    bool isInvalidChangeSubjectiveCamera() const;

    bool tryReceiveCameraRequestFromObject(const CameraObjectRequestInfo& info);
    bool tryRequestCameraTurnToDirection(const CameraTurnInfo* info);

    f32 getNearClipDistance() const;

    void setNearClipDistance(f32 distance) { mNearClipDistance = distance; }

    void setFarClipDistance(f32 distance) { mFarClipDistance = distance; }

    void setAspect(f32 aspect) { mAspect = aspect; }

private:
    nn::album::ImageOrientation mSnapShotOrientation = nn::album::ImageOrientation_None;

    SceneCameraInfo* mSceneCamInfo = nullptr;
    SceneCameraViewCtrl* mSceneCamView = nullptr;
    CameraViewInfo* mViewInfo = nullptr;
    CameraViewFlag* mViewFlag = nullptr;

    bool mIsMainView;
    const s32 mViewIdx;
    sead::LookAtCamera mLookAtCamera;

    CameraTicket* mTicket = nullptr;
    Projection* mProjection = nullptr;
    OrthoProjectionInfo* mOrthoProjectionInfo = nullptr;

    f32 mNearClipDistance = 100.0f;
    f32 mFarClipDistance = 100000.0f;
    f32 mAspect = 16.0f / 9.0f;
    f32 mFovyDegree = 30.0f;

    CameraSwitcher* mSwitcher = nullptr;
    CameraStartParamCtrl* mStartParamCtrl = nullptr;
    const CameraStopJudge* mStopJudge = nullptr;
    const CameraParamTransfer* mParamTransfer = nullptr;
    PauseCameraCtrl* mCtrlPausePtr = nullptr;
    CameraInterpole* mInterpole = nullptr;
    CameraShaker* mShaker = nullptr;
};

static_assert(sizeof(CameraPoseUpdater) == 0x100);

}  // namespace al
