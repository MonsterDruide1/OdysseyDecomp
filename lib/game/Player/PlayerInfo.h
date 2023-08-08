#pragma once

namespace al {
class FootPrintHolder;
class HitSensor;
}  // namespace al

class HackCap;
class IUseCeilingCheck;
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
class PlayerJudgeSameNerve;
class PlayerJudgeSameNerve;
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
    PlayerModelChangerHakoniwa* mPlayerModelChangerHakoniwa = nullptr;
    PlayerOxygen* mPlayerOxygen = nullptr;
    PlayerAnimator* mPlayerAnimator = nullptr;
    PlayerBindKeeper* mPlayerBindKeeper = nullptr;
    PlayerDamageKeeper* mPlayerDamageKeeper = nullptr;
    PlayerDemoActionFlag* mPlayerDemoActionFlag = nullptr;
    PlayerEquipmentUser* mPlayerEquipmentUser = nullptr;
    HackCap* mHackCap = nullptr;
    WorldEndBorderKeeper* mWorldEndBorderKeeper = nullptr;
    PlayerCarryKeeper* mPlayerCarryKeeper = nullptr;
    PlayerJointControlKeeper* mPlayerJointControlKeeper = nullptr;
    PlayerCounterIceWater* mPlayerCounterIceWater = nullptr;
    PlayerStainControl* mPlayerStainControl = nullptr;
    al::FootPrintHolder* mFootPrintHolder = nullptr;
    al::HitSensor* mBodyHitSensor = nullptr;
    PlayerFormSensorCollisionArranger* mPlayerFormSensorCollisionArranger = nullptr;
    PlayerInput* mPlayerInput = nullptr;
    IUseCeilingCheck* mIUseCeilingCheck = nullptr;
    PlayerModelHolder* mPlayerModelHolder = nullptr;
    PlayerHackKeeper* mPlayerHackKeeper = nullptr;
    PlayerCapManHeroEyesControl* mPlayerCapManHeroEyesControl = nullptr;
    PlayerRecoverySafetyPoint* mPlayerRecoverySafetyPoint = nullptr;
    PlayerCostumeInfo* mPlayerCostumeInfo = nullptr;
    PlayerJudgeCameraInWater* mPlayerJudgeCameraInWater = nullptr;
    PlayerJudgeTalkGround* mPlayerJudgeTalkGround = nullptr;
    PlayerJudgeTalkSwim* mPlayerJudgeTalkSwim = nullptr;
    PlayerJudgeDead* mPlayerJudgeDead = nullptr;
    PlayerJudgeDeadWipeStart* mPlayerJudgeDeadWipeStart = nullptr;
    PlayerJudgeDrawForward* mPlayerJudgeDrawForward = nullptr;
    PlayerJudgeSameNerve* mPlayerJudgeNrvPoleClimb = nullptr;
    PlayerJudgeSameNerve* mPlayerJudgeNrvGrabCeil = nullptr;
    PlayerJudgeSameNerve* mPlayerJudgeNrvWallCatch = nullptr;
    PlayerJudgeActiveCameraSubjective* mPlayerJudgeActiveCameraSubjective = nullptr;
    PlayerJudgeSameNerve* mPlayerJudgeNrvSwim = nullptr;
    PlayerJudgeFailureCameraSubjective* mPlayerJudgeFailureCameraSubjective = nullptr;
    PlayerJudgeSafetyPointRecovery* mPlayerJudgeSafetyPointRecovery = nullptr;
    PlayerJudgeStatusPoleClimb* mPlayerJudgeStatusPoleClimb = nullptr;
    PlayerJudgePlaySwitchOnAreaWaitAnim* mPlayerJudgePlaySwitchOnAreaWaitAnim = nullptr;
    PlayerJudgeSleep* mPlayerJudgeSleep = nullptr;
    PlayerJudgeEnableGuideArrow* mPlayerJudgeEnableGuideArrow = nullptr;
    PlayerJudgeEnablePeachAmiibo* mPlayerJudgeEnablePeachAmiibo = nullptr;
    bool mIsMoon = false;
};

static_assert(sizeof(PlayerInfo) == 0x150);
