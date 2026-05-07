#include "Util/PlayerUtil.h"

#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>
#include <nn/g3d/ResFile.h>
#include <nn/g3d/ResModel.h>
#include <prim/seadBitUtil.h>
#include <prim/seadSafeString.h>

#include "Library/Base/StringUtil.h"
#include "Library/Collision/CollisionParts.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActorFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Matrix/MatrixUtil.h"
#include "Library/Message/MessageHolder.h"
#include "Library/Resource/Resource.h"
#include "Library/Resource/ResourceFunction.h"
#include "Library/Shadow/ActorShadowUtil.h"
#include "Library/Yaml/ByamlIter.h"

#include "Enemy/Megane.h"
#include "Player/EquipmentInfo.h"
#include "Player/HackCap.h"
#include "Player/IPlayerModelChanger.h"
#include "Player/IUsePlayerCeilingCheck.h"
#include "Player/IUsePlayerCollision.h"
#include "Player/PlayerActorBase.h"
#include "Player/PlayerAnimator.h"
#include "Player/PlayerBindKeeper.h"
#include "Player/PlayerCarryKeeper.h"
#include "Player/PlayerCostumeInfo.h"
#include "Player/PlayerDamageKeeper.h"
#include "Player/PlayerEquipmentUser.h"
#include "Player/PlayerFormSensorCollisionArranger.h"
#include "Player/PlayerFunction.h"
#include "Player/PlayerHackKeeper.h"
#include "Player/PlayerInfo.h"
#include "Player/PlayerInitInfo.h"
#include "Player/PlayerInput.h"
#include "Player/PlayerJointControlKeeper.h"
#include "Player/PlayerModelChangerHakoniwa.h"
#include "Player/PlayerModelHolder.h"
#include "Player/PlayerOxygen.h"
#include "Player/PlayerStateRolling.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolder.h"
#include "System/GameDataHolderAccessor.h"
#include "Util/JudgeUtil.h"
#include "Util/PlayerCollisionUtil.h"
#include "Util/SensorMsgFunction.h"

class PlayerActorBase;

namespace al {
PlayerActorBase* getPlayerActor(const LiveActor*, s32);
}

namespace {

constexpr f32 cFaceTargetAngle = 60.0f;
constexpr f32 cCapLockOnDotLimit = -0.17365f;

const char* cHackTypeNames[] = {
    "",
    "RigidBodyBox",
    "JugemFishing",
    "Kuribo",
    "KuriboWing",
    "Statue",
    "",
    "",
    "",
    "ElectricWire",
    "TRex",
    "Fukankun",
    "Hosui",
    "Yoshi",
    "Yukimaru",
    "HammerBros",
    "BazookaElectric",
    "Bubble",
    "Tank",
    "Tsukkun",
    "Pukupuku",
    "PukupukuSnow",
    "Radicon",
    "Senobi",
    "Kakku",
    "Megane",
};

PlayerActorBase* getPlayer(const al::LiveActor* actor) {
    return al::getPlayerActor(actor, 0);
}

PlayerInfo* getPlayerInfo(const al::LiveActor* actor) {
    return getPlayer(actor)->getPlayerInfo();
}

PlayerHackKeeper* getHackKeeper(const al::LiveActor* actor) {
    return getPlayer(actor)->getPlayerHackKeeper();
}

const char* getCurrentHackName(const al::LiveActor* actor) {
    PlayerHackKeeper* keeper = getHackKeeper(actor);
    return keeper ? keeper->getCurrentHackName() : nullptr;
}

bool isCurrentHack(const al::LiveActor* actor, const char* name) {
    const char* current = getCurrentHackName(actor);
    return current && al::isEqualString(name, current);
}

HackCap* getHackCap(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    return info ? info->getHackCap() : nullptr;
}

PlayerInput* getPlayerInput(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    return info ? info->getInput() : nullptr;
}

IUsePlayerCollision* getPlayerCollision(const al::LiveActor* actor) {
    return getPlayer(actor)->getPlayerCollision();
}

const al::CollisionParts* tryGetPlayerGroundCollisionParts(const al::LiveActor* actor) {
    PlayerHackKeeper* keeper = getHackKeeper(actor);
    if (keeper && keeper->getCurrentHackName()) {
        PlayerHackKeeper* hackKeeper = getHackKeeper(actor);
        if (!hackKeeper || !hackKeeper->getHackSensor())
            return nullptr;
        if (hackKeeper->isHackUsePlayerCollision()) {
            if (rs::isCollidedGround(hackKeeper->getPlayerCollision()))
                return rs::getCollidedGroundCollisionParts(hackKeeper->getPlayerCollision());
        } else {
            const al::LiveActor* hack = hackKeeper->getHack();
            if (al::isExistActorCollider(hack) && al::isCollidedGround(hack))
                return al::getCollidedGroundCollisionParts(hack);
        }
    } else {
        IUsePlayerCollision* collision = getPlayerCollision(actor);
        if (collision && rs::isCollidedGround(collision))
            return rs::getCollidedGroundCollisionParts(collision);
        if (!collision && al::isExistActorCollider(actor) && al::isCollidedGround(actor))
            return al::tryGetCollidedGroundCollisionParts(actor);
    }

    return nullptr;
}

}  // namespace

void rs::initPlayerActorInfo(PlayerActorBase* player, const PlayerInitInfo& initInfo) {
    player->setPlayerPortNo(initInfo.getPortNo());
    player->setPlayerViewMtx(initInfo.getViewMtx());

    sead::Matrix34f poseMtx;
    poseMtx.makeQT(initInfo.getQuat(), initInfo.getTrans());
    al::updatePoseMtx(player, &poseMtx);
}

const char* rs::getInitPlayerModelName(const PlayerInitInfo& initInfo) {
    sead::FixedSafeString<64> modelName(initInfo.getModelName());
    if (modelName.isEmpty())
        return "Mario";
    return initInfo.getModelName();
}

bool rs::isNeedCreateNoseNeedle(const PlayerInitInfo& initInfo) {
    return initInfo.isNeedCreateNoseNeedle();
}

bool rs::isClosetScenePlayer(const PlayerInitInfo& initInfo) {
    return initInfo.isClosetScenePlayer();
}

al::GamePadSystem* rs::getGamePadSystem(const PlayerInitInfo& initInfo) {
    return initInfo.getGamePadSystem();
}

