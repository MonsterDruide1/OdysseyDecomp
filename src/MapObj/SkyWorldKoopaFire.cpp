#include "MapObj/SkyWorldKoopaFire.h"

#include "Library/Bgm/BgmLineFunction.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/Demo/DemoFunction.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

#include "Util/DemoUtil.h"
#include "Util/ItemUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
const al::IUseAudioKeeper* getAudioKeeperPtr(const SkyWorldKoopaFire* actor) {
    return actor;
}

NERVE_ACTION_IMPL(SkyWorldKoopaFire, Wait)
NERVE_ACTION_IMPL(SkyWorldKoopaFire, HighTension)

NERVE_ACTIONS_MAKE_STRUCT(SkyWorldKoopaFire, Wait, HighTension)

NERVE_IMPL(SkyWorldKoopaFrame, Wait)
NERVE_IMPL(SkyWorldKoopaFrame, Reaction)
NERVE_IMPL(SkyWorldKoopaFrame, Fall)
NERVE_IMPL(SkyWorldKoopaFrame, FallNoCollider)
NERVE_IMPL(SkyWorldKoopaFrame, FallEndWait)

NERVES_MAKE_NOSTRUCT(SkyWorldKoopaFrame, Wait, Reaction, Fall, FallNoCollider, FallEndWait)
}  // namespace

SkyWorldKoopaFire::SkyWorldKoopaFire(const char* actorName) : al::LiveActor(actorName) {}

void SkyWorldKoopaFire::init(const al::ActorInitInfo& info) {
    using SkyWorldKoopaFireFunctor =
        al::FunctorV0M<SkyWorldKoopaFire*, void (SkyWorldKoopaFire::*)()>;

    al::initNerveAction(this, "Wait", &NrvSkyWorldKoopaFire.collector, 0);
    al::initActor(this, info);
    al::onUpdateMovementEffectAudioCollisionSensor(this);
    al::setEffectNamedMtxPtr(this, "EffectEmitPos", &mEffectEmitPos);

    if (al::listenStageSwitchOnOffAppear(
            this, SkyWorldKoopaFireFunctor(this, &SkyWorldKoopaFire::listenAppear),
            SkyWorldKoopaFireFunctor(this, &SkyWorldKoopaFire::listenKill))) {
        makeActorDead();
    } else {
        makeActorAlive();
    }

    al::registActorToDemoInfo(this, info);
}

void SkyWorldKoopaFire::listenAppear() {
    mCapAttackCooldown = 0;
    appear();
    al::startNerveAction(this, "Wait");
}

void SkyWorldKoopaFire::listenKill() {
    kill();
}

void SkyWorldKoopaFire::exeWait() {
    mCapAttackCooldown -= mCapAttackCooldown > 0;

    if (al::isRunningBgm(getAudioKeeperPtr(this), "WorldMain2"))
        al::startNerveAction(this, "HighTension");
}

void SkyWorldKoopaFire::exeHighTension() {
    mCapAttackCooldown -= mCapAttackCooldown > 0;

    if (!al::isRunningBgm(getAudioKeeperPtr(this), "WorldMain2"))
        al::startNerveAction(this, "Wait");
}

void SkyWorldKoopaFire::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    al::sendMsgEnemyAttackFire(other, self, nullptr);
}

bool SkyWorldKoopaFire::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                   al::HitSensor* self) {
    if (al::isMsgPlayerDisregard(message))
        return true;

    if (!rs::isMsgCapAttack(message) || mCapAttackCooldown > 0)
        return false;

    mEffectEmitPos.setTranslation(al::getSensorPos(self));
    al::startHitReaction(this, "帽子ヒット");
    mCapAttackCooldown = 120;
    return false;
}

SkyWorldKoopaFrame::SkyWorldKoopaFrame(const char* actorName) : al::LiveActor(actorName) {
    mDemoCamera = nullptr;
    mShine = nullptr;
}

void SkyWorldKoopaFrame::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "SkyWorldHomeFrame", nullptr);
    al::initNerve(this, &Wait, 0);
    mDemoCamera = al::initObjectCamera(this, info, "デモカメラ", nullptr);
    mShine = rs::tryInitLinkShine(info, "ShineActor", 0);
    makeActorAlive();
}

bool SkyWorldKoopaFrame::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                    al::HitSensor* self) {
    if (!rs::isMsgCapAttackCollide(message) && !rs::isMsgPlayerRollingWallHitMove(message) &&
        !rs::isMsgCapHipDrop(message)) {
        return false;
    }

    if (!al::isNerve(this, &Wait))
        return false;

    al::setNerve(this, &Reaction);
    al::startHitReaction(this, "落下開始");
    return true;
}

void SkyWorldKoopaFrame::exeWait() {
    if (al::isFirstStep(this)) {
        al::validateClipping(this);
        al::offCollide(this);
    }
}

void SkyWorldKoopaFrame::exeReaction() {
    if (al::isFirstStep(this)) {
        if (!rs::requestStartDemoNormal(this, false)) {
            al::setNerve(this, &Reaction);
            return;
        }

        al::startAction(this, "Reaction");
        al::startCamera(this, mDemoCamera, -1);
        al::invalidateClipping(this);
    }

    if (al::isActionEnd(this))
        al::setNerve(this, &Fall);
}

void SkyWorldKoopaFrame::exeFall() {
    al::addVelocityToGravity(this, 1.8);
    al::scaleVelocity(this, 0.98);

    if (al::isStep(this, 40))
        al::onCollide(this);

    if (al::isCollidedGround(this)) {
        al::offCollide(this);
        al::setNerve(this, &FallNoCollider);
    }
}

void SkyWorldKoopaFrame::exeFallNoCollider() {
    al::addVelocityToGravity(this, 1.8);
    al::scaleVelocity(this, 0.98);

    if (al::isStep(this, 4)) {
        al::startHitReaction(this, "破壊");
        al::hideModel(this);
        al::setVelocityZero(this);
        al::offCollide(this);
        al::setNerve(this, &FallEndWait);
    }
}

void SkyWorldKoopaFrame::exeFallEndWait() {
    if (al::isGreaterEqualStep(this, 60)) {
        rs::requestEndDemoNormal(this);
        al::endCamera(this, mDemoCamera, -1, false);
        kill();
        rs::appearPopupShine(mShine);
    }
}
