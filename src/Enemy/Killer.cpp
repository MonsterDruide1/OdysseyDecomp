#include "Enemy/Killer.h"

#include "Library/Base/StringUtil.h"
#include "Library/Bgm/BgmLineFunction.h"
#include "Library/Item/ItemUtil.h"
#include "Library/Light/LightKeeper.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActorFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Player/PlayerUtil.h"
#include "Library/Se/SeFunction.h"
#include "Library/Shadow/ActorShadowUtil.h"

#include "Enemy/EnemyCap.h"
#include "Enemy/EnemyStateDamageCap.h"
#include "Enemy/EnemyStateHackStart.h"
#include "Enemy/KillerStateHack.h"
#include "System/GameDataFunction.h"
#include "Util/ItemUtil.h"
#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"
#include "Util/SpecialBuildUtil.h"

namespace {
NERVE_IMPL(Killer, Fly)
NERVE_IMPL(Killer, DamageCap)
NERVE_IMPL(Killer, Hack)
NERVE_IMPL(Killer, FallDown)
NERVE_IMPL(Killer, Explode)
NERVE_IMPL(Killer, AfterHack)
NERVE_IMPL(Killer, Appear)
NERVE_IMPL(Killer, StandBy)
NERVE_IMPL(Killer, Launch)

NERVES_MAKE_STRUCT(Killer, Fly, DamageCap, Hack, FallDown, Explode, AfterHack, Appear, StandBy,
                   Launch)
}  // namespace

const sead::Vector3f gLightOffset = {0.0f, 0.0f, 100.0f};

Killer::Killer(const char* name) : al::LiveActor(name) {}

void Killer::init(const al::ActorInitInfo& info) {
    if (mIsMagnum)
        al::initActorWithArchiveName(this, info, "KillerMagnum", nullptr);
    else
        al::initActorWithArchiveName(this, info, "Killer", nullptr);

    // NOTE: reserves space for 3 substates, but only adds 2
    al::initNerve(this, &NrvKiller.Fly, 3);
    mEnemyStateDamageCap = new EnemyStateDamageCap(this);

    if (!mIsNoCap) {
        if (mIsMagnum || al::isEqualString(GameDataFunction::getCurrentStageName(this),
                                           "MoonWorldCaptureParadeStage")) {
            mIsCapKoopa = true;
        }

        if (mIsCapKoopa) {
            mEnemyStateDamageCap->createEnemyCap(info, "EnemyCapKoopa");
            mEnemyStateDamageCap->makeActorDeadCap();
        } else {
            mEnemyStateDamageCap->createEnemyCap(info, "EnemyCapKiller");
            mEnemyStateDamageCap->makeActorDeadCap();
        }
    }

    al::initNerveState(this, mEnemyStateDamageCap, &NrvKiller.DamageCap, "帽子ふきとび");
    mStateHack = new KillerStateHack(this, mIsMagnum, mIsUseCaptureLight);
    al::initNerveState(this, mStateHack, &NrvKiller.Hack, "キャプチャ");
    al::setSensorRadius(this, "Explosion", 0.0f);
    al::setSensorRadius(this, "ExplosionToPlayer", 0.0f);
    al::invalidateHitSensor(this, "Explosion");
    al::invalidateHitSensor(this, "ExplosionToPlayer");

    if (al::isExistPrePassLight(this, "Front") && al::isActivePrePassLight(this, "Front"))
        al::killPrePassLight(this, "Front", -1);

    al::setSeUserSyncParamPtr(this, &mRotationAngle, "回転角");

    makeActorAlive();
}