const char* rs::getInitCapTypeName(const PlayerInitInfo& initInfo) {
    sead::FixedSafeString<64> capTypeName(initInfo.getCapTypeName());
    if (capTypeName.isEmpty())
        return "Mario";
    return initInfo.getCapTypeName();
}

al::SklAnimRetargettingInfo* rs::createPlayerSklRetargettingInfo(al::LiveActor* actor,
                                                                 const sead::Vector3f& scale) {
    const u8* byml =
        al::tryGetBymlFromObjectResource("PlayerRetargettingInfo", "PlayerRetargettingInfo");
    al::ByamlIter iter(byml);

    const nn::g3d::ResModel* resModel =
        al::findResource(al::StringTmp<256>("ObjectData/%s", al::getModelName(actor)))
            ->getResFile()
            ->mModels;

    const nn::g3d::ResSkeleton* skeleton =
        sead::BitUtil::bitCastPtr<const nn::g3d::ResSkeleton*>(&resModel->_0[0x20]);
    al::SklAnimRetargettingInfo* info =
        new (0x10) PlayerSklAnimRetargettingInfo(skeleton, iter, "Mario", scale);
    al::bindSklAnimRetargetting(actor, info);
    al::validateSklAnimRetargetting(actor);
    return info;
}

const sead::Vector3f& rs::getPlayerPos(const al::LiveActor* actor) {
    return al::getTrans(getPlayer(actor));
}

const sead::Vector3f& rs::getPlayerHeadPos(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    if (info && info->getFormSensorCollisionArranger())
        return info->getFormSensorCollisionArranger()->getHeadPos();

    return al::getTrans(actor);
}

const sead::Vector3f& rs::getPlayerBodyPos(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    if (info && info->getFormSensorCollisionArranger())
        return info->getFormSensorCollisionArranger()->getBodyPos();

    return al::getTrans(actor);
}

void rs::calcPlayerSideDir(sead::Vector3f* out, const al::LiveActor* actor) {
    al::calcSideDir(out, getPlayer(actor));
}

void rs::calcPlayerUpDir(sead::Vector3f* out, const al::LiveActor* actor) {
    al::calcUpDir(out, getPlayer(actor));
}

void rs::calcPlayerFrontDir(sead::Vector3f* out, const al::LiveActor* actor) {
    al::calcFrontDir(out, getPlayer(actor));
}

void rs::calcPlayerGroundPoseUp(sead::Vector3f* out, const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    PlayerJointControlKeeper* keeper = info ? info->getPlayerJointControlKeeper() : nullptr;
    keeper->calcGroundPoseUp(out);
}

bool rs::isNearPlayerH(const al::LiveActor* actor, f32 distance) {
    sead::Vector3f diff = getPlayerPos(actor) - al::getTrans(actor);
    al::verticalizeVec(&diff, al::getGravity(actor), diff);
    return diff.length() < distance;
}

void rs::calcPlayerFollowLayoutWorldPos(sead::Vector3f* out, const al::LiveActor* actor) {
    if (isPlayerHack(actor)) {
        PlayerInfo* info = getPlayerInfo(actor);
        HackCap* cap = nullptr;
        if (info)
            cap = info->getHackCap();
        cap->calcHackFollowTrans(out, false);
        return;
    }

    out->set(getPlayerHeadPos(actor));
}

bool rs::isPlayerHack(const al::LiveActor* actor) {
    return getCurrentHackName(actor) != nullptr;
}

bool rs::tryCalcPlayerCeilingSpace(f32* out, const al::LiveActor* actor, f32 height, f32 margin) {
    PlayerInfo* info = getPlayerInfo(actor);
    IUsePlayerCeilingCheck* ceilingCheck = info ? info->getCeilingCheck() : nullptr;
    f32 checkHeight = height + margin;
    if (ceilingCheck) {
        f32 safety = ceilingCheck->getSafetyCeilSpace();
        if (!(checkHeight < safety)) {
            *out = sead::Mathf::clampMin(safety - margin, 0.0f);
            return false;
        }
    }

    *out = height;
    return true;
}

// NONMATCHING: https://decomp.me/scratch/nicSk
bool rs::tryCalcKidsGuideCeilingSpace(f32* out, const al::LiveActor* actor, f32 height,
                                      f32 margin) {
    PlayerInfo* info = getPlayerInfo(actor);
    if (info) {
        IUsePlayerCeilingCheck* ceilingCheck = info->getCeilingCheck();
        if (ceilingCheck) {
            f32 ceil = height + margin;
            f32 safety = ceilingCheck->getSafetyCeilSpace();
            f32 ceilHeight = ceilingCheck->getCeilCheckHeight();
            if (ceil <= ceilHeight && ceil >= safety) {
                *out = sead::Mathf::clampMin(ceilHeight - margin, 0.0f);
                return false;
            }
        }
    }

    *out = height;
    return true;
}

const sead::Vector3f& rs::getPlayerVelocity(const al::LiveActor* actor) {
    return al::getVelocity(getPlayer(actor));
}

// NONMATCHING: https://decomp.me/scratch/I4vY0
bool rs::tryCalcPlayerModelHeadJointPos(sead::Vector3f* out, const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    if (!info || !info->getModelHolder())
        return false;

    al::LiveActor* model = info->getModelHolder()->getCurrentModelActor();
    if (al::isExistJoint(model, "Head")) {
        out->set(al::getJointMtxPtr(model, "Head")->getTranslation());
        return true;
    }

    return false;
}

// NONMATCHING: https://decomp.me/scratch/cxeiD
bool rs::tryCalcPlayerModelHeadJointUp(sead::Vector3f* out, const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    PlayerModelHolder* holder = info ? info->getModelHolder() : nullptr;
    if (!holder)
        return false;

    al::LiveActor* model = holder->getCurrentModelActor();
    if (al::isExistJoint(model, "Head")) {
        const sead::Matrix34f* mtx = al::getJointMtxPtr(model, "Head");
        out->set(mtx->getBase(1));
        al::tryNormalizeOrZero(out);
        return true;
    }

    return false;
}

// NONMATCHING: https://decomp.me/scratch/9EUWx
bool rs::tryCalcPlayerModelHeadJointFront(sead::Vector3f* out, const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    PlayerModelHolder* holder = info ? info->getModelHolder() : nullptr;
    if (!holder)
        return false;

    al::LiveActor* model = holder->getCurrentModelActor();
    if (al::isExistJoint(model, "Head")) {
        const sead::Matrix34f* mtx = al::getJointMtxPtr(model, "Head");
        out->set(mtx->getBase(2));
        al::tryNormalizeOrZero(out);
        return true;
    }

    return false;
}

