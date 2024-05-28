#include "Player/PlayerActorHakoniwa.h"
#include "Library/Audio/AudioKeeper.h"
#include "Library/Base/String.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/LiveActor/SubActorKeeper.h"
#include "Library/Math/VectorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Player/ComboCounter.h"
#include "Library/Scene/SceneUtil.h"
#include "Library/Thread/FunctorV0M.h"
#include "Player/HackCap.h"
#include "Player/IUsePlayerCollision.h"
#include "Player/PlayerActorBase.h"
#include "Player/PlayerFunction.h"
#include "Player/PlayerInput.h"
#include "Player/PlayerInputFunction.h"
#include "Player/PlayerPainPartsKeeper.h"
#include "Player/PlayerTrigger.h"
#include "PlayerUtil.h"
#include "System/GameDataFunction.h"
#include "Util/ActorDimensionKeeper.h"
#include "Util/StageSceneFunction.h"
#include "playerUtil.h"

namespace {

NERVE_IMPL(PlayerActorHakoniwa, Wait);
NERVE_IMPL(PlayerActorHakoniwa, Squat);
NERVE_IMPL(PlayerActorHakoniwa, Run);
NERVE_IMPL(PlayerActorHakoniwa, Slope);
NERVE_IMPL(PlayerActorHakoniwa, Rolling);
NERVE_IMPL(PlayerActorHakoniwa, SpinCap);
NERVE_IMPL(PlayerActorHakoniwa, Jump);
NERVE_IMPL(PlayerActorHakoniwa, CapCatchPop);
NERVE_IMPL(PlayerActorHakoniwa, WallAir);
NERVE_IMPL(PlayerActorHakoniwa, WallCatch);
NERVE_IMPL(PlayerActorHakoniwa, GrabCeil);
NERVE_IMPL(PlayerActorHakoniwa, PoleClimb);
NERVE_IMPL(PlayerActorHakoniwa, HipDrop);
NERVE_IMPL(PlayerActorHakoniwa, HeadSliding);
NERVE_IMPL(PlayerActorHakoniwa, LongJump);
NERVE_IMPL(PlayerActorHakoniwa, SandSink);
NERVE_IMPL(PlayerActorHakoniwa, SandGeyser);
NERVE_IMPL(PlayerActorHakoniwa, Rise);
NERVE_IMPL(PlayerActorHakoniwa, Swim);
NERVE_IMPL(PlayerActorHakoniwa, Damage);
NERVE_IMPL(PlayerActorHakoniwa, DamageSwim);
NERVE_IMPL(PlayerActorHakoniwa, DamageFire);
NERVE_IMPL(PlayerActorHakoniwa, Press);
NERVE_IMPL(PlayerActorHakoniwa, Hack);
NERVE_IMPL(PlayerActorHakoniwa, EndHack);
NERVE_IMPL(PlayerActorHakoniwa, Bind);
NERVE_IMPL(PlayerActorHakoniwa, Camera);
NERVE_IMPL(PlayerActorHakoniwa, Abyss);
NERVE_IMPL(PlayerActorHakoniwa, Demo);
NERVE_IMPL(PlayerActorHakoniwa, Dead);
NERVE_IMPL(PlayerActorHakoniwa, Fall);

NERVE_MAKE(PlayerActorHakoniwa, Wait);
NERVE_MAKE(PlayerActorHakoniwa, Squat);
NERVE_MAKE(PlayerActorHakoniwa, Run);
NERVE_MAKE(PlayerActorHakoniwa, Slope);
NERVE_MAKE(PlayerActorHakoniwa, Rolling);
NERVE_MAKE(PlayerActorHakoniwa, SpinCap);
NERVE_MAKE(PlayerActorHakoniwa, Jump);
NERVE_MAKE(PlayerActorHakoniwa, CapCatchPop);
NERVE_MAKE(PlayerActorHakoniwa, WallAir);
NERVE_MAKE(PlayerActorHakoniwa, WallCatch);
NERVE_MAKE(PlayerActorHakoniwa, GrabCeil);
NERVE_MAKE(PlayerActorHakoniwa, PoleClimb);
NERVE_MAKE(PlayerActorHakoniwa, HipDrop);
NERVE_MAKE(PlayerActorHakoniwa, HeadSliding);
NERVE_MAKE(PlayerActorHakoniwa, LongJump);
NERVE_MAKE(PlayerActorHakoniwa, SandSink);
NERVE_MAKE(PlayerActorHakoniwa, SandGeyser);
NERVE_MAKE(PlayerActorHakoniwa, Rise);
NERVE_MAKE(PlayerActorHakoniwa, Swim);
NERVE_MAKE(PlayerActorHakoniwa, Damage);
NERVE_MAKE(PlayerActorHakoniwa, DamageSwim);
NERVE_MAKE(PlayerActorHakoniwa, DamageFire);
NERVE_MAKE(PlayerActorHakoniwa, Press);
NERVE_MAKE(PlayerActorHakoniwa, Hack);
NERVE_MAKE(PlayerActorHakoniwa, EndHack);
NERVE_MAKE(PlayerActorHakoniwa, Bind);
NERVE_MAKE(PlayerActorHakoniwa, Camera);
NERVE_MAKE(PlayerActorHakoniwa, Abyss);
NERVE_MAKE(PlayerActorHakoniwa, Demo);
NERVE_MAKE(PlayerActorHakoniwa, Dead);
NERVE_MAKE(PlayerActorHakoniwa, Fall);

}  // namespace

// NON_MATCHING: slightly different when stopping/restarting memset
PlayerActorHakoniwa::PlayerActorHakoniwa(const char* name)
    : PlayerActorBase(name), mComboCounter(new al::ComboCounter()) {}

// void initAfterPlacement() override;
// void movement() override;
// void attackSensor(al::HitSensor*, al::HitSensor*) override;
// bool receiveMsg(const al::SensorMsg*, al::HitSensor*, al::HitSensor*) override;
// void control() override;
// void updateCollider() override;

