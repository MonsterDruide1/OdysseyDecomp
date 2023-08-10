#pragma once

#include <math/seadMatrix.h>
#include "al/Library/HostIO/HioNode.h"
#include "al/Library/Execute/IUseExecutor.h"

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
class CameraTicket;
class ICameraInput;
class PlacementId;
class CameraPoser;
class AreaObjDirector;
class IUseAudioKeeper;
class CameraRailHolder;
class NameToCameraParamTransferFunc;

class CameraDirector : public HioNode, IUseExecutor {
public:
    virtual ~CameraDirector();

    void init(CameraPoserSceneInfo*, const CameraPoserFactory*);
    void initAreaCameraSwitcherMultiForPrototype(AreaObjDirector*);
    void initAreaCameraSwitcherSingle();
    void initResourceHolder(const CameraResourceHolder* resourceHolder);
    void initSceneFovyDegree(float fov);
    void initSettingCloudSea(float);
    void initSnapShotCameraAudioKeeper(IUseAudioKeeper* audioKeeper);
    void initAndCreatePauseCameraCtrl(float);

    void execute() override;
    void update();
    void endInit();

    CameraPoseUpdater* getPoseUpdater(s32 index);
    CameraTicket* createCameraFromFactory(const char*, const PlacementId*, const char*, s32, const sead::Matrix34f&);
    CameraTicket* createCameraFromFactory(CameraPoser*, const PlacementId*, const char*, s32, const sead::Matrix34f&, bool);
    CameraTicket* createObjectCamera(const PlacementId*, const char*, const char*, s32, const sead::Matrix34f&);
    CameraTicket* createObjectEntranceCamera(const PlacementId*, const char*, const sead::Matrix34f&);
    CameraTicket* createMirrorObjectCamera(const PlacementId*, const char*, s32, const sead::Matrix34f&);

    ICameraInput* getCameraInput();
    void setCameraInput(const ICameraInput* input);
    void setViewCameraInput(const ICameraInput* input, s32);
    void registerCameraRailHolder(CameraRailHolder* railHolder);
    void setCameraParamTransferFuncTable(const NameToCameraParamTransferFunc*, s32);
    f32 getSceneFovyDegree();
    void validateCameraArea2D();
    void invalidateCameraArea2D();
    void stopByDeathPlayer();
    void restartByDeathPlayer();
    void startInvalidStopJudgeByDemo();
    void endInvalidStopJudgeByDemo();
    void startSnapShotMode(bool);
    void endSnapShotMode();

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
