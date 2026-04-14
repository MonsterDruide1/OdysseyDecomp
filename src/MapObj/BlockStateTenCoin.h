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
    ~BlockStateTenCoin() override;

    void init() override;
    void control() override;

    void autoGet(al::HitSensor* sensor);
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self);
    bool isReaction() const;
    bool isEnableAppearItem() const;
    void exeWait();
    void exeReaction();
    void exeReactionHipDrop();
    void exeGetAuto();

private:
    friend class BlockBrick;
    friend class BlockBrick2D;
    friend class BlockQuestion;
    friend class BlockQuestion2D;

    s32 mCoinCount = 10;
    s32 mWaitFrame = 130;
    s32 mReactionCooldown = 0;
    bool mIsSkipStartWaitAction = false;
    al::HitSensor* mAttackerSensor = nullptr;
    f32 mItemOffsetY = 0.0f;
};

static_assert(sizeof(BlockStateTenCoin) == 0x40);
