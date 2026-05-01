#pragma once

#include <math/seadMatrix.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
struct ActorInitInfo;
class CameraTicket;
class LayoutInitInfo;
class LiveActor;
class Scene;
struct SceneInitInfo;
class SimpleLayoutAppearWaitEnd;
class WipeSimple;
}  // namespace al

class GameDataHolder;
class QuestInfo;
class ScenarioStartCameraHolder;
class Shine;
class StageSceneLayout;
class StageSceneStateRecoverLife;
class StageSceneStateScenarioCamera;

class StageSceneStateGetShineMain : public al::HostStateBase<al::Scene> {
public:
    StageSceneStateGetShineMain(const char* name, al::Scene* scene, StageSceneLayout* layout,
                                const al::SceneInitInfo& sceneInitInfo,
                                const al::ActorInitInfo& actorInitInfo,
                                const al::LayoutInitInfo& layoutInitInfo,
                                al::LiveActor* playerActor,
                                ScenarioStartCameraHolder* scenarioStartCameraHolder,
                                GameDataHolder* gameDataHolder);

    void appear() override;
    void kill() override;

    void setScenarioCameraState(StageSceneStateScenarioCamera* scenarioCamera);
    void exeWaitPlayerAnimFirst();
    void exeDemoGetStart();
    void exeDemoAppearShineGetLayout();
    void exeDemoShineCount();
    void exeDemoWipeClose();
    void exeDemoWipeWait();
    void exeDemoLand();
    void exeDemoOpenDoorSnow();
    void exeDemoRiseMapParts();
    void exeDemoScenarioCamera();
    void exeDemoLifeRecover();
    void exeDemoEnd();
    void exeDemoEndCity();
    void startDemoEnd();
    bool isDrawChromakey() const;
    const QuestInfo* getQuestInfo() const;

    void setSkipDemoAfterGet(bool isSkipDemoAfterGet) { mIsSkipDemoAfterGet = isSkipDemoAfterGet; }

private:
    al::SimpleLayoutAppearWaitEnd* mResultMainGetLayout = nullptr;
    al::WipeSimple* mWipeResultMainStart = nullptr;
    al::WipeSimple* mWipeResultMainBack = nullptr;
    al::WipeSimple* mWipeResultMain = nullptr;
    al::SimpleLayoutAppearWaitEnd* mResultMainLayout = nullptr;
    al::CameraTicket* mDemoCameraTicket = nullptr;
    sead::Matrix34f mDemoCameraMtx;
    al::LiveActor* mDemoCapManHero = nullptr;
    al::LiveActor* mPlayerActor = nullptr;
    al::LiveActor* mDemoGetShineWipeActor = nullptr;
    al::LiveActor* mStageShineActor = nullptr;
    StageSceneLayout* mLayout = nullptr;
    ScenarioStartCameraHolder* mScenarioStartCameraHolder = nullptr;
    Shine* mShine = nullptr;
    GameDataHolder* mGameDataHolder = nullptr;
    StageSceneStateScenarioCamera* mStateScenarioCamera = nullptr;
    bool mIsSkipDemoAfterGet = false;
    const QuestInfo* mQuestInfo = nullptr;
};

static_assert(sizeof(StageSceneStateGetShineMain) == 0xd8);