// NONMATCHING: https://decomp.me/scratch/MQtUu
bool rs::tryCalcPlayerModelHeadJointSide(sead::Vector3f* out, const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    PlayerModelHolder* holder = info ? info->getModelHolder() : nullptr;
    if (!holder)
        return false;

    al::LiveActor* model = holder->getCurrentModelActor();
    if (al::isExistJoint(model, "Head")) {
        const sead::Matrix34f* mtx = al::getJointMtxPtr(model, "Head");
        out->set(mtx->getBase(0));
        al::tryNormalizeOrZero(out);
        return true;
    }

    return false;
}

bool rs::tryCalcPlayerModelNoseJointMtx(sead::Matrix34f* out, const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    PlayerModelHolder* holder = info ? info->getModelHolder() : nullptr;
    if (!holder)
        return false;

    al::LiveActor* nose = al::tryGetSubActor(holder->getCurrentModelActor(), "");
    if (al::isExistJoint(nose, "Nose")) {
        *out = *al::getJointMtxPtr(nose, "Nose");
        return true;
    }

    return false;
}

bool rs::isPlayerDamageStopDemo(const al::LiveActor* actor) {
    return getPlayer(actor)->isDamageStopDemo();
}

bool rs::isPlayerHackType(const al::LiveActor* actor, s32 type) {
    if (type == -2)
        return true;

    const char* current = getCurrentHackName(actor);
    if (type != 0) {
        if (type == -1)
            return current == nullptr;
        if (current)
            return al::isEqualString(cHackTypeNames[type], current);
        return false;
    }

    return current != nullptr;
}

bool rs::isPlayerHackRigidBody(const al::LiveActor* actor) {
    return isCurrentHack(actor, "RigidBodyBox");
}

bool rs::isPlayerHackJugemFishing(const al::LiveActor* actor) {
    return isCurrentHack(actor, "JugemFishing");
}

bool rs::isPlayerHackKuriboAny(const al::LiveActor* actor) {
    return isPlayerHackKuribo(actor) || isPlayerHackKuriboWing(actor);
}

bool rs::isPlayerHackKuribo(const al::LiveActor* actor) {
    return isCurrentHack(actor, "Kuribo");
}

bool rs::isPlayerHackKuriboWing(const al::LiveActor* actor) {
    return isCurrentHack(actor, "KuriboWing");
}

bool rs::isPlayerHackStatueMario(const al::LiveActor* actor) {
    return isCurrentHack(actor, "Statue");
}

bool rs::isPlayerHackEnemy(const al::LiveActor* actor) {
    return isPlayerHackKuribo(actor) || isPlayerHackHammerBros(actor) ||
           isCurrentHack(actor, "Bull") || isCurrentHack(actor, "KaronWing");
}

bool rs::isPlayerHackTrilemmaRock(const al::LiveActor* actor) {
    return isCurrentHack(actor, "");
}

bool rs::isPlayerHackTrilemmaPaper(const al::LiveActor* actor) {
    return isCurrentHack(actor, "");
}

bool rs::isPlayerHackTrilemmaScissors(const al::LiveActor* actor) {
    return isCurrentHack(actor, "");
}

bool rs::isPlayerHackElectricWire(const al::LiveActor* actor) {
    return isCurrentHack(actor, "ElectricWire");
}

bool rs::isPlayerHackTRex(const al::LiveActor* actor) {
    return isCurrentHack(actor, "TRex");
}

bool rs::isPlayerHackFukankun(const al::LiveActor* actor) {
    return isCurrentHack(actor, "Fukankun");
}

bool rs::isPlayerHackHosui(const al::LiveActor* actor) {
    return isCurrentHack(actor, "Hosui");
}

bool rs::isPlayerHackYoshi(const al::LiveActor* actor) {
    return isCurrentHack(actor, "Yoshi");
}

bool rs::isPlayerHackYukimaru(const al::LiveActor* actor) {
    return isCurrentHack(actor, "Yukimaru");
}

bool rs::isPlayerHackHammerBros(const al::LiveActor* actor) {
    return isCurrentHack(actor, "HammerBros");
}

bool rs::isPlayerHackBazookaElectric(const al::LiveActor* actor) {
    return isCurrentHack(actor, "BazookaElectric");
}

bool rs::isPlayerHackBubble(const al::LiveActor* actor) {
    return isCurrentHack(actor, "Bubble");
}

bool rs::isPlayerHackTank(const al::LiveActor* actor) {
    return isCurrentHack(actor, "Tank");
}

bool rs::isPlayerHackTsukkun(const al::LiveActor* actor) {
    return isCurrentHack(actor, "Tsukkun");
}

bool rs::isPlayerHackPukupuku(const al::LiveActor* actor) {
    return isCurrentHack(actor, "Pukupuku");
}

bool rs::isPlayerHackPukupukuAll(const al::LiveActor* actor) {
    return isPlayerHackPukupuku(actor) || isCurrentHack(actor, "PukupukuSnow");
}

bool rs::isPlayerHackRadiconNpc(const al::LiveActor* actor) {
    return isCurrentHack(actor, "Radicon");
}

bool rs::isPlayerHackSenobi(const al::LiveActor* actor) {
    return isCurrentHack(actor, "Senobi");
}

bool rs::isPlayerHackKakku(const al::LiveActor* actor) {
    return isCurrentHack(actor, "Kakku");
}

bool rs::isPlayerHackGroupTalkScare(const al::LiveActor* actor) {
    return getHackKeeper(actor)->isHackGroupTalkScare();
}

bool rs::isPlayerHackGroupUseCameraStick(const al::LiveActor* actor) {
    return isPlayerHackTank(actor) || isPlayerHackRadiconNpc(actor);
}

bool rs::isPlayerHackNoSeparateCameraInput(const al::LiveActor* actor) {
    PlayerHackKeeper* keeper = getHackKeeper(actor);
    if (!keeper || !keeper->getCurrentHackName())
        return false;
    return getHackKeeper(actor)->isHackNoSeparateCameraInput();
}

