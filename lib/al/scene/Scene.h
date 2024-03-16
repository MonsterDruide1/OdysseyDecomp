#pragma once

#include <prim/seadSafeString.h>
#include "al/iuse/IUseAudioKeeper.h"
#include "al/iuse/IUseCamera.h"
#include "al/nerve/NerveExecutor.h"
#include "al/scene/SceneObjHolder.h"

namespace al {
class SceneInitInfo;
class StageResourceKeeper;
class LiveActorKit;
class LayoutKit;
class SceneStopCtrl;
class SceneMsgCtrl;
class ScreenCoverCtrl;
class AudioDirector;
class GraphicsInitArg;

class Scene : public NerveExecutor,
              public IUseAudioKeeper,
              public IUseCamera,
              public IUseSceneObjHolder {
public:
    Scene(const char* name);
    ~Scene() override;

    virtual void init(const SceneInitInfo& info);
    virtual void appear();
    virtual void kill();
    virtual void movement();
    virtual void control();
    virtual void drawMain() const;
    virtual void drawSub() const;

    AudioKeeper* getAudioKeeper() const override;
    SceneObjHolder* getSceneObjHolder() const override;
    CameraDirector* getCameraDirector() const override;
    NerveKeeper* getNerveKeeper() const override;

    void initializeAsync(const SceneInitInfo& info);
    void initSceneObjHolder(SceneObjHolder* holder);
    void initAndLoadStageResource(const char*, int);
    void initLiveActorKit(const SceneInitInfo& info, int, int, int);
    void initLiveActorKitImpl(const SceneInitInfo& info, int, int, int);
    void initDrawSystemInfo(const SceneInitInfo& info);
    void initLiveActorKitWithGraphics(const GraphicsInitArg&, const SceneInitInfo& info, int,
                                      int, int);
    void initLayoutKit(const SceneInitInfo& info);
    void initSceneStopCtrl();
    void initSceneMsgCtrl();
    void initScreenCoverCtrl();
    void endInit(const ActorInitInfo& info);

    bool getIsAlive() const { return isAlive; }

private:
    bool isAlive;
    sead::FixedSafeString<64> mName;
    StageResourceKeeper* mStageResourceKeeper;
    LiveActorKit* mLiveActorKit;
    LayoutKit* mLayoutKit;
    SceneObjHolder* mSceneObjHolder;
    SceneStopCtrl* mSceneStopCtrl;
    SceneMsgCtrl* mSceneMsgCtrl;
    ScreenCoverCtrl* mScreenCoverCtrl;
    AudioDirector* mAudioDirector;
    AudioKeeper* mAudioKeeper;
    void* scene_filler[1];
};
}  // namespace al
