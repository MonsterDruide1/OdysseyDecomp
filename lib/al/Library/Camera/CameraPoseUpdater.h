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
struct CameraViewInfo;
class CameraViewFlag;
class Projection;
struct SceneCameraInfo;

class CameraPoseUpdater : public NerveExecutor {
public:
    CameraPoseUpdater(SceneCameraInfo*, s32 viewIdx);

    void init(const CameraParamTransfer* paramTransfer, const CameraStopJudge* stopJudge,
              const CameraStartParamCtrl* startParamCtrl);
    void update();
    bool trySwitchCamera();
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
    sead::Vector2f _7c;
    sead::Vector2f _84;
    sead::Vector2f _8c;
    sead::Vector2f _94;
    CameraTicket* mTicket;
    Projection* mProjection;
    sead::Vector2f _b0;
    f32 mNearClipDistance;
    f32 mFarClipDistance;
    f32 _c0;
    f32 _c4;
    f32 _c8;
    f32 _c;
    f32 _d0;
    CameraStopJudge* mStopJudge;
    void* _e0;
    void* _e8;
    CameraInterpole* mInterpole;
    void* _f8;
};

}  // namespace al
