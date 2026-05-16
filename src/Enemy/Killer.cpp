#include "Enemy/Killer.h"

#include "Enemy/EnemyStateDamageCap.h"
#include "Enemy/KillerStateHack.h"
#include "EnemyStateHackStart.h"
#include "Library/Base/StringUtil.h"
#include "Library/Bgm/BgmLineFunction.h"
#include "Library/Light/LightKeeper.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Se/SeFunction.h"
#include "Library/Shadow/ActorShadowUtil.h"
#include "System/GameDataFunction.h"
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

Killer::Killer(const char* name) : al::LiveActor(name) {}

void Killer::init(const al::ActorInitInfo& info) {
    if (!mIsMagnum)
        al::initActorWithArchiveName(this, info, "Killer", nullptr);
    else
        al::initActorWithArchiveName(this, info, "KillerMagnum", nullptr);

    al::initNerve(this, &NrvKiller.Fly, 3);
    mEnemyStateDamageCap = new EnemyStateDamageCap(this);

    if (!_137) {
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
    mKillerStateHack = new KillerStateHack(this, mIsMagnum, _138);
    al::initNerveState(this, mKillerStateHack, &NrvKiller.Hack, "キャプチャ");
    al::setSensorRadius(this, "Explosion", 0.0f);
    al::setSensorRadius(this, "ExplosionToPlayer", 0.0f);
    al::invalidateHitSensor(this, "Explosion");
    al::invalidateHitSensor(this, "ExplosionToPlayer");

    if (al::isExistPrePassLight(this, "Front") && al::isActivePrePassLight(this, "Front"))
        al::killPrePassLight(this, "Front", -1);

    al::setSeUserSyncParamPtr(this, &_130, "回転角");

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

    if (_13c < 1 && rs::sendMsgKillerAttackNoExplode(other, self)) {
        _13c = 40;

        return;
    }

    if (isHack() && mKillerStateHack->attackSensorCheckExplode(self, other))
        explode();

    if (al::isSensorName(self, "AttackMagnum")) {
        if (mIsMagnum)
            rs::sendMsgKillerMagnumAttack(other, self);

        return;
    }

    if (al::isNerve(this, &NrvKiller.Explode)) {
        if (al::isSensorPlayer(other)) {
            if (_134 && al::isSensorName(self, "ExplosionToPlayer"))
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
        if (!al::isNerve(this, &NrvKiller.AfterHack) && _134 &&
            al::sendMsgExplosion(other, self, nullptr)) {
            explode();

            return;
        }

        rs::sendMsgPushToPlayer(other, self);
    }
}

void Killer::explode() {
    if (isHack()) {
        if (!mKillerStateHack->isEnableExplode())
            return;

        mKillerStateHack->endHackExplode();
        al::endBgmSituation(this, "Capture", false);
        al::hideSilhouetteModelIfShow(this);
    }

    al::tryStopSe(this, "PgMoveEnemyMeLv", -1, nullptr);
    al::setNerve(this, &NrvKiller.Explode);
}

// bool Killer::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self)
// {}

void Killer::resetAliveCountAndAnim() {
    if (!_135)
        return;

    _11c = _118;
    al::startVisAnim(this, "HackOff");
    al::startMclAnim(this, "HackOff");
}

// void Killer::control() {}

void Killer::appearBy2D(const sead::Vector3f& position, const sead::Vector3f& velocity,
                        const sead::Quatf& rotation, s32 param_4) {
    al::resetPosition(this, position);
    al::setQuat(this, rotation);
    appearInit();
    al::setVelocity(this, velocity);
    al::startAction(this, "FlyWait");
    al::startVisAnim(this, "HackOff");
    EnemyStateHackFunction::endHackSwitchShadow(this, nullptr);
    _118 = param_4;
    _11c = _118;
    al::setNerve(this, &NrvKiller.Fly);
    appear();
    al::invalidateClipping(this);
}

void Killer::appearInit() {
    al::startVisAnim(this, "HackOff");
    al::startMclAnim(this, "HackOff");
    al::showModelIfHide(this);
    resetAliveCountAndAnim();

    _134 = true;
    _135 = true;
    al::calcFrontDir(&_124, this);
    _130 = 0.0;
    sead::Quatf quat;
    al::makeQuatFrontUp(&quat, _124, sead::Vector3f::ey);
    al::setQuat(this, quat);
    al::calcFrontDir(&_124, this);
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

void Killer::launch(s32 param_1) {
    _118 = param_1;
    _11c = _118;
    al::startAction(this, "FlyWaitStart");
    al::setNerve(this, &NrvKiller.Launch);
}

void Killer::forceExplode() {
    explode();
}

bool Killer::isHack() const {
    return al::isNerve(this, &NrvKiller.Hack);
}
