#pragma once

#include "al/hio/HioNode.h"
#include "al/iuse/IUseExecutor.h"

namespace al {
class SceneCameraInfo;
class SceneCameraCtrl;
class CameraPoseUpdater;
class CameraPoserFactory;
class CameraPoserSceneInfo;
class CameraTicketHolder;
class SpecialCameraHolder;
class CameraTargetCollideInfoHolder;
class CameraTargetHolder;
class CameraInputHolder;
class CameraAngleVerticalRequester;
class CameraStartParamCtrl;
class CameraStopJudge;
class CameraParamTransfer;
class CameraResourceHolder;
class CameraFlagCtrl;
class CameraInSwitchOnAreaDirector;

class CameraDirector : public HioNode, IUseExecutor {
public:
    virtual ~CameraDirector();
    void startSnapShotMode(bool);
    al::CameraPoseUpdater* getPoseUpdater(int index);
    float getSceneFovyDegree() const;

private:
    int mCountCameraPoseUpdaters;
    SceneCameraInfo* mSceneCameraInfo;
    SceneCameraCtrl* mSceneCameraCtrl;
    CameraPoseUpdater** mPoseUpdaters;
    CameraPoserFactory* mPoserFactory;
    CameraPoserSceneInfo* mPoserSceneInfo;
    CameraTicketHolder* mTicketHolder;
    SpecialCameraHolder* mSpecialCameraHolder;
    CameraTargetCollideInfoHolder* mTargetCollideInfoHolder;
    CameraTargetHolder* mTargetHolder;
    CameraInputHolder* mInputHolder;
    CameraAngleVerticalRequester* mAngleVerticalRequester;
    CameraStartParamCtrl* mStartParamCtrl;
    CameraStopJudge* mStopJudge;
    CameraParamTransfer* mParamTransfer;
    const CameraResourceHolder* mCameraResourceHolder;
    CameraFlagCtrl* mFlagCtrl;
    void* customThing;
    CameraInSwitchOnAreaDirector* mInSwitchOnAreaDirector;
    void* anotherCustomThing;
};
}  // namespace al
