#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class HitSensor;
class LiveActor;
class SensorMsg;
}  // namespace al

class BlockStateTenCoin : public al::ActorStateBase {
public:
    BlockStateTenCoin(al::LiveActor* actor, bool isSkipStartWaitAction);

    void init() override;
    void control() override;

    void autoGet(al::HitSensor* sensor);
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self);
    bool isReaction() const;
    bool isEnableAppearItem() const;

    void setItemOffsetY(f32 itemOffsetY) { mItemOffsetY = itemOffsetY; }

    void exeWait();
    void exeReaction();
    void exeReactionHipDrop();
    void exeGetAuto();

private:
    s32 mCoinCount = 10;
    s32 mTimer = 130;
    s32 mReactionCooldown = 0;
    bool mIsWaitSyncStage = false;
    al::HitSensor* mAttackedSensor = nullptr;
    f32 mItemOffsetY = 0.0f;
};

static_assert(sizeof(BlockStateTenCoin) == 0x40);
