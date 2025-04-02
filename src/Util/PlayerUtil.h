#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/Base/StringUtil.h"

namespace al {
class GamePadSystem;
class HitSensor;
class IUseMessageSystem;
class LiveActor;
struct SklAnimRetargettingInfo;
}  // namespace al

class CapTargetInfo;
class PlayerActorBase;
class PlayerInitInfo;

namespace rs {

void initPlayerActorInfo(PlayerActorBase*, const PlayerInitInfo&);
const char* getInitPlayerModelName(const PlayerInitInfo&);
bool isNeedCreateNoseNeedle(const PlayerInitInfo&);
bool isClosetScenePlayer(const PlayerInitInfo&);
al::GamePadSystem* getGamePadSystem(const PlayerInitInfo&);
const char* getInitCapTypeName(const PlayerInitInfo&);
al::SklAnimRetargettingInfo* createPlayerSklRetargettingInfo(al::LiveActor*, const sead::Vector3f&);
const sead::Vector3f& getPlayerPos(const al::LiveActor*);
const sead::Vector3f& getPlayerHeadPos(const al::LiveActor*);
const sead::Vector3f& getPlayerBodyPos(const al::LiveActor*);
void calcPlayerSideDir(sead::Vector3f*, const al::LiveActor*);
void calcPlayerUpDir(sead::Vector3f*, const al::LiveActor*);
void calcPlayerFrontDir(sead::Vector3f*, const al::LiveActor*);
void calcPlayerGroundPoseUp(sead::Vector3f*, const al::LiveActor*);
bool isNearPlayerH(const al::LiveActor*, f32);
void calcPlayerFollowLayoutWorldPos(sead::Vector3f*, const al::LiveActor*);
bool isPlayerHack(const al::LiveActor*);
bool isPlayerHackGroupUseCameraStick(const al::LiveActor*);
bool tryCalcPlayerCeilingSpace(f32*, const al::LiveActor*, f32, f32);
bool tryCalcKidsGuideCeilingSpace(f32*, const al::LiveActor*, f32, f32);
const sead::Vector3f& getPlayerVelocity(const al::LiveActor*);
bool tryCalcPlayerModelHeadJointPos(sead::Vector3f*, const al::LiveActor*);
bool tryCalcPlayerModelHeadJointUp(sead::Vector3f*, const al::LiveActor*);
bool tryCalcPlayerModelHeadJointFront(sead::Vector3f*, const al::LiveActor*);
bool tryCalcPlayerModelHeadJointSide(sead::Vector3f*, const al::LiveActor*);
bool tryCalcPlayerModelNoseJointMtx(sead::Matrix34f*, const al::LiveActor*);
bool isPlayerDamageStopDemo(const al::LiveActor*);
bool isPlayerHackType(const al::LiveActor*, s32);
bool isPlayerHackRigidBody(const al::LiveActor*);
bool isPlayerHackJugemFishing(const al::LiveActor*);
bool isPlayerHackKuriboAny(const al::LiveActor*);
bool isPlayerHackKuribo(const al::LiveActor*);
bool isPlayerHackKuriboWing(const al::LiveActor*);
bool isPlayerHackStatueMario(const al::LiveActor*);
bool isPlayerHackEnemy(const al::LiveActor*);
bool isPlayerHackTrilemmaRock(const al::LiveActor*);
bool isPlayerHackTrilemmaPaper(const al::LiveActor*);
bool isPlayerHackTrilemmaScissors(const al::LiveActor*);
bool isPlayerHackElectricWire(const al::LiveActor*);
bool isPlayerHackTRex(const al::LiveActor*);
bool isPlayerHackFukankun(const al::LiveActor*);
bool isPlayerHackHosui(const al::LiveActor*);
bool isPlayerHackYoshi(const al::LiveActor*);
bool isPlayerHackYukimaru(const al::LiveActor*);
bool isPlayerHackHammerBros(const al::LiveActor*);
bool isPlayerHackBazookaElectric(const al::LiveActor*);
bool isPlayerHackBubble(const al::LiveActor*);
bool isPlayerHackTank(const al::LiveActor*);
bool isPlayerHackTsukkun(const al::LiveActor*);
bool isPlayerHackPukupuku(const al::LiveActor*);
bool isPlayerHackPukupukuAll(const al::LiveActor*);
bool isPlayerHackRadiconNpc(const al::LiveActor*);
bool isPlayerHackSenobi(const al::LiveActor*);
bool isPlayerHackKakku(const al::LiveActor*);
bool isPlayerHackGroupTalkScare(const al::LiveActor*);
bool isPlayerHackGroupUseCameraStick(const al::LiveActor*);
bool isPlayerHackNoSeparateCameraInput(const al::LiveActor*);
bool isPlayerEnableToSeeOddSpace(const al::LiveActor*);
bool isPlayerMini(const al::LiveActor*);
bool isPlayer3D(const al::LiveActor*);
bool isPlayer2D(const al::LiveActor*);
bool isPlayerSquat(const al::LiveActor*);
bool isPlayerInWater(const al::LiveActor*);
bool isPlayerPoleClimb(const al::LiveActor*);
bool isPlayerWallCatch(const al::LiveActor*);
bool isPlayerCameraSubjective(const al::LiveActor*);
bool isPlayerEnableTalkGround(const al::LiveActor*);
bool isPlayerEnableTalkSwim(const al::LiveActor*);
bool isPlayerBinding(const al::LiveActor*);
bool isPlayerEquipSomething(const al::LiveActor*);
bool isPlayerEquipRocketFlower(const al::LiveActor*);
bool isPlayerEquipNoSeparateAction(const al::LiveActor*);
bool isPlayerCarrySomething(const al::LiveActor*);
bool isPlayerNoInput(const al::LiveActor*);
bool isPlayerEnableShowTutorialInput(const al::LiveActor*);
bool isPlayerSafetyPointRecovery(const al::LiveActor*);
bool isPlayerPlayingSwitchOnAnim(const al::LiveActor*);
bool isPlayerEquipNoSeparateTutorial(const al::LiveActor*);
bool isPlayerWaitSleep(const al::LiveActor*);
bool isPlayerAboveSeparateCapGuideArrow(const al::LiveActor*);
bool isPlayerInvalidateGuideArrow(const al::LiveActor*);
bool isPlayerInvisibleCap(const al::LiveActor*);
bool isPlayerEnablePeachAmiibo(const al::LiveActor*);
bool isPlayerActiveMarioAmiiboInvincible(const al::LiveActor*);
bool isPlayerCollidedGround(const al::LiveActor*);
bool isPlayerCollidedCeiling(const al::LiveActor*);
bool isPlayerOnGround(const al::LiveActor*);
bool isPlayerOnActor(const al::LiveActor*);
bool isPlayerOnChairActor(const al::LiveActor*);
bool isPlayerOnBedActor(const al::LiveActor*);
bool isPlayerFaceToTarget(const al::LiveActor*, const sead::Vector3f&);
bool isPlayerFaceToTarget(const al::LiveActor*);
bool isPlayerFaceToTargetInRange(const al::LiveActor*, const sead::Vector3f&, f32);
bool isPlayerFaceToTargetInRange(const al::LiveActor*, f32);
bool tryGetFlyingCapPos(sead::Vector3f*, const al::LiveActor*);
bool tryGetFlyingCapVelocity(sead::Vector3f*, const al::LiveActor*);
bool isEnableReceiveCapStartLockOnAngle(const al::LiveActor*, const al::HitSensor*);
bool isGuardNosePainCap(const al::LiveActor*);
bool isEquipCapCatched(const al::LiveActor*);
f32 getFlyingCapThrowSpeedMax(const al::LiveActor*);
bool isPlayerCapSpinOrFlying(const al::LiveActor*);
bool isPlayerCapFlying(const al::LiveActor*);
bool isPlayerCapSeparateThrow(const al::LiveActor*);
bool isEmptyPlayerOxygen(const al::LiveActor*);
void recoveryPlayerOxygen(const al::LiveActor*);
void requestBindPlayer(const al::LiveActor*, al::HitSensor*);
void sendMsgBreakFloorToPlayer(const al::LiveActor*);
void requestStageStartHack(const al::LiveActor*, al::HitSensor*, const CapTargetInfo*,
                           al::LiveActor*);
bool isEnableOpenMap(const al::LiveActor*);
void syncPlayerModelAlpha(al::LiveActor*);
f32 getPlayerShadowDropLength(const al::LiveActor*);
bool isExistLabelInPlayerHackSystemMstxt(const al::LiveActor*, const al::IUseMessageSystem*,
                                         const char*, const char*);
const char16* getPlayerHackSystemMessageString(const al::LiveActor*, const al::IUseMessageSystem*,
                                               const char*, const char*);
void getPlayerHackTutorialMoviePath(al::StringTmp<64>*, const al::LiveActor*, const char*);
void calcPlayerAmiiboPeachAppearBasePos(sead::Vector3f*, const al::LiveActor*);
bool isPlayerInputHoldBalloonSet(const al::LiveActor*);
bool isPlayerInputHoldSquat(const al::LiveActor*);
bool isPlayerInputHoldCapKeepLockOn(const al::LiveActor*);
bool isPlayerInputHoldCarryAction(const al::LiveActor*);
bool isPlayerInputHoldEnterUpperDokan2D(const al::LiveActor*);
bool isPlayerInputHoldEnterSideDokan2D(const al::LiveActor*, const sead::Vector3f&);
bool isPlayerInputTriggerStartTalk(const al::LiveActor*);
bool isPlayerInputTriggerRide(const al::LiveActor*);
bool isPlayerInputTriggerSeparateCapJangoHelp(const al::LiveActor*);
bool tryGetPlayerInputWallAlongWallNormal(sead::Vector3f*, const al::LiveActor*);
void calcPlayerHoldPos(sead::Vector3f*, const al::HitSensor*);
void calcPlayerHoldMtx(sead::Matrix34f*, const al::HitSensor*);
void setPlayerHoldJointAngle(const al::HitSensor*, const sead::Vector3f&);
void updatePosePlayerHold(al::LiveActor*, const al::HitSensor*);
void updatePosePlayerHoldAddOffset(al::LiveActor*, const al::HitSensor*, const sead::Vector3f&);
f32 calcPlayerGuideOffsetY(const al::LiveActor*);
bool isEnablePlayerHeadGuide(const al::LiveActor*);
void calcPlayerGuidePos(sead::Vector3f*, const al::LiveActor*);
void calcPlayerGuidePos(sead::Vector3f*, const al::LiveActor*, f32);

}  // namespace rs
