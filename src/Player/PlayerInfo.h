#pragma once

namespace al {
class FootPrintHolder;
class HitSensor;
}  // namespace al

class HackCap;
class IUsePlayerCeilingCheck;
class PlayerAnimator;
class PlayerBindKeeper;
class PlayerCapManHeroEyesControl;
class PlayerCarryKeeper;
class PlayerCostumeInfo;
class PlayerCounterIceWater;
class PlayerDamageKeeper;
class PlayerDemoActionFlag;
class PlayerEquipmentUser;
class PlayerFormSensorCollisionArranger;
class PlayerHackKeeper;
class PlayerInput;
class PlayerJointControlKeeper;
class PlayerJudgeActiveCameraSubjective;
class PlayerJudgeCameraInWater;
class PlayerJudgeDead;
class PlayerJudgeDeadWipeStart;
class PlayerJudgeDrawForward;
class PlayerJudgeEnableGuideArrow;
class PlayerJudgeEnablePeachAmiibo;
class PlayerJudgeFailureCameraSubjective;
class PlayerJudgePlaySwitchOnAreaWaitAnim;
class PlayerJudgeSafetyPointRecovery;
class PlayerJudgeSameNerve;
class PlayerJudgeSleep;
class PlayerJudgeStatusPoleClimb;
class PlayerJudgeTalkGround;
class PlayerJudgeTalkSwim;
class PlayerModelChangerHakoniwa;
class PlayerModelHolder;
class PlayerOxygen;
class PlayerRecoverySafetyPoint;
class PlayerStainControl;
class WorldEndBorderKeeper;

class PlayerInfo {
public:
    PlayerInfo();

private:
    PlayerModelChangerHakoniwa* mModelChangerHakoniwa = nullptr;
    PlayerOxygen* mOxygen = nullptr;
    PlayerAnimator* mAnimator = nullptr;
    PlayerBindKeeper* mBindKeeper = nullptr;
    PlayerDamageKeeper* mDamageKeeper = nullptr;
    PlayerDemoActionFlag* mDemoActionFlag = nullptr;
    PlayerEquipmentUser* mEquipmentUser = nullptr;
    HackCap* mHackCap = nullptr;
    WorldEndBorderKeeper* mWorldEndBorderKeeper = nullptr;
    PlayerCarryKeeper* mCarryKeeper = nullptr;
    PlayerJointControlKeeper* mJointControlKeeper = nullptr;
    PlayerCounterIceWater* mCounterIceWater = nullptr;
    PlayerStainControl* mStainControl = nullptr;
    al::FootPrintHolder* mFootPrintHolder = nullptr;
    al::HitSensor* mBodyHitSensor = nullptr;
    PlayerFormSensorCollisionArranger* mFormSensorCollisionArranger = nullptr;
    PlayerInput* mInput = nullptr;
    IUsePlayerCeilingCheck* mCeilingCheck = nullptr;
    PlayerModelHolder* mModelHolder = nullptr;
    PlayerHackKeeper* mHackKeeper = nullptr;
    PlayerCapManHeroEyesControl* mCapManHeroEyesControl = nullptr;
    PlayerRecoverySafetyPoint* mRecoverySafetyPoint = nullptr;
    PlayerCostumeInfo* mCostumeInfo = nullptr;
    PlayerJudgeCameraInWater* mJudgeCameraInWater = nullptr;
    PlayerJudgeTalkGround* mJudgeTalkGround = nullptr;
    PlayerJudgeTalkSwim* mJudgeTalkSwim = nullptr;
    PlayerJudgeDead* mJudgeDead = nullptr;
    PlayerJudgeDeadWipeStart* mJudgeDeadWipeStart = nullptr;
    PlayerJudgeDrawForward* mJudgeDrawForward = nullptr;
    PlayerJudgeSameNerve* mJudgeNrvPoleClimb = nullptr;
    PlayerJudgeSameNerve* mJudgeNrvGrabCeil = nullptr;
    PlayerJudgeSameNerve* mJudgeNrvWallCatch = nullptr;
    PlayerJudgeActiveCameraSubjective* mJudgeActiveCameraSubjective = nullptr;
    PlayerJudgeSameNerve* mJudgeNrvSwim = nullptr;
    PlayerJudgeFailureCameraSubjective* mJudgeFailureCameraSubjective = nullptr;
    PlayerJudgeSafetyPointRecovery* mJudgeSafetyPointRecovery = nullptr;
    PlayerJudgeStatusPoleClimb* mJudgeStatusPoleClimb = nullptr;
    PlayerJudgePlaySwitchOnAreaWaitAnim* mJudgePlaySwitchOnAreaWaitAnim = nullptr;
    PlayerJudgeSleep* mJudgeSleep = nullptr;
    PlayerJudgeEnableGuideArrow* mJudgeEnableGuideArrow = nullptr;
    PlayerJudgeEnablePeachAmiibo* mJudgeEnablePeachAmiibo = nullptr;
    bool mIsMoon = false;
};

static_assert(sizeof(PlayerInfo) == 0x150);