void PlayerActorHakoniwa::initPlayer(const al::ActorInitInfo& actorInfo,
                                     const PlayerInitInfo& playerInfo) {
    al::initActorWithArchiveName(this, actorInfo, "PlayerActorHakoniwa", nullptr);
    rs::initPlayerActorInfo(this, playerInfo);
    al::onCollide(this);

    bool isMoon = false;
    al::tryGetArg(&isMoon, actorInfo, "IsMoon");
    al::tryGetArg(&mIsLongShadow, actorInfo, "IsLongShadow");

    bool invalidateCap = false;
    al::tryGetArg(&invalidateCap, actorInfo, "IsInvalidateCap");
    if (invalidateCap)
        GameDataFunction::disableCapByPlacement(this);

    al::registerExecutorFunctor(
        "プレイヤー後処理", actorInfo.getExecuteDirector(),
        al::FunctorV0M<PlayerActorHakoniwa*, void (PlayerActorHakoniwa::*)(void)>(
            this, &PlayerActorHakoniwa::initAfterPlacement));
    mBodyHitSensor = al::getHitSensor(this, "Body");

    mActorDimensionKeeper = new ActorDimensionKeeper(this);
    mPlayerTrigger = new PlayerTrigger();

    al::AudioKeeper* audioKeeper = getAudioKeeper();
    PlayerModelHolder* modelHolder = new PlayerModelHolder(3);
    LiveActor* modelActor = new al::LiveActor("マリオモデル");
    const char* initPlayerModelName = rs::getInitPlayerModelName(playerInfo);
    PlayerCostumeInfo* costumeInfo = PlayerFunction::initMarioModelActor(
        modelActor, actorInfo, initPlayerModelName, rs::getInitCapTypeName(playerInfo), audioKeeper,
        rs::isClosetScenePlayer(playerInfo));
    modelHolder->registerModel(modelActor, "Normal");

    // NON_MATCHING: allocated/created differently
    al::StringTmp<128> initPlayerModelNameWithoutMario = "";
    al::tryReplaceString(&initPlayerModelNameWithoutMario, initPlayerModelName, "Mario", "");
    modelHolder->_10.format("%s", initPlayerModelNameWithoutMario.cstr());

    al::StringTmp<64> modelName2d = &sead::SafeString::cNullChar;
    if (costumeInfo->isEnableCostume2D()) {
        modelName2d.format("%s2D", initPlayerModelName);
    } else {
        modelName2d.format("Mario2D");
    }

    LiveActor* modelActor2d = new al::LiveActor("ドットマリオモデル");
    PlayerFunction::initMarioModelActor2D(modelActor2d, actorInfo, modelName2d.cstr(),
                                          PlayerFunction::isInvisibleCap(costumeInfo));
    modelHolder->registerModel(modelActor2d, "Normal2D");

    LiveActor* mario2DMini = new al::LiveActor("ドット死亡モデル");
    al::initChildActorWithArchiveNameNoPlacementInfo(mario2DMini, actorInfo, "Mario2DMini",
                                                     nullptr);
    mario2DMini->makeActorDead();
    modelHolder->registerModel(mario2DMini, "Mario2DMini");

    modelHolder->changeModel("Normal");
    mPlayerModelHolder = modelHolder;

    mPlayerPainPartsKeeper = new PlayerPainPartsKeeper(this, costumeInfo);

    if (rs::isNeedCreateNoseNeedle(playerInfo))
        mPlayerPainPartsKeeper->createNoseNeedle(mPlayerModelHolder, actorInfo);

    mPlayerConst = PlayerFunction::createMarioConst(isMoon ? "Moon" : "");
    mPlayerJointControlKeeper = new PlayerJointControlKeeper(
        mPlayerModelHolder->findModelActor("Normal"), mPlayerConst, costumeInfo->isNeedShrinkNose(),
        costumeInfo->isEnableBigEar(), costumeInfo->isFollowJoeStrap());

    PlayerColliderHakoniwa* collider = new PlayerColliderHakoniwa(this, mPlayerConst);
    collider->init();
    mPlayerColliderHakoniwa = collider;

    mPlayerInput = new PlayerInput(this, collider, this);
    mPlayerModelChangerHakoniwa = new PlayerModelChangerHakoniwa(
        this, mPlayerModelHolder, mPlayerPainPartsKeeper, costumeInfo, this);

    mPlayerAnimator = new PlayerAnimator(mPlayerModelHolder,
                                         PlayerFunction::createPlayerDitherAnimator(this, 200.0f));
    if (costumeInfo->isNeedFullFaceAnim())
        mPlayerAnimator->mIsNeedFullFaceAnim = true;

    mPlayerEffect = new PlayerEffect(this, mPlayerModelHolder, mPlayerJointControlKeeper->mtx);
    mPlayerContinuousJump = new PlayerContinuousJump(mPlayerConst);
    mPlayerContinuousLongJump = new PlayerContinuousLongJump(mPlayerConst);
    mPlayerDamageKeeper = new PlayerDamageKeeper(this, mPlayerEffect);
    mPlayerDemoActionFlag = new PlayerDemoActionFlag();
    mPlayerCapActionHistory =
        new PlayerCapActionHistory(this, mPlayerConst, mPlayerTrigger, mPlayerColliderHakoniwa);
    mPlayerAreaChecker = new PlayerAreaChecker(this, mPlayerModelHolder);
    mWaterSurfaceFinder = new al::WaterSurfaceFinder(this);
    mWorldEndBorderKeeper = new WorldEndBorderKeeper(this);
    mPlayerWallActionHistory = new PlayerWallActionHistory();
    mPlayerCounterAfterUpperPunch = new PlayerCounterAfterUpperPunch();
    mPlayerCounterForceRun = new PlayerCounterForceRun();
    mPlayerCounterQuickTurnJump = new PlayerCounterQuickTurnJump(mPlayerConst, mPlayerTrigger);
    mPlayerJumpMessageRequest = new PlayerJumpMessageRequest();
    mPlayerSandSinkAffect = new PlayerSandSinkAffect(this, mPlayerConst, mPlayerInput,
                                                     mPlayerColliderHakoniwa, mPlayerEffect);
    mPlayerEyeSensorHitHolder = new PlayerEyeSensorHitHolder(128);
    mPlayerEyeSensorHitHolder->createTargetMarkerBuffer(128);
    mPlayerPushReceiver = new PlayerPushReceiver(this);
    mPlayerHitPush = new PlayerHitPush(this, mPlayerConst);
    mPlayerExternalVelocity =
        new PlayerExternalVelocity(this, mPlayerColliderHakoniwa, al::getTransPtr(this));

    al::LiveActor* currentActor = mPlayerModelHolder->currentModel->mLiveActor;
    al::FootPrintHolder* footPrintHolder =
        new al::FootPrintHolder(currentActor ? currentActor : nullptr, "FootPrint", mBodyHitSensor,
                                (al::FootPrintServer*)al::getSceneObj(currentActor, 15));
    footPrintHolder->size[9] = nullptr;
    footPrintHolder->size[10] = nullptr;
    mFootPrintHolder = footPrintHolder;

    mPlayerSeparateCapFlag = new PlayerSeparateCapFlag();

    mPlayerWetControl = new PlayerWetControl(this, mPlayerModelHolder->findModelActor("Normal"),
                                             mPlayerAreaChecker);
    mPlayerWetControl->mEffect = mPlayerEffect;

    IUsePlayerCollision* playerCollision = getPlayerCollision();
    mHackCapJudgePreInputSeparateThrow =
        new HackCapJudgePreInputSeparateThrow(mPlayerInput, mPlayerSeparateCapFlag);
    mHackCapJudgePreInputSeparateJump =
        new HackCapJudgePreInputSeparateJump(mPlayerInput, mPlayerSeparateCapFlag);

    const char* capName = rs::getInitCapTypeName(playerInfo);
    mHackCap =
        new HackCap(this, capName, mPlayerInput, mPlayerAreaChecker, mPlayerWallActionHistory,
                    mPlayerCapActionHistory, mPlayerEyeSensorHitHolder, mPlayerSeparateCapFlag,
                    mPlayerColliderHakoniwa, mPlayerColliderHakoniwa, mPlayerWetControl,
                    mPlayerJointControlKeeper, mHackCapJudgePreInputSeparateThrow,
                    mHackCapJudgePreInputSeparateJump);
    mHackCap->init(actorInfo);
    al::registerExecutorFunctor(
        "帽子装着位置更新", actorInfo.getExecuteDirector(),
        al::FunctorV0M<PlayerActorHakoniwa*, void (PlayerActorHakoniwa::*)(void)>(
            this, &PlayerActorHakoniwa::executeAfterCapTarget));
    PlayerCounterAfterCapCatch* counterAfterCapCatch =
        mPlayerCapActionHistory->mCounterAfterCapCatch;

    mPlayerRippleGenerator = new PlayerRippleGenerator(this, mHackCap, mPlayerModelHolder);
    mPlayerRecoverySafetyPoint = new PlayerRecoverySafetyPoint(
        this, mHackCap, actorInfo, this, mPlayerColliderHakoniwa->mFilter2DOnly, mBodyHitSensor);
    mPlayerPuppet = new PlayerPuppet(
        this, mHackCap, mPlayerAnimator, mPlayerColliderHakoniwa, mActorDimensionKeeper,
        mPlayerModelChangerHakoniwa, mWorldEndBorderKeeper, mPlayerCounterForceRun,
        mPlayerDamageKeeper, mPlayerEffect, mPlayerInput, mPlayerConst);
    mPlayerBindKeeper = new PlayerBindKeeper(mBodyHitSensor, mPlayerPuppet);
    mPlayerCarryKeeper = new PlayerCarryKeeper(
        this, mBodyHitSensor, mPlayerAnimator, mPlayerModelChangerHakoniwa, mPlayerColliderHakoniwa,
        (PlayerJointParamHandLegAngle*)mPlayerJointControlKeeper->gap[12]);
    mPlayerEquipmentUser =
        new PlayerEquipmentUser(mPlayerInput, mPlayerModelHolder, mPlayerModelChangerHakoniwa);
    mPlayerSeCtrl =
        new PlayerSeCtrl(this, mPlayerAnimator, mHackCap, mPlayerModelChangerHakoniwa,
                         mPlayerModelHolder->currentModel->mLiveActor, mPlayerExternalVelocity);

    mPlayerHackKeeper = new PlayerHackKeeper(this, mHackCap, mPlayerRecoverySafetyPoint,
                                             mPlayerInput, getViewMtx(), mPlayerDamageKeeper,
                                             mPlayerModelChangerHakoniwa, mPlayerColliderHakoniwa);
    mPlayerHackKeeper->createHackModel(actorInfo);

    mPlayerFormSensorCollisionArranger = new PlayerFormSensorCollisionArranger(
        this, mPlayerColliderHakoniwa, mPlayerModelChangerHakoniwa, mPlayerHackKeeper);
    mPlayerCapManHeroEyesControl = new PlayerCapManHeroEyesControl(
        "キャップの目の制御",
        al::tryGetSubActor(al::getSubActor(mPlayerModelHolder->findModelActor("Normal"), "頭"),
                           "キャップの目"),
        al::getSubActor(mPlayerModelHolder->findModelActor("Normal2D"), "キャップの目[2D]"));

    mPlayerJudgeAirForceCount =
        new PlayerJudgeAirForceCount(this, mPlayerExternalVelocity, playerCollision);
    mPlayerJudgeCapCatchPop =
        new PlayerJudgeCapCatchPop(this, mPlayerInput, playerCollision, mPlayerModelChangerHakoniwa,
                                   mPlayerCapActionHistory, counterAfterCapCatch);
    mPlayerJudgeDeadWipeStart = new PlayerJudgeDeadWipeStart(this, mPlayerConst);
    mPlayerJudgeDirectRolling = new PlayerJudgeDirectRolling(
        this, mPlayerConst, mPlayerInput, playerCollision, mPlayerModelChangerHakoniwa,
        mPlayerCarryKeeper, mPlayerCounterForceRun);
    mPlayerJudgeEnableStandUp =
        new PlayerJudgeEnableStandUp(playerCollision, mPlayerColliderHakoniwa);
    mPlayerJudgeForceSlopeSlide =
        new PlayerJudgeForceSlopeSlide(this, mPlayerConst, playerCollision);
    mPlayerJudgeForceRolling = new PlayerJudgeForceRolling(this, playerCollision);
    mPlayerJudgeGrabCeil =
        new PlayerJudgeGrabCeil(this, mPlayerConst, playerCollision, mPlayerModelChangerHakoniwa,
                                mPlayerCarryKeeper, mPlayerExternalVelocity);
    mPlayerJudgeInWater1 = new PlayerJudgeInWater(
        this, mPlayerConst, playerCollision, mPlayerAreaChecker, mWaterSurfaceFinder,
        mPlayerColliderHakoniwa, mPlayerCounterForceRun, false, false, false);
    mPlayerJudgeInWater2 = new PlayerJudgeInWater(
        this, mPlayerConst, playerCollision, mPlayerAreaChecker, mWaterSurfaceFinder,
        mPlayerColliderHakoniwa, mPlayerCounterForceRun, false, false, true);
    mPlayerJudgeInWater3 = new PlayerJudgeInWater(
        this, mPlayerConst, playerCollision, mPlayerAreaChecker, mWaterSurfaceFinder,
        mPlayerColliderHakoniwa, nullptr, false, false, false);
    mPlayerJudgeInWater4 = new PlayerJudgeInWater(
        this, mPlayerConst, playerCollision, mPlayerAreaChecker, mWaterSurfaceFinder,
        mPlayerColliderHakoniwa, mPlayerCounterForceRun, false, true, false);
    mPlayerJudgeInvalidateInputFall =
        new PlayerJudgeInvalidateInputFall(this, mPlayerAreaChecker, playerCollision);
    mPlayerJudgeLongFall =
        new PlayerJudgeLongFall(this, mPlayerConst, mPlayerColliderHakoniwa, mPlayerHackKeeper,
                                mPlayerBindKeeper, mPlayerModelChangerHakoniwa);
    mPlayerJudgePoleClimb = new PlayerJudgePoleClimb(
        this, mPlayerConst, playerCollision, mPlayerModelChangerHakoniwa, mPlayerCarryKeeper,
        mPlayerExternalVelocity, mPlayerInput, mPlayerTrigger);
    mPlayerJudgePreInputCapThrow =
        new PlayerJudgePreInputCapThrow(mPlayerConst, mPlayerInput, mPlayerCarryKeeper, mHackCap);
    mPlayerJudgePreInputHackAction = new PlayerJudgePreInputHackAction(mPlayerConst, mPlayerInput);
    mHackCapJudgePreInputHoveringJump =
        new HackCapJudgePreInputHoveringJump(this, playerCollision, mHackCap, mPlayerInput);
    mPlayerJudgeSandSink = new PlayerJudgeSandSink(playerCollision, mPlayerSandSinkAffect);
    mPlayerJudgeSlopeSlide = new PlayerJudgeSlopeSlide(
        this, mPlayerConst, mPlayerInput, playerCollision, mPlayerModelChangerHakoniwa);
    mPlayerJudgeStartHipDrop = new PlayerJudgeStartHipDrop(
        mPlayerConst, mPlayerInput, mPlayerColliderHakoniwa, mPlayerModelChangerHakoniwa);
    mPlayerJudgeStartRise =
        new PlayerJudgeStartRise(this, mPlayerAreaChecker, mPlayerModelChangerHakoniwa);
    mPlayerJudgeStartRolling =
        new PlayerJudgeStartRolling(this, mPlayerConst, mPlayerInput, playerCollision,
                                    mPlayerModelChangerHakoniwa, mPlayerCarryKeeper);
    mPlayerJudgeStartSquat =
        new PlayerJudgeStartSquat(mPlayerInput, mPlayerCounterForceRun, mPlayerCarryKeeper);
    mPlayerJudgeWallCatch = new PlayerJudgeWallCatch(
        this, mPlayerConst, playerCollision, mPlayerModelChangerHakoniwa, mPlayerCarryKeeper,
        mPlayerExternalVelocity, mPlayerInput, mPlayerTrigger, mPlayerCounterForceRun);
    mPlayerJudgeWallCatchInputDir = new PlayerJudgeWallCatchInputDir(
        this, mPlayerConst, mPlayerInput, playerCollision, mPlayerModelChangerHakoniwa,
        mPlayerCarryKeeper, mPlayerExternalVelocity, mPlayerCounterForceRun,
        mPlayerColliderHakoniwa);
    mPlayerJudgeWallHitDown =
        new PlayerJudgeWallHitDown(this, playerCollision, mPlayerConst, mPlayerTrigger);
    mPlayerJudgeWallHitDownForceRun = new PlayerJudgeWallHitDownForceRun(
        this, playerCollision, mPlayerConst, mPlayerCounterForceRun, mPlayerTrigger);
    mPlayerJudgeWallHitDownRolling =
        new PlayerJudgeWallHitDownRolling(this, playerCollision, mPlayerConst, mPlayerTrigger);
    mPlayerJudgeWallKeep = new PlayerJudgeWallKeep(
        this, mPlayerConst, mPlayerInput, playerCollision, mPlayerModelChangerHakoniwa,
        mPlayerColliderHakoniwa, mPlayerCounterAfterUpperPunch, mPlayerWallActionHistory,
        mPlayerCarryKeeper, mPlayerTrigger, mPlayerCounterForceRun);
    mPlayerJudgeCameraSubjective =
        new PlayerJudgeCameraSubjective(this, playerCollision, mPlayerInput);
    mPlayerSpinCapAttack =
        new PlayerSpinCapAttack(mHackCap, mPlayerConst, mPlayerTrigger, mPlayerInput,
                                counterAfterCapCatch, mPlayerJudgePreInputCapThrow);
    mPlayerActionDiveInWater = new PlayerActionDiveInWater(
        mPlayerAnimator, mPlayerModelChangerHakoniwa, mPlayerCarryKeeper,
        new PlayerJudgeDiveInWater(this, mPlayerConst, mPlayerColliderHakoniwa,
                                   mPlayerColliderHakoniwa));
    mPlayerJudgeForceLand = new PlayerJudgeForceLand(mPlayerJudgeLongFall, mPlayerTrigger);
    mPlayerJudgeOutInWater =
        new PlayerJudgeOutInWater(this, mPlayerJudgeInWater1, mPlayerJudgeInWater4);
    mPlayerJudgePreInputJump =
        new PlayerJudgePreInputJump(mPlayerConst, mPlayerInput, mPlayerJudgeForceSlopeSlide);
    mPlayerJudgeStartWaterSurfaceRun =
        new PlayerJudgeStartWaterSurfaceRun(this, mWaterSurfaceFinder, mPlayerCounterForceRun);
    mPlayerJudgeSpeedCheckFall = new PlayerJudgeSpeedCheckFall(this, playerCollision, mPlayerConst,
                                                               mPlayerJudgeStartWaterSurfaceRun);
    mPlayerJudgeStartRun =
        new PlayerJudgeStartRun(this, mPlayerConst, playerCollision, mPlayerInput,
                                mPlayerCounterForceRun, mPlayerJudgeForceLand);

    al::initNerve(this, &Fall, 29);

    mPlayerStateWait = new PlayerStateWait(
        this, mPlayerConst, playerCollision, mPlayerModelChangerHakoniwa, mPlayerJointControlKeeper,
        mWaterSurfaceFinder, mPlayerColliderHakoniwa, mPlayerJudgeForceLand, mPlayerAnimator,
        mPlayerTrigger, mPlayerCapManHeroEyesControl,
        (PlayerJointParamCenterDynamics*)mPlayerJointControlKeeper->gap[13],
        (PlayerJointParamGroundPose*)mPlayerJointControlKeeper->gap[15]);
    mPlayerStateSandSink = new PlayerStateSandSink(this, mPlayerConst, mPlayerInput, mPlayerTrigger,
                                                   mPlayerColliderHakoniwa, mPlayerAnimator,
                                                   mPlayerJudgeStartRun, mPlayerJudgePreInputJump);
    mActorStateSandGeyser = new ActorStateSandGeyser(this);
    mPlayerStateSpinCap = new PlayerStateSpinCap(
        this, mPlayerConst, mPlayerInput, mPlayerCounterForceRun, mWaterSurfaceFinder,
        mPlayerColliderHakoniwa, mPlayerTrigger, mPlayerSpinCapAttack, mPlayerAnimator,
        (PlayerJointParamCapThrow*)mPlayerJointControlKeeper->gap[10]);
    mPlayerStateSquat =
        new PlayerStateSquat(this, mPlayerConst, mPlayerAnimator, mPlayerInput, playerCollision,
                             mPlayerModelChangerHakoniwa, mPlayerTrigger, mPlayerJudgeStartSquat,
                             mPlayerJudgeStartRun, mPlayerJudgeEnableStandUp);
    mPlayerStateRunHakoniwa2D3D = new PlayerStateRunHakoniwa2D3D(
        this, mPlayerConst, this, mPlayerInput, playerCollision, mWaterSurfaceFinder,
        mPlayerCounterForceRun, mPlayerCounterQuickTurnJump, mPlayerTrigger, mPlayerAnimator,
        mPlayerEffect, (PlayerJointParamCenterDynamics*)mPlayerJointControlKeeper->gap[13], isMoon);
    mPlayerStateSlope =
        new PlayerStateSlope(this, mPlayerConst, mPlayerInput, playerCollision, mPlayerAnimator);
    mPlayerStateRolling = new PlayerStateRolling(
        this, mPlayerConst, mPlayerInput, playerCollision, mPlayerTrigger, mPlayerAnimator,
        mPlayerEffect, mPlayerJudgeStartRolling, mPlayerJudgeEnableStandUp,
        mPlayerJudgePreInputJump, mPlayerJudgePreInputCapThrow, mPlayerJudgeForceRolling,
        mPlayerContinuousLongJump, mPlayerSeCtrl);
    mPlayerStateJump = new PlayerStateJump(
        this, mPlayerConst, mPlayerModelChangerHakoniwa, this, mPlayerInput,
        mPlayerJumpMessageRequest, new PlayerJudgeCapCatch(this, counterAfterCapCatch),
        mPlayerCounterForceRun, mPlayerJointControlKeeper, mPlayerColliderHakoniwa, mPlayerTrigger,
        mPlayerContinuousJump, mPlayerAnimator, mPlayerActionDiveInWater, mHackCap,
        new HackCapJudgeHoldHoveringJump(this, mPlayerColliderHakoniwa, mHackCap, mPlayerInput,
                                         mPlayerConst),
        isMoon);
    mPlayerStateCapCatchPop = new PlayerStateCapCatchPop(this, mPlayerConst, playerCollision,
                                                         mPlayerInput, mPlayerAnimator, mHackCap);
    mPlayerStateWallAir = new PlayerStateWallAir(
        this, mPlayerConst, mPlayerInput, mPlayerTrigger, mPlayerColliderHakoniwa,
        mPlayerJudgeWallKeep, mPlayerJudgePreInputJump, mPlayerAnimator, mPlayerWallActionHistory,
        mPlayerActionDiveInWater);
    mPlayerStateWallCatch = new PlayerStateWallCatch(
        this, mPlayerConst, mPlayerInput, getPlayerCollision(), mPlayerColliderHakoniwa,
        mPlayerModelHolder, mPlayerAreaChecker, mPlayerAnimator, mPlayerTrigger,
        mPlayerJudgePreInputJump, (PlayerJointParamGrab*)mPlayerJointControlKeeper->gap[11]);
    mPlayerStateGrabCeil = new PlayerStateGrabCeil(
        this, mPlayerConst, mPlayerInput, mPlayerModelHolder, mPlayerTrigger, getPlayerCollision(),
        mPlayerAnimator, mPlayerJudgePreInputJump, mBodyHitSensor,
        (PlayerJointParamGrab*)mPlayerJointControlKeeper->gap[11]);
    mPlayerStatePoleClimb = new PlayerStatePoleClimb(
        this, mPlayerConst, mPlayerInput, mPlayerTrigger, mPlayerModelHolder, getPlayerCollision(),
        mPlayerAnimator, mPlayerWallActionHistory,
        (PlayerJointParamHandLegAngle*)mPlayerJointControlKeeper->gap[12], mPlayerJudgePreInputJump,
        mPlayerActionDiveInWater);
    mPlayerStateHipDrop = new PlayerStateHipDrop(
        this, mPlayerConst, mPlayerInput, mPlayerColliderHakoniwa, mPlayerAnimator, mPlayerTrigger);
    mPlayerStateHeadSliding =
        new PlayerStateHeadSliding(this, mPlayerConst, playerCollision, mPlayerInput,
                                   mPlayerActionDiveInWater, mPlayerAnimator);
    mPlayerStateLongJump = new PlayerStateLongJump(
        this, mPlayerConst, mPlayerInput, playerCollision, mPlayerTrigger, mPlayerAnimator,
        mPlayerContinuousLongJump, mPlayerActionDiveInWater);
    mPlayerStateFallHakoniwa =
        new PlayerStateFallHakoniwa(this, mPlayerConst, playerCollision, mPlayerInput,
                                    mPlayerTrigger, mPlayerAreaChecker, this, mPlayerAnimator);
    mPlayerStateRise =
        new PlayerStateRise(this, mPlayerConst, playerCollision, mPlayerInput, mPlayerAnimator);
    mPlayerStateSwim = new PlayerStateSwim(
        this, mPlayerConst, playerCollision, mPlayerInput, mPlayerTrigger, mPlayerCarryKeeper,
        mPlayerModelHolder, mPlayerExternalVelocity, mPlayerAnimator, mPlayerSpinCapAttack,
        mWaterSurfaceFinder, mPlayerEffect,
        (PlayerJointParamSwim*)mPlayerJointControlKeeper->gap[14]);
    mPlayerStateDamageLife =
        new PlayerStateDamageLife(this, mPlayerConst, playerCollision, mPlayerInput,
                                  mPlayerColliderHakoniwa, mPlayerAnimator, mPlayerTrigger);
    mPlayerStateDamageSwim = new PlayerStateDamageSwim(
        this, mPlayerConst, playerCollision, mPlayerInput, mPlayerTrigger, mPlayerAnimator,
        mWaterSurfaceFinder, mPlayerEffect, mPlayerJudgeInWater1, mPlayerJudgeOutInWater);
    mPlayerStateDamageFire =
        new PlayerStateDamageFire(this, mPlayerConst, playerCollision, mPlayerInput,
                                  mPlayerAnimator, mPlayerJudgeSpeedCheckFall);
    mPlayerStatePress = new PlayerStatePress(this, playerCollision, mPlayerAnimator);
    mPlayerStateBind = new PlayerStateBind(this, mPlayerConst, mPlayerInput, mPlayerPuppet,
                                           playerCollision, mPlayerTrigger);
    mPlayerStateHack = new PlayerStateHack(this, mPlayerHackKeeper, mPlayerModelChangerHakoniwa,
                                           mPlayerAnimator, mHackCap);
    mPlayerStateEndHack = new PlayerStateEndHack(this, mPlayerConst, playerCollision, mPlayerInput,
                                                 mPlayerTrigger, mPlayerAnimator);
    mPlayerStateCameraSubjective = new PlayerStateCameraSubjective(
        this, *((al::CameraTicket**)mPlayerJudgeCameraSubjective + 4), mPlayerModelChangerHakoniwa,
        mPlayerCarryKeeper, mPlayerAnimator, mPlayerConst, playerCollision, mPlayerInput);
    mPlayerStateAbyss = new PlayerStateAbyss(this, mPlayerConst, mPlayerRecoverySafetyPoint,
                                             mPlayerColliderHakoniwa, mPlayerAnimator,
                                             mPlayerModelHolder->findModelActor("Normal2D"));

    al::initNerveState(this, mPlayerStateWait, &Wait, "待機");
    al::initNerveState(this, mPlayerStateWait, &Wait, "待機");
    al::initNerveState(this, mPlayerStateSquat, &Squat, "しゃがみ");
    al::initNerveState(this, mPlayerStateRunHakoniwa2D3D, &Run, "走り");
    al::initNerveState(this, mPlayerStateSlope, &Slope, "坂すべり");
    al::initNerveState(this, mPlayerStateRolling, &Rolling, "ローリング");
    al::initNerveState(this, mPlayerStateSpinCap, &SpinCap, "帽子スピン");
    al::initNerveState(this, mPlayerStateJump, &Jump, "ジャンプ");
    al::initNerveState(this, mPlayerStateCapCatchPop, &CapCatchPop, "帽子キャッチ浮きあがり");
    al::initNerveState(this, mPlayerStateWallAir, &WallAir, "壁接触[空中]");
    al::initNerveState(this, mPlayerStateWallCatch, &WallCatch, "壁つかまり");
    al::initNerveState(this, mPlayerStateGrabCeil, &GrabCeil, "天井ぶら下がり");
    al::initNerveState(this, mPlayerStatePoleClimb, &PoleClimb, "ポールつかまり");
    al::initNerveState(this, mPlayerStateHipDrop, &HipDrop, "ヒップドロップ");
    al::initNerveState(this, mPlayerStateHeadSliding, &HeadSliding, "ヘッドスライディング");
    al::initNerveState(this, mPlayerStateLongJump, &LongJump, "幅跳び");
    al::initNerveState(this, mPlayerStateFallHakoniwa, &Fall, "落下");
    al::initNerveState(this, mPlayerStateSandSink, &SandSink, "砂沈み");
    al::initNerveState(this, mActorStateSandGeyser, &SandGeyser, "砂の間欠泉");
    al::initNerveState(this, mPlayerStateRise, &Rise, "浮き上がり");
    al::initNerveState(this, mPlayerStateSwim, &Swim, "泳ぎ");
    al::initNerveState(this, mPlayerStateDamageLife, &Damage, "ダメージ");
    al::initNerveState(this, mPlayerStateDamageSwim, &DamageSwim, "水中・水面ダメージ");
    al::initNerveState(this, mPlayerStateDamageFire, &DamageFire, "あちちダメージ");
    al::initNerveState(this, mPlayerStatePress, &Press, "圧死");
    al::initNerveState(this, mPlayerStateHack, &Hack, "憑依");
    al::initNerveState(this, mPlayerStateEndHack, &EndHack, "憑依解除");
    al::initNerveState(this, mPlayerStateBind, &Bind, "バインド");
    al::initNerveState(this, mPlayerStateCameraSubjective, &Camera, "主観カメラ");
    al::initNerveState(this, mPlayerStateAbyss, &Abyss, "奈落死");

    mPlayerEquipmentUser->setPlayerStateRolling(mPlayerStateRolling);

    mPlayerOxygen = new PlayerOxygen();
    mPlayerOxygen->setup(
        mPlayerConst->getOxygenNoReduceFrame(), mPlayerConst->getOxygenReduceFrame(),
        mPlayerConst->getOxygenRecoveryFrame(), mPlayerConst->getOxygenDamageInterval());
    mPlayerOxygen->reset();

    PlayerJudgeInWater* iceWaterJudge = new PlayerJudgeInWater(
        this, mPlayerConst, playerCollision, mPlayerAreaChecker, mWaterSurfaceFinder,
        mPlayerColliderHakoniwa, mPlayerCounterForceRun, true, false, false);
    mPlayerCounterIceWater = new PlayerCounterIceWater(this, actorInfo, mPlayerConst,
                                                       mPlayerColliderHakoniwa, iceWaterJudge);
    mPlayerStainControl =
        new PlayerStainControl(this, mPlayerModelHolder->findModelActor("Normal"),
                               mPlayerModelChangerHakoniwa, mHackCap, mPlayerEffect);
    mPlayerJudgeRecoveryLifeFast = new PlayerJudgeRecoveryLifeFast(
        this, mPlayerDamageKeeper, mPlayerBindKeeper, playerCollision, mPlayerInput,
        mPlayerHackKeeper, mPlayerStateWait, mPlayerStateSwim, iceWaterJudge);

    mPlayerInfo = new PlayerInfo();
    mPlayerInfo->mPlayerModelChangerHakoniwa = mPlayerModelChangerHakoniwa;
    mPlayerInfo->mPlayerOxygen = mPlayerOxygen;
    mPlayerInfo->mPlayerBindKeeper = mPlayerBindKeeper;
    mPlayerInfo->mPlayerDamageKeeper = mPlayerDamageKeeper;
    mPlayerInfo->mPlayerDemoActionFlag = mPlayerDemoActionFlag;
    mPlayerInfo->mPlayerEquipmentUser = mPlayerEquipmentUser;
    mPlayerInfo->mHackCap = mHackCap;
    mPlayerInfo->mWorldEndBorderKeeper = mWorldEndBorderKeeper;
    mPlayerInfo->mPlayerFormSensorCollisionArranger = mPlayerFormSensorCollisionArranger;
    mPlayerInfo->mPlayerAnimator = mPlayerAnimator;
    mPlayerInfo->mPlayerInput = mPlayerInput;
    mPlayerInfo->mPlayerModelHolder = mPlayerModelHolder;
    mPlayerInfo->mCeilingCheck = mPlayerColliderHakoniwa;
    mPlayerInfo->mPlayerHackKeeper = mPlayerHackKeeper;
    mPlayerInfo->mPlayerCapManHeroEyesControl = mPlayerCapManHeroEyesControl;
    mPlayerInfo->mPlayerCarryKeeper = mPlayerCarryKeeper;
    mPlayerInfo->mPlayerJointControlKeeper = mPlayerJointControlKeeper;
    mPlayerInfo->mPlayerCounterIceWater = mPlayerCounterIceWater;
    mPlayerInfo->mPlayerStainControl = mPlayerStainControl;
    mPlayerInfo->mFootPrintHolder = mFootPrintHolder;
    mPlayerInfo->mBodyHitSensor = mBodyHitSensor;
    mPlayerInfo->mPlayerRecoverySafetyPoint = mPlayerRecoverySafetyPoint;
    mPlayerInfo->mCostumeInfo = costumeInfo;
    mPlayerInfo->mPlayerJudgeCameraInWater =
        new PlayerJudgeCameraInWater(this, mPlayerJudgeInWater1, mWaterSurfaceFinder);
    mPlayerInfo->mPlayerJudgeActiveCameraSubjective =
        new PlayerJudgeActiveCameraSubjective(mPlayerStateCameraSubjective);
    mPlayerInfo->mPlayerJudgeTalkGround = new PlayerJudgeTalkGround(
        this, mPlayerModelChangerHakoniwa, mPlayerHackKeeper, mPlayerCarryKeeper, playerCollision,
        mPlayerInput, mPlayerConst, mPlayerStateWait);
    mPlayerInfo->mPlayerJudgeTalkSwim =
        new PlayerJudgeTalkSwim(mPlayerHackKeeper, mPlayerCarryKeeper, mPlayerStateSwim);
    mPlayerInfo->mPlayerJudgeDead = new PlayerJudgeDead(this);
    mPlayerInfo->mPlayerJudgeDeadWipeStart = mPlayerJudgeDeadWipeStart;
    mPlayerInfo->mPlayerJudgeDrawForward =
        new PlayerJudgeDrawForward(this, mPlayerJudgeDeadWipeStart);
    mPlayerInfo->mPlayerJudgeSameNervePoleClimb = new PlayerJudgeSameNerve(this, &PoleClimb);
    mPlayerInfo->mPlayerJudgeSameNerveGrabCeil = new PlayerJudgeSameNerve(this, &GrabCeil);
    mPlayerInfo->mPlayerJudgeSameNerveWallCatch = new PlayerJudgeSameNerve(this, &WallCatch);
    mPlayerInfo->mPlayerJudgeSameNerveSwim = new PlayerJudgeSameNerve(this, &Swim);
    mPlayerInfo->mPlayerJudgeFailureCameraSubjective = new PlayerJudgeFailureCameraSubjective(
        mPlayerInput, new PlayerJudgeSameNerve(this, &Camera));
    mPlayerInfo->mPlayerJudgeSafetyPointRecovery = new PlayerJudgeSafetyPointRecovery(
        new PlayerJudgeSameNerve(this, &Abyss), mPlayerStateAbyss);
    mPlayerInfo->mPlayerJudgeStatusPoleClimb = new PlayerJudgeStatusPoleClimb(
        new PlayerJudgeSameNerve(this, &PoleClimb), mPlayerStatePoleClimb);
    mPlayerInfo->mPlayerJudgePlaySwitchOnAreaWaitAnim =
        new PlayerJudgePlaySwitchOnAreaWaitAnim(mPlayerStateWait);
    mPlayerInfo->mPlayerJudgeSleep = new PlayerJudgeSleep(mPlayerStateWait);
    mPlayerInfo->mPlayerJudgeEnableGuideArrow = new PlayerJudgeEnableGuideArrow(mPlayerPuppet);
    mPlayerInfo->mPlayerJudgeEnablePeachAmiibo =
        new PlayerJudgeEnablePeachAmiibo(mPlayerStateDamageFire);
    mPlayerInfo->mIsMoon = isMoon;

    CapFunction::putOnCapPlayer(mHackCap, mPlayerAnimator);
    syncSensorAndCollision();
    makeActorAlive();
    rs::resetCollision(mPlayerColliderHakoniwa);
    mGaugeAir = new GaugeAir("酸素タイマー", *al::getLayoutInitInfo(actorInfo));
    mWaterSurfaceShadow = new WaterSurfaceShadow(actorInfo, "WaterSurfaceShadow");
    mWaterSurfaceShadow->_10 = {1500.0f, 0.25f, 0.75f};
    mWaterSurfaceShadow->setScale(0.5f);
}

