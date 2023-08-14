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
class GraphicsInitArg;
struct DrawSystemInfo;

class Scene : public NerveExecutor,
              public IUseAudioKeeper,
              public IUseCamera,
              public IUseSceneObjHolder {
public:
    Scene(const char*);

    virtual ~Scene();
    virtual void init(const SceneInitInfo& initInfo);
    virtual void appear();
    virtual void kill();
    virtual void movement();
    virtual void control();
    virtual void drawMain();
    virtual void drawSub();
    AudioKeeper* getAudioKeeper() const override;
    SceneObjHolder* getSceneObjHolder() const override;
    CameraDirector* getCameraDirector() const override;

    LayoutKit* getLayoutKit() const;
    SceneStopCtrl* getSceneStopCtrl() const;
    SceneMsgCtrl* getSceneMsgCtrl() const;
    void initializeAsync(const SceneInitInfo&);
    void initDrawSystemInfo(const SceneInitInfo&);
    void initSceneObjHolder(SceneObjHolder*);
    void initAndLoadStageResource(const char*, s32);
    void initLiveActorKit(const SceneInitInfo&, s32, s32, s32);
    void initLiveActorKitWithGraphics(const GraphicsInitArg&, const SceneInitInfo&, s32, s32, s32);
    void initLayoutKit(const SceneInitInfo&);
    void initSceneStopCtrl();
    void initSceneMsgCtrl();
    void initScreenCoverCtrl();
    void endInit(const ActorInitInfo&);

private:
    void initLiveActorKitImpl(const SceneInitInfo&, s32, s32, s32);

    bool mIsAlive;
    sead::FixedSafeString<0x40> mName;
    StageResourceKeeper* mStageResourceKeeper;
    LiveActorKit* mLiveActorKit;
    LayoutKit* mLayoutKit;
    SceneObjHolder* mSceneObjHolder;
    SceneStopCtrl* mSceneStopCtrl;
    SceneMsgCtrl* mSceneMsgCtrl;
    ScreenCoverCtrl* mScreenCoverCtrl;
    AudioDirector* mAudioDirector;
    AudioKeeper* mAudioKeeper;
    DrawSystemInfo* mDrawSystemInfo;
};

class StageScene : public Scene {
public:
    StageScene();

    virtual ~StageScene();
    virtual void init(const SceneInitInfo&);
    virtual void appear();
    virtual void kill();

    virtual void control();
    virtual void drawMain();
};
}  // namespace al
