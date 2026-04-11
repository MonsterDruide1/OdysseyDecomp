#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class CollisionObj;
class HitSensor;
class SensorMsg;
}  // namespace al

class BossKnuckleFix : public al::LiveActor {
public:
    BossKnuckleFix(const char* name);

    void init(const al::ActorInitInfo& info) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void exeWait();
    void exeReaction();
    void exeReactionLarge();

private:
    al::CollisionObj* mCollisionObj = nullptr;
    s32 mReactionCount = 0;
};

static_assert(sizeof(BossKnuckleFix) == 0x118);
