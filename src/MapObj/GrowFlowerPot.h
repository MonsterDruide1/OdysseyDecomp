#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class SensorMsg;
}  // namespace al

class GrowFlowerWatcher;
class Shine;

class GrowFlowerPot : public al::LiveActor {
public:
    GrowFlowerPot(const char* name, bool isLong, GrowFlowerWatcher* watcher);

    void init(const al::ActorInitInfo& info) override;
    void initSetShinePtrFromSeed(Shine* shine);
    void initAfterPlacement() override;
    void tryMaxGrowLevel();
    void setGrowFlowerTimeLocal(al::HitSensor* sensor);
    u32 calcGrowLevelFromImplantTime() const;
    void calcGrowLevelTarget();
    bool isGrowAlready() const;
    void control() override;
    void calcAnim() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void setShineFromSeed(Shine* shine);
    void growLevelFromGamaner();
    void exeNoSeed();
    void exeImplant();
    void growSetting();
    void startGrow();
    void tryStartActionReactionParts();
    bool updateGrow();
    void endGrow();
    void endPartsReactionTrans();
    void growLevelFromHosui();
    void exeGrow();
    bool isEnableGrow() const;
    void exeGrowWait();
    void tryStartActionWaitParts();
    void exeBerryWait();
    void exeAppearShine();
    void endAppearShine();
    void exeWaitShine();
    void exeResumeShine();
    void exeReaction();
    void exeReactionWater();
    u64 calcGrowLevelTargetLocal(u32 growLevel);
    bool isEqualPlacementId(const char* placementId) const;

    void setIsReactionWater(bool isReactionWater) { mIsReactionWater = isReactionWater; }

private:
    u8 _108[0x88]{};
    bool mIsReactionWater = false;
    u8 _191 = 0;
    u8 _192 = 0;
    u8 _193[5]{};
};

static_assert(sizeof(GrowFlowerPot) == 0x198);