bool rs::isPlayerEnableToSeeOddSpace(const al::LiveActor* actor) {
    if (!isCurrentHack(actor, "Megane"))
        return false;

    PlayerHackKeeper* keeper = getHackKeeper(actor);
    Megane* hack = nullptr;
    if (keeper)
        hack = keeper->getMeganeHack();
    return hack->isWearingGlasses();
}

bool rs::isPlayerMini(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    PlayerModelChangerHakoniwa* modelChanger = info ? info->getModelChangerHakoniwa() : nullptr;
    return modelChanger && modelChanger->isMini();
}

bool rs::isPlayer3D(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    PlayerModelChangerHakoniwa* modelChanger = info ? info->getModelChangerHakoniwa() : nullptr;
    return !modelChanger || !modelChanger->is2DModel();
}

bool rs::isPlayer2D(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    PlayerModelChangerHakoniwa* modelChanger = info ? info->getModelChangerHakoniwa() : nullptr;
    return modelChanger && modelChanger->is2DModel();
}

bool rs::isPlayerSquat(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    PlayerFormSensorCollisionArranger* arranger =
        info ? info->getFormSensorCollisionArranger() : nullptr;
    return arranger && arranger->getFormStatus() == 2;
}

bool rs::isPlayerInWater(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    const IJudge* judge = info ? info->getJudgeCameraInWater() : nullptr;
    return judge && rs::isJudge(judge);
}

bool rs::isPlayerPoleClimb(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    return info && rs::isJudge(info->getJudgeStatusPoleClimb());
}

bool rs::isPlayerWallCatch(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    return info && rs::isJudge(info->getJudgeNrvWallCatch());
}

bool rs::isPlayerCameraSubjective(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    return info && rs::isJudge(info->getJudgeActiveCameraSubjective());
}

bool rs::isPlayerEnableTalkGround(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    return info && rs::isJudge(info->getJudgeTalkGround());
}

bool rs::isPlayerEnableTalkSwim(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    return info && rs::isJudge(info->getJudgeTalkSwim());
}

bool rs::isPlayerBinding(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    PlayerBindKeeper* keeper = info ? info->getBindKeeper() : nullptr;
    return keeper && keeper->getBindSensor();
}

bool rs::isPlayerEquipSomething(const al::LiveActor* actor) {
    PlayerEquipmentUser* user = getPlayerInfo(actor)->getEquipmentUser();
    return user->getEquipmentSensor() != nullptr;
}

bool rs::isPlayerEquipRocketFlower(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    PlayerEquipmentUser* user = nullptr;
    if (info)
        user = info->getEquipmentUser();

    return user->getEquipmentSensor() && user->getEquipmentInfo()->isForceDash();
}

bool rs::isPlayerEquipNoSeparateAction(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    PlayerEquipmentUser* user = info ? info->getEquipmentUser() : nullptr;
    return user && user->getEquipmentSensor() && user->getEquipmentInfo()->isNoCapThrow();
}

bool rs::isPlayerCarrySomething(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    PlayerCarryKeeper* keeper = info ? info->getCarryKeeper() : nullptr;
    return keeper && keeper->isCarry();
}

bool rs::isPlayerNoInput(const al::LiveActor* actor) {
    PlayerInput* input = getPlayerInput(actor);
    return input && input->isNoInput();
}

bool rs::isPlayerEnableShowTutorialInput(const al::LiveActor* actor) {
    PlayerInput* input = getPlayerInput(actor);
    return input && input->isEnableShowTutorialInput();
}

bool rs::isPlayerSafetyPointRecovery(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    return info && rs::isJudge(info->getJudgeSafetyPointRecovery());
}

bool rs::isPlayerPlayingSwitchOnAnim(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    return info && rs::isJudge(info->getJudgePlaySwitchOnAreaWaitAnim());
}

bool rs::isPlayerEquipNoSeparateTutorial(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    PlayerEquipmentUser* user = info ? info->getEquipmentUser() : nullptr;
    return user && user->getEquipmentSensor() && user->getEquipmentInfo()->isNoCapThrow();
}

bool rs::isPlayerWaitSleep(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    return info && rs::isJudge(info->getJudgeSleep());
}

bool rs::isPlayerAboveSeparateCapGuideArrow(const al::LiveActor* actor) {
    HackCap* cap = getHackCap(actor);
    return cap && cap->isSeparateHide();
}

bool rs::isPlayerInvalidateGuideArrow(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    return info && !rs::isJudge(info->getJudgeEnableGuideArrow());
}

bool rs::isPlayerInvisibleCap(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    return info && info->getCostumeInfo()->isInvisibleHead();
}

bool rs::isPlayerEnablePeachAmiibo(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    return !info || rs::isJudge(info->getJudgeEnablePeachAmiibo());
}

bool rs::isPlayerActiveMarioAmiiboInvincible(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    PlayerDamageKeeper* damageKeeper = info ? info->getDamageKeeper() : nullptr;
    return damageKeeper && damageKeeper->isPreventDamage();
}

bool rs::isPlayerCollidedGround(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    PlayerBindKeeper* bindKeeper = info ? info->getBindKeeper() : nullptr;
    if (bindKeeper && bindKeeper->getBindSensor()) {
        PlayerInfo* bindInfo = getPlayerInfo(actor);
        PlayerBindKeeper* currentBindKeeper = nullptr;
        if (bindInfo)
            currentBindKeeper = bindInfo->getBindKeeper();
        if (currentBindKeeper->sendMsgCollidedGround())
            return true;
    }

    PlayerHackKeeper* keeper = getHackKeeper(actor);
    if (keeper && keeper->getCurrentHackName()) {
        PlayerHackKeeper* hackKeeper = getHackKeeper(actor);
        if (!hackKeeper)
            return false;
        if (!hackKeeper->getHackSensor())
            return false;
        if (hackKeeper->isHackUsePlayerCollision())
            return rs::isCollidedGround(hackKeeper->getPlayerCollision());
        actor = hackKeeper->getHack();
    } else {
        IUsePlayerCollision* collision = getPlayerCollision(actor);
        if (collision)
            return rs::isCollidedGround(collision);
    }

    return al::isExistActorCollider(actor) && al::isCollidedGround(actor);
}