u32 PlayerActorHakoniwa::getPortNo() const {
    if (rs::isSeparatePlay(this))
        return PlayerInput::getSeparatePlay1P();
    else
        return PlayerActorBase::getPortNo();
}

IUsePlayerCollision* PlayerActorHakoniwa::getPlayerCollision() const {
    return mPlayerColliderHakoniwa;
}

PlayerHackKeeper* PlayerActorHakoniwa::getPlayerHackKeeper() const {
    return mPlayerHackKeeper;
}
// bool isEnableDemo() override;
// void startDemo() override;
// void endDemo() override;
// void startDemoPuppetable() override;
// void endDemoPuppetable() override;
// void startDemoShineGet() override;
// void endDemoShineGet() override;
// void startDemoMainShineGet() override;
// void endDemoMainShineGet() override;
// void startDemoHack() override;
// void endDemoHack() override;
// void startDemoKeepBind() override;
// void noticeDemoKeepBindExecute() override;
// void endDemoKeepBind() override;
// void startDemoKeepCarry() override;
// void endDemoKeepCarry() override;
// void getDemoActor() override;
// void* getDemoAnimator() override;
// bool isDamageStopDemo() const override;
// void* getPlayerPuppet() override;

PlayerInfo* PlayerActorHakoniwa::getPlayerInfo() const {
    return mPlayerInfo;
}

