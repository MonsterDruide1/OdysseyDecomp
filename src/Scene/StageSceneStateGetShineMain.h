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

class DoorSnow;
class GameDataHolder;
class QuestInfo;
class ScenarioStartCameraHolder;
class Shine;
class StageSceneLayout;

class StageSceneStateScenarioCamera;

class StageSceneStateGetShineMain : public al::HostStateBase<al::Scene> {
public:
    StageSceneStateGetShineMain(const char*, al::Scene*, StageSceneLayout*,
                                const al::SceneInitInfo&, const al::ActorInitInfo&,
                                const al::LayoutInitInfo&, al::LiveActor*,
                                ScenarioStartCameraHolder*, GameDataHolder*);
    ~StageSceneStateGetShineMain() override;

    void appear() override;
    void kill() override;

    void setScenarioCameraState(StageSceneStateScenarioCamera*);

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
    QuestInfo* getQuestInfo() const;

    al::LiveActor* getCapManHero() const { return mCapManHero; }

    void setIsEndAfterDemoLand(bool isEndAfterDemoLand) {
        mIsEndAfterDemoLand = isEndAfterDemoLand;
    }

private:
    al::SimpleLayoutAppearWaitEnd* mMoonGetLayout = nullptr;
    al::WipeSimple* mResultMainStartWipe = nullptr;
    al::WipeSimple* mResultMainStartBackWipe = nullptr;
    al::WipeSimple* mResultMainWipe = nullptr;
    al::SimpleLayoutAppearWaitEnd* mResultMainLayout = nullptr;
    al::CameraTicket* mDemoAnimCameraTicket = nullptr;
    sead::Matrix34f mDemoAnimCameraMtx;
    al::LiveActor* mCapManHero = nullptr;
    al::LiveActor* mDemoShine = nullptr;
    al::LiveActor* mDemoDepthMask = nullptr;
    al::LiveActor* mShineReactionActor = nullptr;
    StageSceneLayout* mStageSceneLayout = nullptr;
    ScenarioStartCameraHolder* mScenarioStartCameraHolder = nullptr;
    Shine* mShine = nullptr;
    GameDataHolder* mGameDataHolder = nullptr;
    DoorSnow* mDoorSnow = nullptr;
    bool mIsEndAfterDemoLand = false;
    void* _d0 = nullptr;
};

static_assert(sizeof(StageSceneStateGetShineMain) == 0xD8);
