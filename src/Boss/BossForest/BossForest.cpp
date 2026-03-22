#include "Boss/BossForest/BossForest.h"

#include <prim/seadSafeString.h>

#include "Library/Base/StringUtil.h"
#include "Library/Bgm/BgmLineFunction.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/Joint/JointControllerKeeper.h"
#include "Library/Joint/JointLookAtController.h"
#include "Library/Joint/JointSpringControllerHolder.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/PostProcessing/CameraBlurController.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "Boss/BossForest/BossForestBarrierCtrl.h"
#include "Boss/BossForest/BossForestBattlePhase.h"
#include "Boss/BossForest/BossForestCameraController.h"
#include "Boss/BossForest/BossForestSatelliteController.h"
#include "Boss/BossForest/BossForestSatelliteHolder.h"
#include "Boss/BossForest/BossForestStateDemoBattleEnd.h"
#include "Boss/BossForest/BossForestStateDemoBattleStart.h"
#include "Boss/BossForest/BossForestStateWaitBattleStart.h"
#include "Boss/BossUtil/BossUtil.h"
#include "Enemy/Senobi.h"
#include "Enemy/SenobiGeneratePoint.h"
#include "MapObj/CapMessageShowInfo.h"
#include "Util/SensorMsgFunction.h"

namespace BossForestLocalFunction {
void createCollisionParts(BossForest*, const al::ActorInitInfo&, sead::Matrix34f*);
void setVisAnimDomeDamage(al::LiveActor*, f32);
void appearLifeParts(al::LiveActor*);
void setupLifePartsEffectMtx(BossForest*, const sead::Matrix34f*, s32);
void offEyesJointControl(BossForest*);
void onEyesJointControl(BossForest*);
s32 countActiveSatellitesNum(const BossForest*);
}  // namespace BossForestLocalFunction

namespace {
NERVE_HOST_TYPE_IMPL(BossForest, Die)
NERVE_HOST_TYPE_IMPL(BossForest, WaitForBattleStart)
NERVE_HOST_TYPE_IMPL(BossForest, BattlePhase1)
NERVE_HOST_TYPE_IMPL(BossForest, BattlePhase2)
NERVE_HOST_TYPE_IMPL(BossForest, BattlePhase3)
NERVE_HOST_TYPE_IMPL(BossForest, DemoBattleStart)
NERVE_HOST_TYPE_IMPL(BossForest, DemoBattleEnd)

NERVES_MAKE_NOSTRUCT(HostType, Die)
NERVES_MAKE_STRUCT(HostType, WaitForBattleStart, BattlePhase1, BattlePhase2, BattlePhase3,
                   DemoBattleStart, DemoBattleEnd)
}  // namespace

BossForest::BossForest(const char* name) : al::LiveActor(name) {}

