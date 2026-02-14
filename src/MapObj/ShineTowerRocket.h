#pragma once

#include <basis/seadTypes.h>

#include "Library/Event/IEventFlowEventReceiver.h"
#include "Library/LiveActor/LiveActor.h"

#include "Demo/IUseDemoSkip.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al
class ShineTowerCommonKeeper;

namespace rs {
void setupHomeMeter(al::LiveActor*);
void setupHomeMeterFitherParam(al::LiveActor*, ShineTowerCommonKeeper*);
void setupHomeSticker(al::LiveActor*);
void setupHomeCompLight(al::LiveActor*);
void getHomeArchiveName(const al::LiveActor*);
}  // namespace rs

class ShineTowerRocket : public al::LiveActor,
                         public al::IEventFlowEventReceiver,
                         public IUseDemoSkip {
public:
    ShineTowerRocket(const char*);
    void init(const al::ActorInitInfo& info) override;
    void onSwitchDither();
    void offSwitchDither();
    void makeActorDead() override;
    void makeActorAlive() override;
    void initAfterPlacement() override;
    void startClipped() override;
    void control() override;
    bool isActiveDirtyModel() const;
    void calcAnim() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveEvent(const al::EventFlowEventData* event) override;
    void tryStartEntranceCamera(s32);
    bool isFirstDemo() const override;
    bool isEnableSkipDemo() const override;
    void skipDemo() override;
    void exeWait();
    void updateParts();
    bool isNearPlayerEntrance() const;
    void exeReaction();
    void exeDemoPrepare();
    bool tryStartDemo();
    void tryEndEntranceCamera();
    void exeDemoWalkPlayerToPoint();
    void calcPlayerPoseForPayDemo();
    void tryStartHitReactionDemoStart();
    void exeDemoAppearShine();
    void exeDemoWaitAfterAppearShine();
    bool tryLevelUp();
    void exeDemoWaitBeforeScaleUpDirect();
    void calcCameraMtxMeterUpPrev();
    void exeDemoScaleUp();
    void exeDemoMeterRotate();
    void calcCameraMtx();
    void setupRotateMeter();
    void exeDemoMeterUpPrev();
    void exeDemoMeterUp();
    void exeDemoMeterUpPost();
    void exeDemoTutorialShine();
    void exeDemoSelectGoOtherWorld();
    void exeDemoAwardMoon();
    void exeDemoUpLevelCamera();
    void calcCameraMtxLevelUp();
    void exeDemoUpLevel();
    void exeDemoInformPowerUp();
    void exeDemoInformPowerUpMessage();
    void exeDemoKoopaShip();
    void exeDemoKoopaShipFade();
    void exeDemoUpLevelCloseFade();
    void exeDemoUpLevelWaitFade();
    void exeDemoUpLevelOpenFade();
    void exeDemoInformNewHome();
    void exeDemoInformNewHomeMessage();
    void exeDemoInformPeachCastleCap();
    void exeDemoInformRepairHome();
    void exeDemoInformNewItem();
    void exeDemoInformCompleteShineFadeIn();
    void exeDemoInformCompleteShineFadeWait();
    void exeDemoInformCompleteShineFadeOut();
    void exeDemoInformCompleteShine();
    void exeDemoWarpWorld();
    void exeWaitDemo();
    void exeDemoAppearPlayerFromHome();
    bool isActiveDamageModel() const;
    void exeDemoAppearPlayerFromHomeAfter();
    void exeDemoReturnToHome();
    bool isActiveDirtyOrClashModel() const;
    void exeDemoWorldTakeoff();
    void exeDemoWorldTakeoffNext();
    void setupWorldTakeoffPose(bool);
    void exeDemoAppearFromEntrance();
    void exeDemoWorldTakeoffForDebug();
    void exeNoStart();
    void exeNoStartEarth();
    void exeNoStartEnter();
    void exeBackDoor();
    void exeNoStartAndCoin();
    void exeGoToWorldMapWithCamera();
    void setupWorldMapCameraParam();
    void exeGoToWorldMapWithFade();
    void exeWorldMap();
    void cancelWorldMap();
    void decideWorldMap(s32);
    bool isWorldMap() const;
    void startDemoAppearPlayerFromHome();
    void startDemoReturnToHome();
    void updatePartsByDamage();
    bool isActiveClashModel() const;

private:
    char filler[0x3c8];
};

static_assert(sizeof(ShineTowerRocket) == 0x4e0);
