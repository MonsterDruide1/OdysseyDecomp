#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class HitSensor;
class LiveActor;
class SensorMsg;
}  // namespace al

class BlockStateSingleItem : public al::ActorStateBase {
public:
    BlockStateSingleItem(al::LiveActor* actor, s32 itemType, bool isSkipStartWaitAction);

    void init() override;

    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self);
    bool isReaction() const;
    bool isEnableAppearItem() const;
    void setItemOffsetY(f32 itemOffsetY);
    void autoGet(al::HitSensor* sensor);
    void exeWait();
    void exeReaction();
    void exeReactionHipDrop();

private:
    s32 mItemType = 0;
    bool mIsNoWaitAction = false;
    al::HitSensor* mAttackedSensor = nullptr;
    f32 mItemOffsetY = 0.0f;
};

static_assert(sizeof(BlockStateSingleItem) == 0x38);
