#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveExecutor.h"

namespace al {

class CameraInterpole;
class CameraParamTransfer;
class CameraStopJudge;
class CameraStartParamCtrl;
class CameraTicket;
class CameraViewInfo;
class CameraViewFlag;
class Projection;
class SceneCameraInfo;

class CameraPoseUpdater : public NerveExecutor {
public:
    CameraPoseUpdater(SceneCameraInfo*, s32 viewIdx);
    virtual ~CameraPoseUpdater();

    void init(const CameraParamTransfer* paramTransfer, const CameraStopJudge* stopJudge,
              const CameraStartParamCtrl* startParamCtrl);
    void update();
    void trySwitchCamera();
    bool isActiveinterpole();
    void startinterpole();

    void setNearClipDistance(f32 distance) { mNearClipDistance = distance; }

    void setFarClipDistance(f32 distance) { mFarClipDistance = distance; }

private:
    s32 mSnapShotOrientation;  // nn::album::ImageOrientation type
    SceneCameraInfo* mSceneCamInfo;
    void* _20;
    CameraViewInfo* mViewInfo;
    CameraViewFlag* mViewFlag;
    bool mIsMainView;
    s32 mViewIdx;
    void* _40;
    sead::Matrix34f mMatrix;
    s32 _78;
    sead::Vector2f _7C;
    sead::Vector2f _84;
    sead::Vector2f _8C;
    sead::Vector2f _94;
    CameraTicket* mTicket;
    Projection* mProjection;
    sead::Vector2f _B0;
    f32 mNearClipDistance;
    f32 mFarClipDistance;
    f32 _C0;
    f32 _C4;
    f32 _C8;
    f32 _CC;
    f32 _D0;
    CameraStopJudge* mStopJudge;
    void* _E0;
    void* _E8;
    CameraInterpole* mInterpole;
    void* _F8;
};

}  // namespace al
