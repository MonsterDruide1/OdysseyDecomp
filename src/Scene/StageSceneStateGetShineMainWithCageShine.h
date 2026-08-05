#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
struct ActorInitInfo;
class Scene;
}  // namespace al

class StageSceneStateBreakCageShine;
class StageSceneStateGetShineMain;
class StageSceneStateGetShineMainLast;
class StageSceneStateRecoverLife;
class StageSceneStateScenarioCamera;

class StageSceneStateGetShineMainWithCageShine : public al::HostStateBase<al::Scene> {
public:
    StageSceneStateGetShineMainWithCageShine(const char* name, al::Scene* scene,
                                             StageSceneStateBreakCageShine* breakCageShine);

    void init() override;
    void appear() override;
    void kill() override;

    void setState(StageSceneStateGetShineMain* getShineMain,
                  StageSceneStateScenarioCamera* scenarioCamera,
                  StageSceneStateRecoverLife* recoverLife,
                  StageSceneStateGetShineMainLast* getShineMainLast);
    void exeDemoGetShine();
    void exeDemoBreakCageShine();
    void exeDemoScenarioCamera();
    void exeDemoRecoverLife();
    void exeDemoEnd();

    static StageSceneStateGetShineMainWithCageShine* tryCreate(al::Scene* scene,
                                                               const al::ActorInitInfo& initInfo);

private:
    StageSceneStateGetShineMain* mStateGetShineMain = nullptr;
    StageSceneStateScenarioCamera* mStateScenarioCamera = nullptr;
    StageSceneStateRecoverLife* mStateRecoverLife = nullptr;
    StageSceneStateGetShineMainLast* mStateGetShineMainLast = nullptr;
    StageSceneStateBreakCageShine* mStateBreakCageShine = nullptr;
};

static_assert(sizeof(StageSceneStateGetShineMainWithCageShine) == 0x48);