bool rs::isPlayerCollidedCeiling(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    PlayerBindKeeper* bindKeeper = info ? info->getBindKeeper() : nullptr;
    if (bindKeeper && bindKeeper->getBindSensor()) {
        (void)getPlayerInfo(actor);
        return false;
    }

    PlayerHackKeeper* keeper = getHackKeeper(actor);
    if (keeper && keeper->getCurrentHackName()) {
        PlayerHackKeeper* hackKeeper = getHackKeeper(actor);
        if (!hackKeeper || !hackKeeper->getHackSensor())
            return false;
        if (hackKeeper->isHackUsePlayerCollision())
            return rs::isCollidedCeiling(hackKeeper->getPlayerCollision());
        actor = hackKeeper->getHack();
    } else {
        IUsePlayerCollision* collision = getPlayerCollision(actor);
        if (collision)
            return rs::isCollidedCeiling(collision);
    }

    return al::isExistActorCollider(actor) && al::isCollidedCeiling(actor);
}

bool rs::isPlayerOnGround(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    PlayerBindKeeper* bindKeeper = info ? info->getBindKeeper() : nullptr;
    if (bindKeeper && bindKeeper->getBindSensor()) {
        PlayerInfo* bindInfo = getPlayerInfo(actor);
        PlayerBindKeeper* currentBindKeeper = nullptr;
        if (bindInfo)
            currentBindKeeper = bindInfo->getBindKeeper();
        if (currentBindKeeper->sendMsgCollidedGround())
            return true;
    }

    const al::LiveActor* groundActor = actor;
    PlayerHackKeeper* keeper = getHackKeeper(actor);
    if (keeper && keeper->getCurrentHackName()) {
        PlayerHackKeeper* hackKeeper = getHackKeeper(actor);
        if (!hackKeeper || !hackKeeper->getHackSensor())
            return false;
        groundActor = hackKeeper->getHack();
        if (hackKeeper->isHackUsePlayerCollision())
            return rs::isOnGround(groundActor, hackKeeper->getPlayerCollision());
    } else {
        PlayerActorBase* player = getPlayer(actor);
        groundActor = player;
        IUsePlayerCollision* collision = player->getPlayerCollision();
        if (collision)
            return rs::isOnGround(groundActor, collision);
    }

    return al::isExistActorCollider(groundActor) && al::isOnGround(groundActor, 0);
}

bool rs::isPlayerOnActor(const al::LiveActor* actor) {
    const al::CollisionParts* parts = tryGetPlayerGroundCollisionParts(actor);
    return parts && parts->getConnectedHost() == actor;
}

bool rs::isPlayerOnChairActor(const al::LiveActor* actor) {
    PlayerHackKeeper* hackKeeper = getHackKeeper(actor);
    if (hackKeeper && hackKeeper->getHack())
        return false;

    IUsePlayerCollision* collision = getPlayerCollision(actor);
    return collision && rs::isCollidedGround(collision) && rs::isCollisionCodeChair(collision) &&
           rs::getCollidedGroundCollisionParts(collision)->getConnectedHost() == actor;
}

bool rs::isPlayerOnBedActor(const al::LiveActor* actor) {
    PlayerHackKeeper* hackKeeper = getHackKeeper(actor);
    if (hackKeeper && hackKeeper->getHack())
        return false;

    IUsePlayerCollision* collision = getPlayerCollision(actor);
    return collision && rs::isCollidedGround(collision) && rs::isCollisionCodeBed(collision) &&
           rs::getCollidedGroundCollisionParts(collision)->getConnectedHost() == actor;
}

bool rs::isPlayerFaceToTarget(const al::LiveActor* actor, const sead::Vector3f& target) {
    sead::Vector3f front = {0.0f, 0.0f, 0.0f};
    al::calcFrontDir(&front, getPlayer(actor));

    sead::Vector3f dir = target - getPlayerPos(actor);
    dir.y = 0.0f;

    return al::tryNormalizeOrZero(&dir) && al::calcAngleDegree(front, dir) < cFaceTargetAngle;
}

bool rs::isPlayerFaceToTarget(const al::LiveActor* actor) {
    return isPlayerFaceToTarget(actor, al::getTrans(actor));
}

bool rs::isPlayerFaceToTargetInRange(const al::LiveActor* actor, const sead::Vector3f& target,
                                     f32 range) {
    sead::Vector3f front = {0.0f, 0.0f, 0.0f};
    al::calcFrontDir(&front, getPlayer(actor));

    sead::Vector3f dir = target - getPlayerPos(actor);
    dir.y = 0.0f;

    if (dir.length() > range)
        return false;
    return al::tryNormalizeOrZero(&dir) && al::calcAngleDegree(front, dir) < cFaceTargetAngle;
}

bool rs::isPlayerFaceToTargetInRange(const al::LiveActor* actor, f32 range) {
    return isPlayerFaceToTargetInRange(actor, al::getTrans(actor), range);
}

bool rs::tryGetFlyingCapPos(sead::Vector3f* out, const al::LiveActor* actor) {
    HackCap* cap = getHackCap(actor);
    if (!cap)
        return false;

    if (cap->isSpinAttack()) {
        al::calcJointPos(out, cap, "JointRoot");
        return true;
    }

    if (cap->isFlying()) {
        out->set(al::getTrans(cap));
        return true;
    }

    return false;
}

bool rs::tryGetFlyingCapVelocity(sead::Vector3f* out, const al::LiveActor* actor) {
    HackCap* cap = getHackCap(actor);
    if (!cap)
        return false;

    if (cap->isSpinAttack()) {
        out->set(0.0f, 0.0f, 0.0f);
        return true;
    }

    if (cap->isFlying()) {
        out->set(al::getVelocity(cap));
        return true;
    }

    return false;
}

bool rs::isEnableReceiveCapStartLockOnAngle(const al::LiveActor* actor,
                                            const al::HitSensor* sensor) {
    HackCap* cap = getHackCap(actor);
    if (!cap || !cap->isSpinAttack())
        return true;

    PlayerActorBase* player = getPlayer(actor);
    const al::HitSensor* spinSensor = al::getHitSensor(player, "SpinAttack");
    sead::Vector3f dir = {0.0f, 0.0f, 0.0f};
    if (!al::calcDirBetweenSensorsH(&dir, spinSensor, sensor))
        return true;

    sead::Vector3f front = {0.0f, 0.0f, 0.0f};
    al::calcFrontDir(&front, player);
    al::verticalizeVec(&front, al::getGravity(player), front);
    if (al::tryNormalizeOrZero(&front))
        return dir.dot(front) >= cCapLockOnDotLimit;

    return true;
}

