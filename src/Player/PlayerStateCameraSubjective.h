#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class CameraTicket;
}
class IPlayerModelChanger;
class PlayerCarryKeeper;
class PlayerAnimator;
class PlayerConst;
class IUsePlayerCollision;
class PlayerInput;

class PlayerStateCameraSubjective : public al::ActorStateBase {
public:
    PlayerStateCameraSubjective(al::LiveActor* player, al::CameraTicket* cameraTicket,
                                IPlayerModelChanger* modelChanger, PlayerCarryKeeper* carryKeeper,
                                PlayerAnimator* animator, const PlayerConst* playerConst,
                                const IUsePlayerCollision* collider, const PlayerInput* input);

    void appear() override;
    void kill() override;
    void control() override;

    void interruptByDemo();
    bool isStateActiveCamera() const;

    void exeStart();
    void faceToCameraDir();
    void exeWait();
    void exeEnd();

private:
    const PlayerConst* mConst = nullptr;
    const IUsePlayerCollision* mCollider = nullptr;
    const PlayerInput* mInput = nullptr;
    s32 mTurnRate = 0;
    bool mIsInterruptByDemo = false;
    al::CameraTicket* mTicket = nullptr;
    IPlayerModelChanger* mModelChanger = nullptr;
    PlayerCarryKeeper* mCarryKeeper = nullptr;
    PlayerAnimator* mAnimator = nullptr;
};

static_assert(sizeof(PlayerStateCameraSubjective) == 0x60);
