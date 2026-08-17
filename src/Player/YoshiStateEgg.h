#pragma once

#include "Library/Nerve/NerveStateBase.h"

class YoshiEgg;
class IUsePlayerCollision;
class PlayerModelChangerYoshi;
class PlayerConst;

namespace al {
class WaterSurfaceFinder;
struct ActorInitInfo;
}  // namespace al

class YoshiStateEgg : public al::ActorStateBase {
public:
    explicit YoshiStateEgg(const al::ActorInitInfo&, al::LiveActor*, const IUsePlayerCollision*,
                           const PlayerConst*, const al::WaterSurfaceFinder*,
                           PlayerModelChangerYoshi*);
    ~YoshiStateEgg() override;

    void appear() override;
    void kill() override;

    bool reactionCollidedCollisionCode();

    void exeWait();
    void exeAppear();

private:
    IUsePlayerCollision* mCollisionUser;
    PlayerConst* mPlayerConst;
    al::WaterSurfaceFinder* mWaterSurfaceFinder;
    YoshiEgg* mYoshiEgg = nullptr;
    PlayerModelChangerYoshi* mModelChanger;
    bool mIsUnk1;
};

static_assert(sizeof(YoshiStateEgg) == 0x50);
