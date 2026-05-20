#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}

class IUsePlayerCollision;
class PlayerActionAirMoveControl;
class PlayerConst;
class PlayerInput;
class PlayerPuppet;
class PlayerTrigger;

class PlayerStateBind : public al::ActorStateBase {
public:
    PlayerStateBind(al::LiveActor* player, const PlayerConst* playerConst, const PlayerInput* input,
                    const PlayerPuppet* playerPuppet, const IUsePlayerCollision* collision,
                    PlayerTrigger* trigger);

    void appear() override;

    void endBind();
    void exeBind();
    void exeEndOnGround();
    void exeEndJump();
    void exeEndCapThrow();
    void exeEndFall();
    bool isEndOnGround() const;
    bool isEndAir() const;
    bool isEndCapThrow() const;
    bool isInvalidInput() const;

private:
    const PlayerConst* mConst = nullptr;
    const PlayerPuppet* mPuppet = nullptr;
    PlayerTrigger* mTrigger = nullptr;
    PlayerActionAirMoveControl* mAirMoveControl = nullptr;
};

static_assert(sizeof(PlayerStateBind) == 0x40);