void Killer::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isNerve(this, &NrvKiller.Appear) || al::isNerve(this, &NrvKiller.StandBy) ||
        al::isNerve(this, &NrvKiller.Launch)) {
        if (mIsMagnum)
            rs::sendMsgPushToPlayer(other, self);

        return;
    }

    if (al::isNerve(this, &NrvKiller.FallDown))
        return;

    if (mAttackCooldown < 1 && rs::sendMsgKillerAttackNoExplode(other, self)) {
        mAttackCooldown = 40;

        return;
    }

    if (isHack() && mStateHack->attackSensorCheckExplode(self, other))
        explode();

    if (al::isSensorName(self, "AttackMagnum")) {
        if (mIsMagnum)
            rs::sendMsgKillerMagnumAttack(other, self);

        return;
    }

    if (al::isNerve(this, &NrvKiller.Explode)) {
        if (al::isSensorPlayer(other)) {
            if (mCanExplode && al::isSensorName(self, "ExplosionToPlayer"))
                al::sendMsgExplosion(other, self, nullptr);
        } else if (al::isSensorName(self, "Explosion")) {
            al::sendMsgExplosion(other, self, nullptr);
        }

        return;
    }

    if (al::isSensorEnemyAttack(self)) {
        if (al::sendMsgExplosion(other, self, nullptr) && !mIsMagnum) {
            explode();

            return;
        }

        if (al::sendMsgPush(other, self)) {
            if (!mIsMagnum)
                explode();

            return;
        }
    }

    if (al::isSensorEnemyAttack(self) && al::isSensorPlayer(other)) {
        if (!al::isNerve(this, &NrvKiller.AfterHack) && mCanExplode &&
            al::sendMsgExplosion(other, self, nullptr)) {
            explode();

            return;
        }

        rs::sendMsgPushToPlayer(other, self);
    }
}

void Killer::explode() {
    if (isHack()) {
        if (!mStateHack->isEnableExplode())
            return;

        mStateHack->endHackExplode();
        al::endBgmSituation(this, "Capture", false);
        al::hideSilhouetteModelIfShow(this);
    }

    al::tryStopSe(this, "PgMoveEnemyMeLv", -1, nullptr);
    al::setNerve(this, &NrvKiller.Explode);
}

bool Killer::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    if ((al::isNerve(this, &NrvKiller.Appear) || al::isNerve(this, &NrvKiller.StandBy) ||
         al::isNerve(this, &NrvKiller.Launch) || al::isNerve(this, &NrvKiller.Explode) ||
         al::isNerve(this, &NrvKiller.FallDown)) &&
        rs::isMsgPlayerDisregardHomingAttack(message)) {
        return true;
    }

    if (rs::isMsgCapCancelLockOn(message))
        return true;

    if (al::isDead(this))
        return false;

    if (al::isNerve(this, &NrvKiller.Appear) || al::isNerve(this, &NrvKiller.StandBy) ||
        al::isNerve(this, &NrvKiller.Launch)) {
        if (mIsMagnum && rs::isMsgCapReflect(message)) {
            rs::requestHitReactionToAttacker(message, self, other);

            return true;
        }

        return false;
    }

    if (al::isNerve(this, &NrvKiller.Explode))
        return false;

    if (al::isNerve(this, &NrvKiller.FallDown))
        return false;

    if (al::isSensorName(self, "AttackMagnum"))
        return false;

    if (isHack()) {
        if (mStateHack->receiveMsgHackEnd(message, other, self)) {
            if (rs::isModeE3MovieRom())
                EnemyStateHackFunction::endHackSwitchShadow(this, nullptr);

            mHackEndImmuneTime = 20;
            al::endBgmSituation(this, "Capture", false);
            al::hideSilhouetteModelIfShow(this);
            mCanExplode = false;

            if (!al::isNerve(this, &NrvKiller.Explode))
                al::setNerve(this, &NrvKiller.AfterHack);

            return true;
        }

        if (mStateHack->receiveMsgHackEndExplode(message, other, self)) {
            if (!mIsMagnum && mStateHack->isEnableExplode()) {
                mCanExplode = false;
                explode();
            }

            return true;
        }
    }

    if ((rs::isMsgCapStartLockOn(message) || rs::isMsgCapKeepLockOn(message)) &&
        !mEnemyStateDamageCap->isCapOn()) {
        if (!al::isNerve(this, &NrvKiller.DamageCap) || al::isGreaterEqualStep(this, 8))
            return true;
    }

    if (mStateHack->receiveMsgInitCapTargetInfo(message))
        return true;

    if (mStateHack->receiveMsgHackStart(message, other, self)) {
        al::onCollide(this);
        al::setVelocityZero(this);
        resetAliveCountAndAnim();
        al::startBgmSituation(this, "Capture", false);

        if (mIsFirstCapture) {
            mExplosionTime = mIsMagnum ? 1850.0f : 900.0f;
            al::startVisAnim(this, "HackOff");
            al::startMclAnim(this, "HackOff");
            mIsFirstCapture = false;
        }

        al::showModelIfHide(this);
        al::setNerve(this, &NrvKiller.Hack);

        return true;
    }

    if (rs::isMsgKillerMagnumAttack(message)) {
        if (!isHack() || !mIsMagnum)
            explode();

        return true;
    }

    if (rs::isMsgCactusNeedleAttack(message)) {
        if (!mIsMagnum)
            explode();

        return false;
    }

    if (rs::isMsgSandGeyserRaise(message)) {
        if (!mIsMagnum)
            explode();

        return true;
    }

    if (al::isMsgExplosion(message) || rs::isMsgBlowDown(message)) {
        rs::requestHitReactionToAttacker(message, self, other);

        if (!mIsMagnum)
            explode();

        return true;
    }

    if (mIsMagnum && al::tryReceiveMsgPushAndAddVelocityH(this, message, other, self, 1.0f))
        return true;

    if (isHack())
        return mStateHack->receiveMsg(message, other, self);

    if (mStateHack->receiveMsgNpcScareByEnemy(message))
        return true;

    if (rs::isMsgKillerMagnumHackAttack(message)) {
        rs::requestHitReactionToAttacker(message, self, other);
        rs::setAppearItemFactorAndOffsetByMsg(this, message, other);
        explode();
        al::appearItem(this);

        return true;
    }

    if (mEnemyStateDamageCap->tryReceiveMsgCapBlow(message, other, self)) {
        al::setNerve(this, &NrvKiller.DamageCap);

        return true;
    }

    if (rs::isMsgKillByShineGet(message)) {
        kill();

        return true;
    }

    if (rs::isMsgMeganeAttack(message)) {
        explode();

        return true;
    }

    if ((rs::isMsgPlayerAndCapObjHipDropReflectAll(message) || rs::isMsgPressDown(message)) &&
        mHackEndImmuneTime <= 0) {
        rs::requestHitReactionToAttacker(message, self, other);
        rs::setAppearItemFactorAndOffsetByMsg(this, message, other);
        al::setNerve(this, &NrvKiller.FallDown);

        return true;
    }

    return false;
}

