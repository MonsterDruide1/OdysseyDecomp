#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
struct ActorInitInfo;
class WaterSurfaceFinder;
}  // namespace al

class IUsePlayerCollision;
class PlayerConst;
class PlayerModelChangerYoshi;
class YoshiEgg;

class YoshiStateEgg : public al::ActorStateBase {
public:
    YoshiStateEgg(const al::ActorInitInfo&, al::LiveActor*, const IUsePlayerCollision*,
                  const PlayerConst*, const al::WaterSurfaceFinder*, PlayerModelChangerYoshi*);

    void appear() override;
    void kill() override;

    bool reactionCollidedCollisionCode();

    void exeAppear();
    void exeWait();

private:
    const IUsePlayerCollision* mPlayerCollision;
    const PlayerConst* mPlayerConst;
    const al::WaterSurfaceFinder* mWaterSurfaceFinder;
    YoshiEgg* mYoshiEgg = nullptr;
    PlayerModelChangerYoshi* mPlayerModelChangerYoshi;
    bool _48 = true;
};

static_assert(sizeof(YoshiStateEgg) == 0x50);
