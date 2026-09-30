#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}  // namespace al

class IPlayerModelChanger;
class IUseDimension;
class IUsePlayerCollision;
class PlayerAnimator;
class PlayerConst;
class PlayerInput;
class PlayerStateRun;
class PlayerStateRun2D;

class PlayerStateRun2D3D : public al::ActorStateBase {
public:
    PlayerStateRun2D3D(al::LiveActor* player, const PlayerConst* pConst,
                       const IUseDimension* dimension, const IPlayerModelChanger* modelChanger,
                       const PlayerInput* input, const IUsePlayerCollision* collision,
                       PlayerAnimator* animator);

    void appear() override;

    void syncModel();
    void exeRun3D();
    void exeRun2D();

private:
    const PlayerConst* mConst = nullptr;
    const IPlayerModelChanger* mModelChanger = nullptr;
    PlayerStateRun* mStateRun3D = nullptr;
    PlayerStateRun2D* mStateRun2D = nullptr;
};

static_assert(sizeof(PlayerStateRun2D3D) == 0x40);