void Killer::resetAliveCountAndAnim() {
    if (!mIsFirstCapture)
        return;

    mExplosionTime = mAliveFrame;
    al::startVisAnim(this, "HackOff");
    al::startMclAnim(this, "HackOff");
}

void Killer::control() {
    if (!isHack() && mIsUseCaptureLight) {
        if (al::isAlive(al::getSubActor(this, "キラーアイライト")))
            al::getSubActor(this, "キラーアイライト")->kill();

        if (al::isExistPrePassLight(this, "Front") && al::isActivePrePassLight(this, "Front"))
            al::killPrePassLight(this, "Front", -1);
    }

    if (mAttackCooldown > 0)
        mAttackCooldown--;

    sead::Vector3f front;
    al::calcFrontDir(&front, this);

    // adds roll based on current horizontal rotation speed
    if (!isHack() || !mStateHack->isStarting()) {
        f32 angle = al::calcAngleOnPlaneDegree(front, mLastFrontDir, sead::Vector3f::ey);
        f32 rate;
        if (!mIsMagnum)
            rate = 1.2f;
        else if (isHack())
            rate = 2.0f;
        else
            rate = 0.7f;

        angle *= rate;

        mRotationAngle = (mRotationAngle + angle) * 0.9f;
        sead::Quatf quat;
        al::makeQuatFrontUp(&quat, front, sead::Vector3f::ey);
        al::setQuat(this, quat);
        al::rotateQuatLocalDirDegree(this, 2 /* axis Z */, mRotationAngle);
    }

    mLastFrontDir.set(front);

    if (mHackEndImmuneTime > 0)
        mHackEndImmuneTime--;

    if (al::isNerve(this, &NrvKiller.Explode) || al::isNerve(this, &NrvKiller.FallDown) ||
        al::isNerve(this, &NrvKiller.Appear) || al::isNerve(this, &NrvKiller.StandBy) ||
        (isHack() && mStateHack->isStarting()))
        return;

    if (isHack()) {
        if (mExplosionTime > 180.0f)
            if (!mIsMagnum)
                mExplosionTime -= sead::Mathf::min(al::getVelocity(this).length() / 10.8f, 3.8f);
            else
                mExplosionTime -= sead::Mathf::min(al::getVelocity(this).length() / 18.4f, 3.15f);
        else
            mExplosionTime--;

        if (mIsUseCaptureLight) {
            sead::Vector3f trans;
            al::calcTransLocalOffset(&trans, this, gLightOffset);
            al::setTrans(al::getSubActor(this, "キラーアイライト"), trans);
            al::setQuat(al::getSubActor(this, "キラーアイライト"), al::getQuat(this));
            if (al::isExistPrePassLight(this, "Front") && !al::isActivePrePassLight(this, "Front"))
                al::appearPrePassLight(this, "Front", -1);
        }
    } else {
        mExplosionTime--;
    }

    if (mExplosionTime <= 180.0f) {
        al::tryStartMclAnimIfNotPlaying(this, "SignExplosion");

        if (isHack())
            al::holdSe(this, "ExplodeSign");
    }

    if (mExplosionTime <= 0.0f)
        explode();
}

