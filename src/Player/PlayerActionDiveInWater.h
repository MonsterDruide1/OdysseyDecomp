#pragma once

class IPlayerModelChanger;
class PlayerAnimator;
class PlayerCarryKeeper;
class PlayerJudgeDiveInWater;

class PlayerActionDiveInWater {
public:
    PlayerActionDiveInWater(PlayerAnimator* animator, const IPlayerModelChanger* modelChanger,
                            const PlayerCarryKeeper* carryKeeper,
                            const PlayerJudgeDiveInWater* judgeDiveInWater);
    bool tryChangeDiveInWaterAnim();
    bool judgeEnableDiveInWater() const;
    bool isDiveInWaterAnim() const;

private:
    PlayerAnimator* mAnimator;
    const IPlayerModelChanger* mModelChanger;
    const PlayerCarryKeeper* mCarryKeeper;
    const PlayerJudgeDiveInWater* mJudgeDiveInWater;
};
