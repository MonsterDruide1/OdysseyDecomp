#pragma once

#include "Player/PlayerJudgeActiveCameraSubjective.h"
#include "Player/PlayerJudgeCameraInWater.h"
#include "Player/PlayerJudgeEnableGuideArrow.h"
#include "Player/PlayerJudgeEnablePeachAmiibo.h"
#include "Player/PlayerJudgePlaySwitchOnAreaWaitAnim.h"
#include "Player/PlayerJudgeSafetyPointRecovery.h"
#include "Player/PlayerJudgeSameNerve.h"
#include "Player/PlayerJudgeSleep.h"
#include "Player/PlayerJudgeStatusPoleClimb.h"
#include "Player/PlayerJudgeTalkGround.h"
#include "Player/PlayerJudgeTalkSwim.h"

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
class PlayerJudgeDead;
class PlayerJudgeDeadWipeStart;
class PlayerJudgeDrawForward;
class PlayerJudgeFailureCameraSubjective;
class PlayerModelChangerHakoniwa;
class PlayerModelHolder;
class PlayerOxygen;
class PlayerRecoverySafetyPoint;
class PlayerStainControl;
class WorldEndBorderKeeper;

class PlayerInfo {
public:
    PlayerInfo();

    PlayerModelChangerHakoniwa* getModelChangerHakoniwa() const { return mModelChangerHakoniwa; }

    PlayerOxygen* getOxygen() const { return mOxygen; }

    PlayerAnimator* getAnimator() const { return mAnimator; }

    PlayerBindKeeper* getBindKeeper() const { return mBindKeeper; }

    PlayerDamageKeeper* getDamageKeeper() const { return mDamageKeeper; }

    PlayerEquipmentUser* getEquipmentUser() const { return mEquipmentUser; }

    HackCap* getHackCap() const { return mHackCap; }

    PlayerCarryKeeper* getCarryKeeper() const { return mCarryKeeper; }

    PlayerJointControlKeeper* getPlayerJointControlKeeper() const { return mJointControlKeeper; }

    al::HitSensor* getBodyHitSensor() const { return mBodyHitSensor; }

    PlayerFormSensorCollisionArranger* getFormSensorCollisionArranger() const {
        return mFormSensorCollisionArranger;
    }

    PlayerInput* getInput() const { return mInput; }

    IUsePlayerCeilingCheck* getCeilingCheck() const { return mCeilingCheck; }

    PlayerModelHolder* getModelHolder() const { return mModelHolder; }

    PlayerHackKeeper* getHackKeeper() const { return mHackKeeper; }

    PlayerCostumeInfo* getCostumeInfo() const { return mCostumeInfo; }

    const IJudge* getJudgeCameraInWater() const { return mJudgeCameraInWater; }

    const IJudge* getJudgeTalkGround() const { return mJudgeTalkGround; }

    const IJudge* getJudgeTalkSwim() const { return mJudgeTalkSwim; }

    const IJudge* getJudgeNrvPoleClimb() const { return mJudgeNrvPoleClimb; }

    const IJudge* getJudgeNrvWallCatch() const { return mJudgeNrvWallCatch; }

    const IJudge* getJudgeActiveCameraSubjective() const { return mJudgeActiveCameraSubjective; }

    const IJudge* getJudgeNrvSwim() const { return mJudgeNrvSwim; }

    const IJudge* getJudgeSafetyPointRecovery() const { return mJudgeSafetyPointRecovery; }

    const IJudge* getJudgeStatusPoleClimb() const { return mJudgeStatusPoleClimb; }

    const IJudge* getJudgePlaySwitchOnAreaWaitAnim() const {
        return mJudgePlaySwitchOnAreaWaitAnim;
    }

    const IJudge* getJudgeSleep() const { return mJudgeSleep; }

    const IJudge* getJudgeEnableGuideArrow() const { return mJudgeEnableGuideArrow; }

    const IJudge* getJudgeEnablePeachAmiibo() const { return mJudgeEnablePeachAmiibo; }

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
