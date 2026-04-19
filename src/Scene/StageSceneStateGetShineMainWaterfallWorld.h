#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
struct ActorInitInfo;
class Scene;
}  // namespace al

class StageSceneStateGetShineMain;
class StageSceneStateGetShineMainLast;
class StageSceneStateRecoverLife;
class StageSceneStateScenarioCamera;
class WaterfallWorldFallDownBridge;

class StageSceneStateGetShineMainWaterfallWorld : public al::NerveStateBase {
public:
    static StageSceneStateGetShineMainWaterfallWorld* tryCreate(al::Scene* scene,
                                                                const al::ActorInitInfo& info);

    StageSceneStateGetShineMainWaterfallWorld(const char* name, al::Scene* scene,
                                              WaterfallWorldFallDownBridge* bridge);

    void init() override;
    void appear() override;
    void kill() override;

    void setStateGetShine(StageSceneStateGetShineMain* stateGetShine);
    void setStateScenarioCamera(StageSceneStateScenarioCamera* stateScenarioCamera);
    void setStateRecoverLife(StageSceneStateRecoverLife* stateRecoverLife);
    void setStateGetShineLast(StageSceneStateGetShineMainLast* stateGetShineLast);

    void exeDemoGetShine();
    void exeDemoDownBridge();
    void exeDemoScenarioCamera();
    void exeDemoRecoverLife();
    void exeDemoCapManHeroTalkFirstMoonGet();
    void exeDemoEnd();

private:
    al::Scene* mScene = nullptr;
    StageSceneStateGetShineMain* mStateGetShine = nullptr;
    StageSceneStateScenarioCamera* mStateScenarioCamera = nullptr;
    StageSceneStateRecoverLife* mStateRecoverLife = nullptr;
    StageSceneStateGetShineMainLast* mStateGetShineLast = nullptr;
    WaterfallWorldFallDownBridge* mBridge = nullptr;
};

static_assert(sizeof(StageSceneStateGetShineMainWaterfallWorld) == 0x48);
