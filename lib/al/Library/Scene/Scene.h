#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>

#include "Library/Audio/IUseAudioKeeper.h"
#include "Library/Camera/IUseCamera.h"
#include "Library/Nerve/NerveExecutor.h"
#include "Library/Scene/IUseSceneObjHolder.h"

namespace al {
class ActorInitInfo;
class AudioDirector;
class AudioKeeper;
class LayoutKit;
class LiveActorKit;
class SceneMsgCtrl;
class SceneObjHolder;
class SceneStopCtrl;
class ScreenCoverCtrl;
struct GraphicsInitArg;
struct DrawSystemInfo;
struct SceneInitInfo;
class StageResourceKeeper;

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
    virtual void drawMain() const;
    virtual void drawSub() const;
    AudioKeeper* getAudioKeeper() const override;
    SceneObjHolder* getSceneObjHolder() const override;
    CameraDirector* getCameraDirector() const override;

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

    StageResourceKeeper* getStageResourceKeeper() const { return mStageResourceKeeper; }

    LiveActorKit* getLiveActorKit() const { return mLiveActorKit; }

    LayoutKit* getLayoutKit() const { return mLayoutKit; }

    SceneStopCtrl* getSceneStopCtrl() const { return mSceneStopCtrl; }

    SceneMsgCtrl* getSceneMsgCtrl() const { return mSceneMsgCtrl; }

    ScreenCoverCtrl* getScreenCoverCtrl() const { return mScreenCoverCtrl; }

    AudioDirector* getAudioDirector() const { return mAudioDirector; }

    DrawSystemInfo* getDrawSystemInfo() const { return mDrawSystemInfo; }

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
}  // namespace al
