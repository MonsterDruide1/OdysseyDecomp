#pragma once

#include "Npc/ActorStateReactionBase.h"

namespace al {
class HitSensor;
class LiveActor;
class SensorMsg;
}  // namespace al

class NpcStateReactionParam;

class NpcStateReaction : public ActorStateReactionBase {
public:
    NpcStateReaction(al::LiveActor*, bool);

    static NpcStateReaction* create(al::LiveActor*, const NpcStateReactionParam*);
    static NpcStateReaction* createForHuman(al::LiveActor*, const NpcStateReactionParam*);
    static NpcStateReaction* createForHackActor(al::LiveActor*, const NpcStateReactionParam*);

    void appear() override;
    void kill() override;

    bool receiveMsg(const al::SensorMsg*, al::HitSensor* other, al::HitSensor* self) override;
    bool receiveMsgWithoutTrample(const al::SensorMsg*, al::HitSensor* other,
                                  al::HitSensor* self) override;
    bool receiveMsgNoReaction(const al::SensorMsg*, al::HitSensor* other,
                              al::HitSensor* self) override;
    bool isCapReaction() const override;

    void exeReaction();
    void exeCapReaction();

private:
    const NpcStateReactionParam* mParam;
    bool mIsCapReflectInvalid;
    bool mIsHuman;
    bool _2a;
    bool mIsClippingInvalidBeforeReaction;
};

static_assert(sizeof(NpcStateReaction) == 0x30);