bool rs::isGuardNosePainCap(const al::LiveActor* actor) {
    HackCap* cap = getHackCap(actor);
    return cap && !cap->isSeparateHide() &&
           GameDataFunction::isEnableCap(GameDataHolderAccessor(cap)) && cap->isCatched();
}

bool rs::isEquipCapCatched(const al::LiveActor* actor) {
    HackCap* cap = getHackCap(actor);
    return cap && cap->isCatched();
}

f32 rs::getFlyingCapThrowSpeedMax(const al::LiveActor* actor) {
    HackCap* cap = getHackCap(actor);
    return cap ? cap->getFlyingSpeedMax() : 0.0f;
}

bool rs::isPlayerCapSpinOrFlying(const al::LiveActor* actor) {
    HackCap* cap = getHackCap(actor);
    return cap && (cap->isSpinAttack() || cap->isFlying());
}

bool rs::isPlayerCapFlying(const al::LiveActor* actor) {
    HackCap* cap = getHackCap(actor);
    return cap && cap->isFlying();
}

bool rs::isPlayerCapSeparateThrow(const al::LiveActor* actor) {
    HackCap* cap = getHackCap(actor);
    return cap && cap->isSeparateThrowFlying();
}

bool rs::isEmptyPlayerOxygen(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    PlayerOxygen* oxygen = info ? info->getOxygen() : nullptr;
    return oxygen && oxygen->getOxygenLevel() <= 0.0f;
}

void rs::recoveryPlayerOxygen(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    PlayerOxygen* oxygen = info ? info->getOxygen() : nullptr;
    if (oxygen)
        oxygen->reset();
}

void rs::requestBindPlayer(const al::LiveActor* actor, al::HitSensor* sensor) {
    PlayerInfo* info = getPlayerInfo(actor);
    PlayerBindKeeper* bindKeeper = info ? info->getBindKeeper() : nullptr;
    if (bindKeeper)
        bindKeeper->appendBindRequest(sensor);
}

void rs::sendMsgBreakFloorToPlayer(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    if (!info)
        return;

    const al::CollisionParts* parts = tryGetPlayerGroundCollisionParts(actor);
    if (parts && parts->getConnectedHost() == actor)
        rs::sendMsgRequestPlayerJumpBreakFloor(info->getBodyHitSensor(),
                                               parts->getConnectedSensor());
}

void rs::requestStageStartHack(const al::LiveActor* actor, al::HitSensor* sensor,
                               const CapTargetInfo* targetInfo, al::LiveActor* hackActor) {
    getHackKeeper(actor)->requestForceHackStageStart(sensor, targetInfo, hackActor);
}

bool rs::isEnableOpenMap(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    PlayerBindKeeper* bindKeeper = info ? info->getBindKeeper() : nullptr;
    if (bindKeeper && bindKeeper->getBindSensor()) {
        PlayerInfo* bindInfo = getPlayerInfo(actor);
        PlayerBindKeeper* currentBindKeeper = nullptr;
        if (bindInfo)
            currentBindKeeper = bindInfo->getBindKeeper();
        return currentBindKeeper->sendMsgEnableMapCheckPointWarp();
    }

    PlayerHackKeeper* hackKeeper = getHackKeeper(actor);
    if (hackKeeper && hackKeeper->getCurrentHackName())
        return getHackKeeper(actor)->sendMsgEnableMapCheckPointWarp();

    info = getPlayerInfo(actor);
    if (info && info->getJudgeNrvSwim() && rs::isJudge(info->getJudgeNrvSwim()))
        return true;

    return rs::isPlayerCollidedGround(actor);
}

void rs::syncPlayerModelAlpha(al::LiveActor* actor) {
    al::setModelAlphaMask(actor, getPlayerInfo(actor)->getAnimator()->getModelAlpha());
}

f32 rs::getPlayerShadowDropLength(const al::LiveActor* actor) {
    PlayerInfo* info = getPlayerInfo(actor);
    PlayerModelChangerHakoniwa* changer = info ? info->getModelChangerHakoniwa() : nullptr;
    if (changer && changer->is2DModel())
        return 0.0f;

    info = getPlayerInfo(actor);
    PlayerModelHolder* holder = info ? info->getModelHolder() : nullptr;
    if (!holder)
        return 0.0f;

    return al::getDepthShadowMapLength(holder->getCurrentModelActor(),
                                       PlayerFunction::getPlayerDepthGroundShadowName());
}

bool rs::isExistLabelInPlayerHackSystemMstxt(const al::LiveActor* actor,
                                             const al::IUseMessageSystem* messageSystem,
                                             const char* category, const char* suffix) {
    PlayerHackKeeper* hackKeeper = getHackKeeper(actor);
    const HackObjInfo* hackInfo = nullptr;
    if (hackKeeper) {
        GameDataHolderAccessor accessor(actor);
        const char* hackName = hackKeeper->getCurrentHackName();
        hackInfo = GameDataFunction::getHackObjInfo(accessor, hackName);
    }

    PlayerHackKeeper* currentHackKeeper = getHackKeeper(actor);
    const char* hackName = currentHackKeeper ? currentHackKeeper->getCurrentHackName() : nullptr;
    al::StringTmp<64> label(hackName);
    const char* tutorialName = hackInfo->tutorialName;
    if (tutorialName)
        label.format("%s", tutorialName);
    if (suffix)
        label.appendWithFormat("_%s", suffix);
    return al::isExistLabelInSystemMessage(messageSystem, category, label.cstr());
}

const char16* rs::getPlayerHackSystemMessageString(const al::LiveActor* actor,
                                                   const al::IUseMessageSystem* messageSystem,
                                                   const char* category, const char* suffix) {
    PlayerHackKeeper* hackKeeper = getHackKeeper(actor);
    const HackObjInfo* hackInfo = nullptr;
    if (hackKeeper) {
        GameDataHolderAccessor accessor(actor);
        const char* hackName = hackKeeper->getCurrentHackName();
        hackInfo = GameDataFunction::getHackObjInfo(accessor, hackName);
    }

    PlayerHackKeeper* currentHackKeeper = getHackKeeper(actor);
    const char* hackName = currentHackKeeper ? currentHackKeeper->getCurrentHackName() : nullptr;
    al::StringTmp<64> label(hackName);
    if (hackInfo->tutorialName)
        label.format("%s", hackInfo->tutorialName);
    const char* suffixOrEmpty = suffix ? suffix : "";
    if (!al::isEqualString(suffixOrEmpty, ""))
        label.appendWithFormat("_%s", suffix);
    return al::getSystemMessageString(messageSystem, category, label.cstr());
}

