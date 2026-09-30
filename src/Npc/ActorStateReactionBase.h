#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class HitSensor;
class SensorMsg;
}  // namespace al

class ActorStateReactionBase : public al::ActorStateBase {
public:
    using al::ActorStateBase::ActorStateBase;

    virtual bool receiveMsg(const al::SensorMsg* msg, al::HitSensor* other,
                            al::HitSensor* self) = 0;
    virtual bool receiveMsgWithoutTrample(const al::SensorMsg* msg, al::HitSensor* other,
                                          al::HitSensor* self) = 0;
    virtual bool receiveMsgNoReaction(const al::SensorMsg* msg, al::HitSensor* other,
                                      al::HitSensor* self) = 0;
    virtual bool isCapReaction() const = 0;
};
