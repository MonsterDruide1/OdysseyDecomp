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
    YoshiStateEgg(const al::ActorInitInfo& info, al::LiveActor* player,
                  const IUsePlayerCollision* collision, const PlayerConst* playerConst,
                  const al::WaterSurfaceFinder* waterSurfaceFinder,
                  PlayerModelChangerYoshi* modelChanger);

    void appear() override;
    void kill() override;

    void exeAppear();
    void exeWait();
    bool reactionCollidedCollisionCode();

private:
    const IUsePlayerCollision* mCollision = nullptr;
    const PlayerConst* mConst = nullptr;
    const al::WaterSurfaceFinder* mWaterSurfaceFinder = nullptr;
    YoshiEgg* mEgg = nullptr;
    PlayerModelChangerYoshi* mModelChanger = nullptr;
    bool mIsFirstPlacement = true;
};

static_assert(sizeof(YoshiStateEgg) == 0x50);
