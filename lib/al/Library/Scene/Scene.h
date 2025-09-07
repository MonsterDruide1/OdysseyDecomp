#pragma once

#include <prim/seadSafeString.h>

#include "Library/Audio/IUseAudioKeeper.h"
#include "Library/Camera/IUseCamera.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/Nerve/NerveExecutor.h"
#include "Library/Scene/IUseSceneObjHolder.h"
#include "Project/Scene/SceneInitInfo.h"

namespace al {
class AudioDirector;
class StageResourceKeeper;
class LiveActorKit;
class LayoutKit;
class SceneMsgCtrl;
class SceneStopCtrl;
class ScreenCoverCtrl;
struct GraphicsInitArg;
struct DrawSystemInfo;

class Scene : public NerveExecutor,
              public IUseAudioKeeper,
              public IUseCamera,
              public IUseSceneObjHolder {
public:
    Scene(const char* name);

    ~Scene() override;

    virtual void init(const SceneInitInfo& initInfo) {}

    virtual void appear();
    virtual void kill();
    virtual void movement();

    virtual void control() {}

    virtual void drawMain() const {}

    virtual void drawSub() const {}

    AudioKeeper* getAudioKeeper() const override { return mAudioKeeper; }

    void setAudioKeeper(AudioKeeper* audioKeeper) { mAudioKeeper = audioKeeper; }

    SceneObjHolder* getSceneObjHolder() const override { return mSceneObjHolder; }

    CameraDirector* getCameraDirector() const override;

    void initializeAsync(const SceneInitInfo& initInfo);
    void initDrawSystemInfo(const SceneInitInfo& initInfo);
    void initSceneObjHolder(SceneObjHolder* sceneObjHolder);
    void initAndLoadStageResource(const char* stageName, s32 scenarioNo);
    void initLiveActorKit(const SceneInitInfo& initInfo, s32 maxActors, s32 maxPlayers,
                          s32 maxCameras);
    void initLiveActorKitImpl(const SceneInitInfo& initInfo, s32 maxActors, s32 maxPlayers,
                              s32 maxCameras);
    void initLiveActorKitWithGraphics(const GraphicsInitArg& graphicsInitArg,
                                      const SceneInitInfo& initInfo, s32 maxActors, s32 maxPlayers,
                                      s32 maxCameras);
    void initLayoutKit(const SceneInitInfo& initInfo);
    void initSceneStopCtrl();
    void initSceneMsgCtrl();
    void initScreenCoverCtrl();
    void endInit(const ActorInitInfo& initInfo);

    StageResourceKeeper* getStageResourceKeeper() const { return mStageResourceKeeper; }

    LiveActorKit* getLiveActorKit() const { return mLiveActorKit; }

    LayoutKit* getLayoutKit() const { return mLayoutKit; }

    SceneStopCtrl* getSceneStopCtrl() const { return mSceneStopCtrl; }

    SceneMsgCtrl* getSceneMsgCtrl() const { return mSceneMsgCtrl; }

    ScreenCoverCtrl* getScreenCoverCtrl() const { return mScreenCoverCtrl; }

    AudioDirector* getAudioDirector() const { return mAudioDirector; }

    void setAudioDirector(AudioDirector* audioDirector) { mAudioDirector = audioDirector; }

    DrawSystemInfo* getDrawSystemInfo() const { return mDrawSystemInfo; }

private:
    bool mIsAlive = false;
    sead::FixedSafeString<0x40> mName;
    StageResourceKeeper* mStageResourceKeeper = nullptr;
    LiveActorKit* mLiveActorKit = nullptr;
    LayoutKit* mLayoutKit = nullptr;
    SceneObjHolder* mSceneObjHolder = nullptr;
    SceneStopCtrl* mSceneStopCtrl = nullptr;
    SceneMsgCtrl* mSceneMsgCtrl = nullptr;
    ScreenCoverCtrl* mScreenCoverCtrl = nullptr;
    AudioDirector* mAudioDirector = nullptr;
    AudioKeeper* mAudioKeeper = nullptr;
    DrawSystemInfo* mDrawSystemInfo = nullptr;
};
}  // namespace al
