#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class HitSensor;
class LiveActor;
class SensorMsg;
}  // namespace al

class BlockStateNoItem : public al::ActorStateBase {
public:
    BlockStateNoItem(al::LiveActor* actor);

    void init() override;
    void appear() override;

    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self);
    void doReaction();
    bool isReaction() const;
    void exeWait();
    void exeReaction();
};

static_assert(sizeof(BlockStateNoItem) == 0x20);