void Killer::appearBy2D(const sead::Vector3f& position, const sead::Vector3f& velocity,
                        const sead::Quatf& rotation, s32 aliveFrame) {
    al::resetPosition(this, position);
    al::setQuat(this, rotation);
    appearInit();
    al::setVelocity(this, velocity);
    al::startAction(this, "FlyWait");
    al::startVisAnim(this, "HackOff");
    EnemyStateHackFunction::endHackSwitchShadow(this, nullptr);
    mAliveFrame = aliveFrame;
    mExplosionTime = mAliveFrame;
    al::setNerve(this, &NrvKiller.Fly);
    appear();
    al::invalidateClipping(this);
}

void Killer::appearInit() {
    al::startVisAnim(this, "HackOff");
    al::startMclAnim(this, "HackOff");
    al::showModelIfHide(this);
    resetAliveCountAndAnim();

    mCanExplode = true;
    mIsFirstCapture = true;
    al::calcFrontDir(&mLastFrontDir, this);
    mRotationAngle = 0.0f;
    sead::Quatf quat;
    al::makeQuatFrontUp(&quat, mLastFrontDir, sead::Vector3f::ey);
    al::setQuat(this, quat);
    al::calcFrontDir(&mLastFrontDir, this);
    al::hideSilhouetteModelIfShow(this);
}

void Killer::standByAppear(const sead::Vector3f& position, const sead::Quatf& rotation) {
    al::resetPosition(this, position);
    al::setQuat(this, rotation);
    appearInit();
    mEnemyStateDamageCap->resetCap();
    al::startAction(this, "Appear");
    al::startVisAnim(this, "HackOff");

    if (rs::isModeE3MovieRom()) {
        al::invalidateShadow(this);
        al::offDepthShadowModel(this);
        al::validateDepthShadowMap(this);
    } else {
        EnemyStateHackFunction::endHackSwitchShadow(this, nullptr);
    }

    al::setNerve(this, &NrvKiller.Appear);
    appear();
    al::invalidateClipping(this);
}

void Killer::launch(s32 aliveFrame) {
    mAliveFrame = aliveFrame;
    mExplosionTime = mAliveFrame;
    al::startAction(this, "FlyWaitStart");
    al::setNerve(this, &NrvKiller.Launch);
}

void Killer::forceExplode() {
    explode();
}

bool Killer::isHack() const {
    return al::isNerve(this, &NrvKiller.Hack);
}

void Killer::exeAppear() {
    if (al::isFirstStep(this)) {
        al::offCollide(this);

        if (mIsCapKoopa && !mIsNoCap)
            al::hideModelIfShow(mEnemyStateDamageCap->getEnemyCap());
    }

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvKiller.StandBy);
}

void Killer::exeStandBy() {
    if (al::isFirstStep(this))
        al::startAction(this, "StandBy");
}

void Killer::exeLaunch() {
    if (al::isFirstStep(this) && mIsCapKoopa && !mIsNoCap) {
        al::showModelIfHide(mEnemyStateDamageCap->getEnemyCap());
        al::startAction(mEnemyStateDamageCap->getEnemyCap(), "AppearKillerMagnum");
    }

    applyVelocityDamp();

    if (al::isGreaterEqualStep(this, mIsMagnum ? 40 : 25)) {
        al::onCollide(this);
        al::setNerve(this, &NrvKiller.Fly);
    }
}

void Killer::applyVelocityDamp() {
    al::addVelocityToFront(this, mIsMagnum ? 1.72f : 0.63f);
    al::scaleVelocity(this, mIsMagnum ? 0.90f : 0.95f);
}