// bool checkDeathArea() override;
// void sendCollisionMsg() override;
// bool receivePushMsg(const al::SensorMsg*, al::HitSensor*, al::HitSensor*, f32) override;

ActorDimensionKeeper* PlayerActorHakoniwa::getActorDimensionKeeper() const {
    return mActorDimensionKeeper;
}

// void updateModelShadowDropLength();
// void executeAfterCapTarget();
// void syncSensorAndCollision();
// void checkDamageFromCollision();
// void executePreMovementNerveChange();
// void updateCarry();

void sub_71004229D0(al::LiveActor* player, PlayerTrigger* trigger,
                    const IUsePlayerCollision* collision) {
    trigger->set(PlayerTrigger::EActionTrigger_val8);
    sead::Vector3f normal = {0.0f, 0.0f, 0.0f};
    rs::calcGroundNormalOrGravityDir(&normal, player, collision);
    sead::Vector3f slideDir = {0.0f, 0.0f, 0.0f};
    if (rs::calcSlideDir(&slideDir, al::getGravity(player), normal)) {
        sead::Quatf quat = sead::Quatf::unit;
        al::makeQuatFrontUp(&quat, slideDir, normal);
        al::updatePoseQuat(player, quat);
    }
    al::setNerve(player, &Rolling);
}