void BossForest::init(const al::ActorInitInfo& initInfo) {
    al::initActorWithArchiveName(this, initInfo, sead::SafeString("BossForest"), nullptr);
    mIsLv2 = al::isObjectName(initInfo, "BossForestLv2");
    BossForestLocalFunction::createCollisionParts(this, initInfo, &mCollisionMtx);
    al::getTrans(&mInitTrans, initInfo);

    sead::Vector3f displayOffset = sead::Vector3f::zero;
    al::tryGetDisplayOffset(&displayOffset, initInfo);
    mInitTrans += displayOffset;
    al::setTrans(this, mInitTrans);

    mBattleEyePos = sead::Vector3f::ey * 700.0f + mInitTrans;

    al::initJointControllerKeeper(this, 5);
    mJointLookAtController = al::initJointLookAtController(this, 1);
    mJointLookAtController->mIsFlipped = true;

    al::appendJointLookAtControllerNoJudge(mJointLookAtController, this, "Head", 0.1f,
                                           {-179.0f, 180.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f},
                                           {1.0f, 0.0f, 0.0f});

    mJointSpringControllerHolder = new al::JointSpringControllerHolder();
    mJointSpringControllerHolder->init(this, "InitJointSpringCtrl");

    mCameraTargetPos.set(mBattleEyePos);

    mCameraController = new BossForestCameraController(this, initInfo, &mCameraTargetPos);

    mSatelliteHolder = new BossForestSatelliteHolder(this);
    mSatelliteHolder->init(initInfo);

    mBarrierCtrl = new BossForestBarrierCtrl(this);

    al::initNerve(this, &NrvHostType.WaitForBattleStart, 8);

    mBattlePhase1 = new BossForestBattlePhase(this, initInfo, 0);
    al::initNerveState(this, mBattlePhase1, &NrvHostType.BattlePhase1, u8"バトルフェーズ1");

    mBattlePhase2 = new BossForestBattlePhase(this, initInfo, 1);
    al::initNerveState(this, mBattlePhase2, &NrvHostType.BattlePhase2, u8"バトルフェーズ2");

    mBattlePhase3 = new BossForestBattlePhase(this, initInfo, 2);
    al::initNerveState(this, mBattlePhase3, &NrvHostType.BattlePhase3, u8"バトルフェーズ3");

    mStateWaitBattleStart = new BossForestStateWaitBattleStart(this, initInfo);
    al::initNerveState(this, mStateWaitBattleStart, &NrvHostType.WaitForBattleStart,
                       u8"バトル開始前");

    mCameraTicket = al::initDemoAnimCamera(this, initInfo, "Anim");

    mStateDemoBattleStart = new BossForestStateDemoBattleStart(this, initInfo, mCameraTicket);
    al::initNerveState(this, mStateDemoBattleStart, &NrvHostType.DemoBattleStart,
                       u8"バトル開始デモ");

    al::initNerveState(this, new BossForestStateDemoBattleEnd(this, initInfo, mCameraTicket),
                       &NrvHostType.DemoBattleEnd, u8"バトル終了デモ");

    s32 senobiCount = al::calcLinkChildNum(initInfo, "SenobiGeneratePoint");
    if (senobiCount > 0)
        mSenobiGeneratePoints.allocBuffer(senobiCount, nullptr);
    for (s32 i = 0; i < senobiCount; i++) {
        SenobiGeneratePoint* point = (SenobiGeneratePoint*)al::createLinksActorFromFactory(
            initInfo, "SenobiGeneratePoint", i);
        point->getSenobi()->setBossGenerated(true);
        mSenobiGeneratePoints.pushBack(point);
    }

    f32 domeDamage = 0.0f;
    if (al::isNerve(this, &NrvHostType.WaitForBattleStart))
        domeDamage = 4.0f;
    else if (al::isNerve(this, &NrvHostType.DemoBattleStart) ||
             al::isNerve(this, &NrvHostType.BattlePhase1))
        domeDamage = 3.0f;
    else if (al::isNerve(this, &NrvHostType.BattlePhase2))
        domeDamage = 2.0f;
    else
        domeDamage = al::isNerve(this, &NrvHostType.BattlePhase3);

    BossForestLocalFunction::setVisAnimDomeDamage(this, domeDamage);
    BossForestLocalFunction::appearLifeParts(this);
    al::emitEffect(this, "HeadPetal", nullptr);

    BossForestLocalFunction::setupLifePartsEffectMtx(this, mLifePartsMtxArray.emplaceBack(), 0);
    BossForestLocalFunction::setupLifePartsEffectMtx(this, mLifePartsMtxArray.emplaceBack(), 1);
    BossForestLocalFunction::setupLifePartsEffectMtx(this, mLifePartsMtxArray.emplaceBack(), 2);

    al::setEffectNamedMtxPtr(this, "AttackRingBeamLaserGround", &mAttackRingMtx);

    s32 wallCount = al::calcLinkChildNum(initInfo, "ForestWorldBossGroundWall");
    if (wallCount <= 0) {
        makeActorAlive();
        return;
    }

    mFieldWalls.allocBuffer(wallCount, nullptr);
    for (s32 i = 0; i < wallCount; i++) {
        al::LiveActor* wall =
            al::createLinksActorFromFactory(initInfo, "ForestWorldBossGroundWall", i);
        mFieldWalls.pushBack(wall);
    }
    makeActorAlive();
}

bool BossForest::isFirstDemo() const {
    return !rs::isAlreadyShowDemoBossBattleStart(this, 5, 1);
}

bool BossForest::isEnableSkipDemo() const {
    return true;
}

void BossForest::skipDemo() {
    if (al::isNerve(this, &NrvHostType.DemoBattleStart))
        return mStateDemoBattleStart->skipDemo();
}

