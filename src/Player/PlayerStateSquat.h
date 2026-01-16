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
    PlayerStateSquat(al::LiveActor* player, const PlayerConst* pConst, PlayerAnimator* animator,
                     const PlayerInput* input, const IUsePlayerCollision* collider,
                     const IPlayerModelChanger* modelChanger, const PlayerTrigger* trigger,
                     IJudge* judgeStartSquat, IJudge* judgeStartRun, IJudge* judgeEnableStandUp);

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
    bool mIsSkipSquatStartAnim = false;
};