static bool isExplodeCollision(Killer* killer, bool isMagnum) {
    al::HitSensor* wallSensor = al::tryGetCollidedWallSensor(killer);
    if (wallSensor) {
        if (al::getCollidedWallNormal(killer).dot(al::getVelocity(killer)) > -0.2f)
            return false;

        if (al::sendMsgExplosion(wallSensor, al::getHitSensor(killer, "Attack"), nullptr) &&
            isMagnum)
            return false;

        return true;
    }

    if (al::isOnGround(killer, 0) &&
        al::calcAngleDegree(sead::Vector3f::ey, al::getCollidedGroundNormal(killer)) > 44.0f) {
        al::sendMsgExplosion(al::getCollidedGroundSensor(killer),
                             al::getHitSensor(killer, "Attack"), nullptr);

        return true;
    }

    return false;
}

void Killer::exeFly() {
    if (al::isFirstStep(this)) {
        mCanExplode = true;
        al::startAction(this, "FlyWait");
        al::tryStartSe(this, "PgMoveEnemyMeLv");
    }

    al::LiveActor* player = al::tryFindNearestPlayerActor(this);
    if (player && !rs::isPlayer2D(this)) {
        const sead::Vector3f& playerTrans = al::getTrans(player);
        f32 distance = al::calcDistance(this, playerTrans);
        f32 turnDegrees = mIsMagnum ? 0.4f : 2.4f;

        if (distance < (mIsMagnum ? 700.0f : 1100.0f))
            turnDegrees *= distance / (mIsMagnum ? 700.0f : 1100.0f);

        al::turnToTarget(this, playerTrans, turnDegrees);
    }

    applyVelocityDamp();

    if (isExplodeCollision(this, mIsMagnum))
        explode();
}

void Killer::exeExplode() {
    if (al::isFirstStep(this)) {
        al::hideSilhouetteModelIfShow(this);
        al::validateHitSensor(this, "Explosion");
        al::validateHitSensor(this, "ExplosionToPlayer");
        al::setVelocityZero(this);
        al::startAction(this, "Explosion");
        al::hideModelIfShow(this);
    }

    f32 rate = al::calcNerveRate(this, 5);
    al::setSensorRadius(this, "Explosion", al::lerpValue(0.0f, 200.0f, rate));
    al::setSensorRadius(this, "ExplosionToPlayer", al::lerpValue(0.0f, 100.0f, rate));

    if (al::isGreaterEqualStep(this, 5)) {
        mEnemyStateDamageCap->makeActorDeadCap();
        al::setSensorRadius(this, "Explosion", 0.0f);
        al::setSensorRadius(this, "ExplosionToPlayer", 0.0f);
        al::invalidateHitSensor(this, "Explosion");
        al::invalidateHitSensor(this, "ExplosionToPlayer");
        kill();
    }
}

void Killer::exeDamageCap() {
    if (al::isFirstStep(this))
        al::setVelocityZero(this);

    if (al::updateNerveState(this) && (!mIsMagnum || al::isGreaterEqualStep(this, 50)))
        al::setNerve(this, &NrvKiller.Fly);
}

void Killer::exeHack() {
    if (al::isFirstStep(this))
        mCanExplode = false;

    if (al::updateNerveState(this)) {
        explode();

        return;
    }

    if (isExplodeCollision(this, mIsMagnum)) {
        al::startVisAnim(this, "HackOff");
        al::startMclAnim(this, "HackOff");
        explode();
    }
}

void Killer::exeAfterHack() {
    if (al::isFirstStep(this)) {
        al::startVisAnim(this, "HackOff");
        al::startMclAnim(this, "Default");
        al::startAction(this, "HackEnd");
        al::tryStartSe(this, "PgMoveEnemyMeLv");
        EnemyStateHackFunction::endHackSwitchShadow(this, nullptr);
        al::invalidateClipping(this);
    }

    applyVelocityDamp();

    if (al::isActionPlaying(this, "HackEnd") && al::isActionEnd(this))
        al::startAction(this, "FlyWait");

    if (al::isGreaterEqualStep(this, mIsMagnum ? 180 : 280)) {
        mCanExplode = true;
        al::setNerve(this, &NrvKiller.Fly);
    } else if (isExplodeCollision(this, mIsMagnum)) {
        explode();
    }
}

void Killer::exeFallDown() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "FallDown");
        al::setVelocityZero(this);
        al::addVelocityToGravity(this, -20.0f);
    }

    al::addVelocityToGravity(this, 1.75f);
    al::scaleVelocity(this, mIsMagnum ? 0.90f : 0.95f);

    if (al::isActionEnd(this) || al::isOnGround(this, 0)) {
        al::startHitReaction(this, "死亡");
        al::appearItem(this);
        mEnemyStateDamageCap->makeActorDeadCap();
        kill();
    }
}