void BossForest::updateSatellitesPose(const sead::Quatf& quat) {
    f32 rotSpeed = getCurrentSatelliteController()->getRotAngle();

    sead::Vector3f upDir;
    al::calcUpDir(&upDir, this);
    al::rotateQuatRadian(al::getQuatPtr(this), quat, upDir, sead::Mathf::deg2rad(rotSpeed));
}

BossForestSatelliteController* BossForest::getCurrentSatelliteController() const {
    if (al::isNerve(this, &NrvHostType.BattlePhase1))
        return mBattlePhase1->getSatelliteController();
    if (al::isNerve(this, &NrvHostType.BattlePhase2))
        return mBattlePhase2->getSatelliteController();
    if (al::isNerve(this, &NrvHostType.BattlePhase3))
        return mBattlePhase3->getSatelliteController();
    return nullptr;
}

SenobiGeneratePoint* BossForest::getSenobiGeneratePoint(s32 index) const {
    if ((u32)mSenobiGeneratePoints.size() <= (u32)index)
        return nullptr;
    return mSenobiGeneratePoints.at(index);
}

sead::PtrArray<al::LiveActor>* BossForest::getFieldWalls() {
    return &mFieldWalls;
}

void BossForest::control() {
    mCameraController->updateNerve();
    mBarrierCtrl->updateNerve();
    if (mHasCollisionParts) {
        sead::Matrix34f* dest = &mCollisionMtx;
        sead::Matrix34f* jointMtx = al::getJointMtxPtr(this, "JointRoot");
        *dest = *jointMtx;
    }
}

void BossForest::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    bool isAttackable = isStateBodyAttack();

    if (isAttackable && al::isSensorEnemyAttack(self) && !al::isSensorName(self, "LineBeam")) {
        if (al::isSensorName(self, "AttackFront")) {
            sead::Vector3f upDir;
            al::calcUpDir(&upDir, this);
            if (!al::isHitPlaneSensor(other, self, upDir, 150.0f))
                return;
        }
        al::sendMsgEnemyAttack(other, self);
    }

    if (al::isNerve(this, &NrvHostType.BattlePhase1))
        mBattlePhase1->attackSensor(self, other);
    if (al::isNerve(this, &NrvHostType.BattlePhase2))
        mBattlePhase2->attackSensor(self, other);
    if (al::isNerve(this, &NrvHostType.BattlePhase3))
        mBattlePhase3->attackSensor(self, other);

    if (al::isSensorName(self, "EyePush") || al::isSensorName(self, "EyePushL") ||
        al::isSensorName(self, "EyePushR")) {
        if (!al::sendMsgPush(other, self))
            rs::sendMsgPushToPlayer(other, self);
    }
}

bool BossForest::isStateBodyAttack() const {
    if (al::isNerve(this, &NrvHostType.BattlePhase1))
        return mBattlePhase1->isStateAttackable();
    if (al::isNerve(this, &NrvHostType.BattlePhase2))
        return mBattlePhase2->isStateAttackable();
    if (al::isNerve(this, &NrvHostType.BattlePhase3))
        return mBattlePhase3->isStateAttackable();
    return false;
}

void BossForest::setStateDemoBattleEnd() {
    al::endBgmSituation(this, "BossForestKnockOver", false);
    al::setNerve(this, &NrvHostType.DemoBattleEnd);
}

bool BossForest::receiveMsg(const al::SensorMsg* msg, al::HitSensor* self, al::HitSensor* other) {
    if (al::isNerve(this, &NrvHostType.WaitForBattleStart))
        return mStateWaitBattleStart->receiveMsg(msg, self, other);
    if (al::isNerve(this, &NrvHostType.BattlePhase1))
        return mBattlePhase1->receiveMsg(msg, self, other);
    if (al::isNerve(this, &NrvHostType.BattlePhase2))
        return mBattlePhase2->receiveMsg(msg, self, other);
    if (al::isNerve(this, &NrvHostType.BattlePhase3))
        return mBattlePhase3->receiveMsg(msg, self, other);
    return false;
}