void rs::getPlayerHackTutorialMoviePath(al::StringTmp<64>* out, const al::LiveActor* actor,
                                        const char*) {
    out->format("content:/MovieData/%s.mp4", getCurrentHackName(actor));
}

void rs::calcPlayerAmiiboPeachAppearBasePos(sead::Vector3f* out, const al::LiveActor* actor) {
    PlayerHackKeeper* keeper = getHackKeeper(actor);
    if (keeper && keeper->getCurrentHackName()) {
        PlayerInfo* info = getPlayerInfo(actor);
        HackCap* cap = nullptr;
        if (info)
            cap = info->getHackCap();
        cap->calcHackFollowTrans(out, false);
        return;
    }

    out->set(getPlayerPos(actor));
}

bool rs::isPlayerInputHoldBalloonSet(const al::LiveActor* actor) {
    PlayerInput* input = getPlayerInput(actor);
    return input && input->isHoldBalloonSet();
}

bool rs::isPlayerInputHoldSquat(const al::LiveActor* actor) {
    PlayerInput* input = getPlayerInput(actor);
    return input && input->isHoldSquat();
}

bool rs::isPlayerInputHoldCapKeepLockOn(const al::LiveActor* actor) {
    HackCap* cap = getHackCap(actor);
    return cap && cap->isHoldInputKeepLockOn();
}

bool rs::isPlayerInputHoldCarryAction(const al::LiveActor* actor) {
    PlayerInput* input = getPlayerInput(actor);
    return input && input->isEnableCarry();
}

bool rs::isPlayerInputHoldEnterUpperDokan2D(const al::LiveActor* actor) {
    PlayerInput* input = getPlayerInput(actor);
    return input && input->isHoldEnterUpperDokan2D();
}

bool rs::isPlayerInputHoldEnterSideDokan2D(const al::LiveActor* actor, const sead::Vector3f& dir) {
    PlayerInput* input = getPlayerInput(actor);
    return input && input->isHoldEnterSideDokan2D(dir);
}

bool rs::isPlayerInputTriggerStartTalk(const al::LiveActor* actor) {
    PlayerInput* input = getPlayerInput(actor);
    return input && input->isTriggerStartTalk();
}

bool rs::isPlayerInputTriggerRide(const al::LiveActor* actor) {
    PlayerInput* input = getPlayerInput(actor);
    return input && input->isTriggerSpinCap();
}

bool rs::isPlayerInputTriggerSeparateCapJangoHelp(const al::LiveActor* actor) {
    PlayerInput* input = getPlayerInput(actor);
    return input && input->isTriggerSeparateCapJangoHelp();
}

bool rs::tryGetPlayerInputWallAlongWallNormal(sead::Vector3f* out, const al::LiveActor* actor) {
    PlayerHackKeeper* keeper = getHackKeeper(actor);
    if (keeper && keeper->getCurrentHackName()) {
        PlayerHackKeeper* hackKeeper = getHackKeeper(actor);
        if (!hackKeeper || !hackKeeper->getHackSensor())
            return false;
        if (hackKeeper->isHackUsePlayerCollision()) {
            if (rs::isCollidedWallFace(hackKeeper->getPlayerCollision())) {
                out->set(rs::getCollidedWallNormal(hackKeeper->getPlayerCollision()));
                return true;
            }
        } else {
            const al::LiveActor* hack = hackKeeper->getHack();
            if (al::isExistActorCollider(hack) && al::isCollidedWallFace(hack)) {
                out->set(al::getCollidedWallNormal(hack));
                return true;
            }
        }
        return false;
    }

    IUsePlayerCollision* collision = getPlayerCollision(actor);
    if (collision && rs::isCollidedWallFace(collision)) {
        out->set(rs::getCollidedWallNormal(collision));
        return true;
    }

    return false;
}

void rs::calcPlayerHoldPos(sead::Vector3f* out, const al::HitSensor* sensor) {
    const al::LiveActor* host = al::getSensorHost(sensor);
    PlayerInfo* info = getPlayerInfo(host);
    PlayerCarryKeeper* carryKeeper = info ? info->getCarryKeeper() : nullptr;

    if (carryKeeper->isThrowHandR()) {
        sead::Matrix34f handMtx = sead::Matrix34f::ident;
        info = getPlayerInfo(host);
        PlayerModelHolder* holder = info ? info->getModelHolder() : nullptr;
        if (holder) {
            al::LiveActor* model = holder->getCurrentModelActor();
            if (al::isExistJoint(model, "HandR"))
                handMtx = *al::getJointMtxPtr(model, "HandR");
        }

        sead::Vector3f pos = {0.0f, 0.0f, 0.0f};
        al::calcTransLocalOffsetByMtx(&pos, handMtx, sead::Vector3f::ex * 30.0f);
        out->set(pos);
        return;
    }

    sead::Vector3f handL = {0.0f, 0.0f, 0.0f};
    info = getPlayerInfo(host);
    PlayerModelHolder* holder = info ? info->getModelHolder() : nullptr;
    if (holder) {
        al::LiveActor* model = holder->getCurrentModelActor();
        if (al::isExistJoint(model, "HandL"))
            handL = al::getJointMtxPtr(model, "HandL")->getTranslation();
    }

    sead::Vector3f handR = {0.0f, 0.0f, 0.0f};
    info = getPlayerInfo(host);
    holder = info ? info->getModelHolder() : nullptr;
    if (holder) {
        al::LiveActor* model = holder->getCurrentModelActor();
        if (al::isExistJoint(model, "HandR"))
            handR = al::getJointMtxPtr(model, "HandR")->getTranslation();
    }

    out->set((handL + handR) * 0.5f);
}

void rs::calcPlayerHoldMtx(sead::Matrix34f* out, const al::HitSensor* sensor) {
    const al::LiveActor* host = al::getSensorHost(sensor);
    sead::Matrix34f base = sead::Matrix34f::ident;
    PlayerInfo* info = getPlayerInfo(host);
    PlayerModelHolder* holder = info ? info->getModelHolder() : nullptr;
    if (holder) {
        al::LiveActor* model = holder->getCurrentModelActor();
        if (al::isExistJoint(model, "Spine2"))
            base = *al::getJointMtxPtr(model, "Spine2");
    }

    sead::Quatf quat;
    quat.setAxisRadian(sead::Vector3f::ez, -sead::Mathf::pi() / 2);
    sead::Matrix34f rotMtxZ;
    rotMtxZ.fromQuat(quat);
    out->setMul(base, rotMtxZ);

    quat.setAxisRadian(sead::Vector3f::ey, -sead::Mathf::pi() / 2);
    sead::Matrix34f rotMtxY;
    rotMtxY.fromQuat(quat);
    out->setMul(*out, rotMtxY);

    sead::Vector3f pos;
    calcPlayerHoldPos(&pos, sensor);
    out->setTranslation(pos);
}