void PlayerActorHakoniwa::setNerveOnGround() {
    if (rs::updateJudgeAndResult(mPlayerJudgeStartWaterSurfaceRun)) {
        al::setNerve(this, &Run);
        return;
    } else if (rs::updateJudgeAndResult(mPlayerJudgeInWater1)) {
        al::setNerve(this, &Swim);
        return;
    } else if (mPlayerCounterForceRun->_0 < 1 &&
               !rs::isOnGroundRunAngle(this, mPlayerColliderHakoniwa, mPlayerConst)) {
        al::setNerve(this, &Fall);
        return;
    } else if (!rs::isOnGround(this, mPlayerColliderHakoniwa)) {
        al::setNerve(this, &Fall);
        return;
    } else if (rs::updateJudgeAndResult(mPlayerJudgeForceSlopeSlide)) {
        if (mPlayerCarryKeeper->isCarry())
            mPlayerCarryKeeper->startCancelAndRelease();
        al::setNerve(this, &Slope);
        return;
    } else if (rs::updateJudgeAndResult(mPlayerJudgeForceRolling)) {
        sub_71004229D0(this, mPlayerTrigger, mPlayerColliderHakoniwa);
        return;
    } else if (rs::updateJudgeAndResult(mPlayerJudgeSandSink)) {
        al::setNerve(this, &SandSink);
        return;
    } else if (rs::updateJudgeAndResult(mPlayerJudgePoleClimb)) {
        mPlayerStatePoleClimb->setup(
            mPlayerJudgePoleClimb->mCollisionParts, mPlayerJudgePoleClimb->_58,
            mPlayerJudgePoleClimb->_64, mPlayerJudgePoleClimb->_70,
            mPlayerJudgePoleClimb->mPoleCodeAngleOffsetCeiling, mPlayerJudgePoleClimb->_7C,
            mPlayerJudgePoleClimb->mMaterialCodeCeiling);
        al::setNerve(this, &PoleClimb);
        return;
    } else if (rs::updateJudgeAndResult(mPlayerJudgeGrabCeil)) {
        mPlayerStateGrabCeil->setup(mPlayerJudgeGrabCeil->_40, mPlayerJudgeGrabCeil->_48,
                                    mPlayerJudgeGrabCeil->_54, mPlayerJudgeGrabCeil->_60);
        al::setNerve(this, &GrabCeil);
        return;
    } else if (!rs::updateJudgeAndResult(mPlayerJudgeEnableStandUp)) {
        if (mPlayerCarryKeeper->isCarry())
            mPlayerCarryKeeper->startCancelAndRelease();
        rs::cutVerticalVelocityGroundNormal(this, getPlayerCollision());
        al::setNerve(this, &Squat);
        return;
    } else if (rs::updateJudgeAndResult(mPlayerJudgePreInputJump)) {
        al::setNerve(this, &Jump);
        return;
    } else if (rs::updateJudgeAndResult(mPlayerJudgeStartRun)) {
        al::setNerve(this, &Run);
        return;
    } else if (!rs::updateJudgeAndResult(mPlayerJudgeStartSquat) ||
               rs::isJudge(mPlayerJudgeForceLand)) {
        al::setNerve(this, &Wait);
        return;
    } else if (!rs::isJustLand(mPlayerColliderHakoniwa) || al::isNerve(this, &LongJump)) {
        al::setNerve(this, &Squat);
        return;
    } else {
        if (mPlayerCarryKeeper->isCarry())
            mPlayerCarryKeeper->startCancelAndRelease();
        al::setNerve(this, &Slope);
        return;
    }
}