sead::Matrix34f* BossForest::getLifePartsEffectMtx(const char* name) {
    if (al::isEqualString(name, u8"ライフパーツ00")) {
        if (mLifePartsMtxArray.size() != 0)
            return mLifePartsMtxArray.at(0);
    } else if (al::isEqualString(name, u8"ライフパーツ01")) {
        if ((u32)mLifePartsMtxArray.size() >= 2)
            return mLifePartsMtxArray.at(1);
    } else if (al::isEqualString(name, u8"ライフパーツ02")) {
        if ((u32)mLifePartsMtxArray.size() >= 3)
            return mLifePartsMtxArray.at(2);
    }
    return nullptr;
}

void BossForest::updateOnlyDemoGraphics() {}

void BossForest::exeWaitForBattleStart() {
    if (al::isFirstStep(this)) {
        f32 domeDamage;
        if (al::isNerve(this, &NrvHostType.WaitForBattleStart))
            domeDamage = 4.0f;
        else if (al::isNerve(this, &NrvHostType.DemoBattleStart) ||
                 al::isNerve(this, &NrvHostType.BattlePhase1))
            domeDamage = 3.0f;
        else if (al::isNerve(this, &NrvHostType.BattlePhase2))
            domeDamage = 2.0f;
        else
            domeDamage = al::isNerve(this, &NrvHostType.BattlePhase3);
        BossForestLocalFunction::setVisAnimDomeDamage(this, domeDamage);
    }
    al::updateNerveStateAndNextNerve(this, &NrvHostType.DemoBattleStart);
}

void BossForest::exeDemoBattleStart() {
    if (al::isFirstStep(this)) {
        al::tryOnStageSwitch(this, "SwitchBattleStartOn");
        f32 domeDamage;
        if (al::isNerve(this, &NrvHostType.WaitForBattleStart))
            domeDamage = 4.0f;
        else if (al::isNerve(this, &NrvHostType.DemoBattleStart) ||
                 al::isNerve(this, &NrvHostType.BattlePhase1))
            domeDamage = 3.0f;
        else if (al::isNerve(this, &NrvHostType.BattlePhase2))
            domeDamage = 2.0f;
        else
            domeDamage = al::isNerve(this, &NrvHostType.BattlePhase3);
        BossForestLocalFunction::setVisAnimDomeDamage(this, domeDamage);
        BossForestLocalFunction::offEyesJointControl(this);
        al::stopAllBgm(this, 5);
    }
    al::getNerveStep(this);
    if (al::updateNerveStateAndNextNerve(this, &NrvHostType.BattlePhase1)) {
        BossForestLocalFunction::onEyesJointControl(this);
        alCameraBlurFunction::setCameraBlurName(this, "BossCameraBlur");
        al::setTrans(this, mBattleEyePos);
        mCameraController->tryStartCamera();
    }
}

void BossForest::exeBattlePhase1() {
    if (!mIsHintShown && !mIsLv2 && al::isNerve(this, &NrvHostType.BattlePhase1) &&
        !al::isLessStep(this, 1800) &&
        BossForestLocalFunction::countActiveSatellitesNum(this) >= 3) {
        rs::showCapMessageBossHint(this, "BossForest_Hint", 90, 0);
        mIsHintShown = true;
    }
    if (al::updateNerveState(this))
        al::setNerve(this, &NrvHostType.BattlePhase2);
}

void BossForest::exeBattlePhase2() {
    if (al::updateNerveState(this))
        al::setNerve(this, &NrvHostType.BattlePhase3);
}

void BossForest::exeBattlePhase3() {
    if (al::updateNerveState(this)) {
        al::endBgmSituation(this, "BossForestKnockOver", false);
        al::setNerve(this, &NrvHostType.DemoBattleEnd);
    }
}

void BossForest::exeDemoBattleEnd() {
    if (al::isFirstStep(this)) {
        BossForestLocalFunction::offEyesJointControl(this);
        al::stopAllBgm(this, 5);
    }
    mCameraController->tryEndCamera();
    if (al::updateNerveStateAndNextNerve(this, &Die)) {
        for (s32 i = 0; i < mSenobiGeneratePoints.size(); i++)
            mSenobiGeneratePoints.unsafeAt(i)->forceKill();
        al::tryOffStageSwitch(this, "SwitchBossBattleOn");
        alCameraBlurFunction::resetCameraBlurName(this);
        kill();
    }
}

void BossForest::exeDie() {}
