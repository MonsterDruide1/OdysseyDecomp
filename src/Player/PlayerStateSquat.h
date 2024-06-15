#pragma once

#include "Library/Nerve/NerveStateBase.h"

class PlayerConst;
class PlayerAnimator;
class PlayerInput;
class IUsePlayerCollision;
class IPlayerModelChanger;
class PlayerTrigger;
class PlayerActionTurnControl;
class IJudge;

class PlayerStateSquat : public al::ActorStateBase {
public:
    PlayerStateSquat(al::LiveActor*, PlayerConst const*, PlayerAnimator*, PlayerInput const*,
                     IUsePlayerCollision const*, IPlayerModelChanger const*, PlayerTrigger const*,
                     IJudge*, IJudge*, IJudge*);
    ~PlayerStateSquat() override;

    void appear() override;

    bool isEnableLongJump() const;
    void setNerveStandUpOrKill();
    void fixPressRumble();

    void exeBrake();
    void exeWait();
    void exeWalk();
    void exeStandUp();

private:
    const PlayerConst* mConst;
    PlayerAnimator* mAnimator;
    const PlayerInput* mInput;
    const IUsePlayerCollision* mCollision;
    const IPlayerModelChanger* mModelChanger;
    const PlayerTrigger* mTrigger;
    PlayerActionTurnControl* mActionTurnControl;
    IJudge* mJudgeStartSquat;
    IJudge* mJudgeStartRun;
    IJudge* mJudgeEnableStandUp;
    bool someFlag = false;
};