// void startPlayerPuppet();
// void cancelHackPlayerPuppetDemo();
// void endPlayerPuppet();
// void exeWait();
// bool tryActionCapReturn();
// bool tryActionCapSpinAttack();
// void exeSquat();
// bool tryActionSeparateCapThrow();
// void exeRun();
// void exeSlope();
// void exeRolling();
// void exeSpinCap();

bool PlayerActorHakoniwa::tryChangeNerveFromAir() {
    if(rs::updateJudgeAndResult(mPlayerJudgeForceSlopeSlide)) {
        if (mPlayerCarryKeeper->isCarry())
            mPlayerCarryKeeper->startCancelAndRelease();
        al::setNerve(this, &Slope);
        return true;
    }
    if(rs::updateJudgeAndResult(mPlayerJudgeInvalidateInputFall)) {
        mPlayerInput->convergedInUpdateToDisableInput = 1;
        mPlayerInput->mIsDisableInput = true;
        mPlayerInput->usedInUpdate = false;
        al::setNerve(this, &Fall);
        return true;
    }
    if(!al::isNerve(this, &SpinCap) && tryActionCapSpinAttackImpl(true)) {
        al::setNerve(this, &SpinCap);
        return true;
    }
    if(!al::isNerve(this, &LongJump) && rs::updateJudgeAndResult(mPlayerJudgeStartHipDrop)) {
        al::setNerve(this, &HipDrop);
        return true;
    }
    if(rs::updateJudgeAndResult(mPlayerJudgePoleClimb)) {
        mPlayerStatePoleClimb->setup(
            mPlayerJudgePoleClimb->mCollisionParts, mPlayerJudgePoleClimb->_58,
            mPlayerJudgePoleClimb->_64, mPlayerJudgePoleClimb->_70,
            mPlayerJudgePoleClimb->mPoleCodeAngleOffsetCeiling, mPlayerJudgePoleClimb->_7C,
            mPlayerJudgePoleClimb->mMaterialCodeCeiling);
        al::setNerve(this, &PoleClimb);
        return true;
    }
    if((!al::isNerve(this, &GrabCeil) || mPlayerStateGrabCeil->isEnableNextGrabCeil()) && rs::updateJudgeAndResult(mPlayerJudgeGrabCeil)) {
        mPlayerStateGrabCeil->setup(mPlayerJudgeGrabCeil->_40, mPlayerJudgeGrabCeil->_48,
                                    mPlayerJudgeGrabCeil->_54, mPlayerJudgeGrabCeil->_60);
        al::setNerve(this, &GrabCeil);
        return true;
    }
    if(al::isNerve(this, &LongJump) && rs::updateJudgeAndResult(mPlayerJudgeWallHitDown)) {
        mPlayerTrigger->set(PlayerTrigger::EActionTrigger_val10);
        if(mPlayerEquipmentUser->getEquipmentSensor() && mPlayerCounterForceRun->_0 >= 1)
            mPlayerEquipmentUser->cancelEquip();
        al::setNerve(this, &Damage);
        return true;
    }
    if(rs::updateJudgeAndResult(mPlayerJudgeWallCatch)) {
        mPlayerStateWallCatch->setup(mPlayerJudgeWallCatch->mCollidedWallPart, mPlayerJudgeWallCatch->_60,
                                    -mPlayerJudgeWallCatch->_6C, mPlayerJudgeWallCatch->_78);
        al::setNerve(this, &WallCatch);
        return true;
    }
    if(!al::isNerve(this, &WallAir) && rs::updateJudgeAndResult(mPlayerJudgeWallKeep)) {
        al::setNerve(this, &WallAir);
        return true;
    }
    if(rs::updateJudgeAndResult(mPlayerJudgeCapCatchPop)) {
        al::setNerve(this, &CapCatchPop);
        return true;
    }
    if(rs::updateJudgeAndResult(mPlayerJudgeStartWaterSurfaceRun)) {
        al::setNerve(this, &Run);
        return true;
    }
    if(rs::updateJudgeAndResult(mPlayerJudgeInWater1)) {
        if(mPlayerActionDiveInWater->isDiveInWaterAnim())
            mPlayerTrigger->set(PlayerTrigger::EActionTrigger_val9);
        al::setNerve(this, &Swim);
        return true;
    }
    return false;
}

// bool tryActionCapSpinAttackMiss();
// void exeJump();
// void exeCapCatchPop();
// void exeWallAir();
// void exeWallCatch();
// void exeGrabCeil();
// void exePoleClimb();
// void exeHipDrop();
// void exeHeadSliding();
// void exeLongJump();
// void exeFall();
// void exeSandSink();
// void exeSandGeyser();
// void exeRise();
// void exeSwim();
// void exeDamage();
// void exeDamageSwim();
// void exeDamageFire();
// void exePress();
// void exeHack();
// void exeEndHack();
// void exeBind();
// bool tryActionCapSpinAttackBindEnd();
// void exeDemo();
// void exeCamera();
// void exeAbyss();
// void exeDead();
// bool tryActionCapSpinAttackImpl(bool);