void rs::setPlayerHoldJointAngle(const al::HitSensor* sensor, const sead::Vector3f& angle) {
    const PlayerActorBase* player = static_cast<const PlayerActorBase*>(al::getSensorHost(sensor));
    PlayerCarryKeeper* carryKeeper = player->getPlayerInfo()->getCarryKeeper();
    carryKeeper->setJointAngle(angle);
}

void rs::updatePosePlayerHold(al::LiveActor* actor, const al::HitSensor* sensor) {
    sead::Matrix34f mtx = sead::Matrix34f::ident;
    calcPlayerHoldMtx(&mtx, sensor);
    al::updatePoseMtx(actor, &mtx);
}

void rs::updatePosePlayerHoldAddOffset(al::LiveActor* actor, const al::HitSensor* sensor,
                                       const sead::Vector3f& offset) {
    updatePosePlayerHold(actor, sensor);
    al::multVecPose(al::getTransPtr(actor), actor, offset);
}

f32 rs::calcPlayerGuideOffsetY(const al::LiveActor* actor) {
    PlayerHackKeeper* hackKeeper = getHackKeeper(actor);
    if (hackKeeper && hackKeeper->getCurrentHackName())
        return getHackKeeper(actor)->getHackGuideHeight();

    PlayerInfo* info = getPlayerInfo(actor);
    PlayerCarryKeeper* carryKeeper = info ? info->getCarryKeeper() : nullptr;
    if (carryKeeper && carryKeeper->isCarry() && carryKeeper->isCarryUp())
        return 350.0f;

    info = getPlayerInfo(actor);
    HackCap* cap = info ? info->getHackCap() : nullptr;
    f32 offsetY = 200.0f;
    if (cap)
        return cap->isSeparateHide() ? 260.0f : 200.0f;
    return offsetY;
}

bool rs::isEnablePlayerHeadGuide(const al::LiveActor* actor) {
    PlayerHackKeeper* hackKeeper = getHackKeeper(actor);
    return !hackKeeper || !hackKeeper->getCurrentHackName() ||
           getHackKeeper(actor)->isHackGuideEnable();
}

void rs::calcPlayerGuidePos(sead::Vector3f* out, const al::LiveActor* actor) {
    calcPlayerGuidePos(out, actor, calcPlayerGuideOffsetY(actor));
}

void rs::calcPlayerGuidePos(sead::Vector3f* out, const al::LiveActor* actor, f32 offsetY) {
    out->set(getPlayerPos(actor));
    out->setAdd(sead::Vector3f::ey * offsetY, *out);
}

EquipmentInfo* PlayerEquipmentFunction::createEquipmentInfoRocketFlower(al::LiveActor* actor,
                                                                        s32 frame, f32 speed) {
    EquipmentInfo* info = new EquipmentInfo(actor);
    info->onForceDash(frame, speed);
    return info;
}

EquipmentInfo* PlayerEquipmentFunction::createEquipmentInfoKoopaCap(al::LiveActor* actor) {
    EquipmentInfo* info = new EquipmentInfo(actor);
    info->onNoCapThrow();
    return info;
}

PlayerEquipmentUser* PlayerEquipmentFunction::startEquip(al::HitSensor* equipmentSensor,
                                                         al::HitSensor* sourceSensor,
                                                         const EquipmentInfo* info) {
    PlayerInfo* playerInfo = getPlayerInfo(al::getSensorHost(equipmentSensor));
    PlayerEquipmentUser* user = playerInfo ? playerInfo->getEquipmentUser() : nullptr;
    user->startEquip(equipmentSensor, sourceSensor, info);
    return user;
}

void PlayerEquipmentFunction::endEquip(PlayerEquipmentUser** user) {
    (*user)->endEquip();
    *user = nullptr;
}

void PlayerEquipmentFunction::syncEquipVisibility(al::LiveActor* actor,
                                                  const PlayerEquipmentUser* user) {
    if (user->getModelChanger()->isHiddenModel())
        al::hideModelIfShow(actor);
    else
        al::showModelIfHide(actor);
}

bool PlayerEquipmentFunction::tryNoticeEquipPlayerDamage(PlayerEquipmentUser* user) {
    if (user->getEquipmentSensor())
        return user->noticeDamage();
    return false;
}

bool PlayerEquipmentFunction::isTriggerCapAction(const PlayerEquipmentUser* user) {
    return user->getPlayerInput()->isTriggerAction();
}

bool PlayerEquipmentFunction::isTriggerSwingLeft(const PlayerEquipmentUser* user) {
    return user->getPlayerInput()->isTriggerSwingLeftHand();
}

bool PlayerEquipmentFunction::isTriggerSwingRight(const PlayerEquipmentUser* user) {
    return user->getPlayerInput()->isTriggerSwingRightHand();
}

bool PlayerEquipmentFunction::isPlayerRolling(const PlayerEquipmentUser* user) {
    return user->getPlayerStateRolling()->isRolling();
}

bool PlayerEquipmentFunction::isEquipmentNoCapThrow(const PlayerEquipmentUser* user) {
    if (user->getEquipmentSensor())
        return user->getEquipmentInfo()->isNoCapThrow();
    return false;
}

bool PlayerEquipmentFunction::isEquipmentForceDash(const PlayerEquipmentUser* user) {
    if (user->getEquipmentSensor())
        return user->getEquipmentInfo()->isForceDash();
    return false;
}

bool PlayerEquipmentFunction::tryGetEquipmentForceDashInfo(s32* frame, f32* speed,
                                                           const PlayerEquipmentUser* user) {
    if (!user->getEquipmentSensor() || !user->getEquipmentInfo()->isForceDash())
        return false;

    const EquipmentInfo* info = user->getEquipmentInfo();
    *frame = info->getForceDashFrame();
    *speed = info->getForceDashSpeed();
    return true;
}
