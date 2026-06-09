#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
}  // namespace al

class BlockEmpty : public al::LiveActor {
public:
    BlockEmpty(const char*, const char*);
    void init(const al::ActorInitInfo&) override;
    void initAfterPlacement() override;
    void control() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void setShadowDropLength(f32);
    void startReaction();
    void startReactionTransparent();
    void exeWait();
    void exeReaction();
    void exeReactionTransparent();

private:
    char filler[0x28];
};

static_assert(sizeof(BlockEmpty) == 0x130);
