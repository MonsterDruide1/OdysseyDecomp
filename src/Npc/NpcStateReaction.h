#pragma once

#include "Library/Nerve/NerveStateBase.h"

class NpcStateReactionParam;

namespace al {
class LiveActor;
}  // namespace al

class NpcStateReaction : public al::NerveStateBase {
public:
    static NpcStateReaction* create(al::LiveActor* actor, const NpcStateReactionParam* param);

    virtual bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                            al::HitSensor* self);
    virtual bool receiveMsgWithoutTrample(const al::SensorMsg* message, al::HitSensor* other,
                                          al::HitSensor* self);
    virtual bool receiveMsgNoReaction(const al::SensorMsg* message, al::HitSensor* other,
                                      al::HitSensor* self);
    virtual bool isCapReaction() const;
};
