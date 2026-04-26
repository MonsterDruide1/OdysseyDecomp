#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>

#include "Library/LiveActor/LiveActor.h"

#include "Demo/IUseDemoSkip.h"

namespace al {
class AddDemoInfo;
struct ActorInitInfo;
class CameraTicket;
class HitSensor;
class PlacementId;
class SensorMsg;
}  // namespace al
class CapMessageEnableChecker;
class ChangeStageInfo;
class GoalMark;

class MoonRock : public al::LiveActor, public IUseDemoSkip {
public:
    MoonRock(const char* actorName) : al::LiveActor(actorName) {}

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void makeActorAlive() override;
    void kill() override;
    bool isFirstDemo() const override;
    bool isEnableSkipDemo() const override;
    void skipDemo() override;
    void movement() override;
    void calcAnim() override;
    GoalMark* getGoalMarkForCapTalk() const;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void exeWait();
    void exeReaction();
    void exeBreak();
    void exeReadyRequestStartDemoMoonSetStart();
    void exeRequestStartDemoMoonSetStart();
    void exeDemoMoonSetStart();
    void exeEndDemoMoonSetStart();
    void exeChangeScene();

private:
    GoalMark* mGoalMark;
    al::LiveActor* mWreckageActor;
    al::PlacementId* mPlacementId;
    al::CameraTicket* mDemoCamera;
    ChangeStageInfo* mChangeStageInfo;
    CapMessageEnableChecker* mCapMessageEnableChecker;
    al::AddDemoInfo* mAddDemoInfo;
    sead::Matrix34f mDemoCameraPosition;
    bool mIsOpenedOnInit;
    bool mHasDemoCameraPosition;
    s32 mWreckagePoseType;
};

static_assert(sizeof(MoonRock) == 0x180);
