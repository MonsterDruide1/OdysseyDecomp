#include "Enemy/GamaneBullet.h"

#include <math/seadVector.h>

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(GamaneBullet, Straight)

class GamaneBulletNrvHit : public al::Nerve {
public:
    void execute(al::NerveKeeper* keeper) const override {
        GamaneBullet* bullet = keeper->getParent<GamaneBullet>();
        al::startHitReaction(bullet, "命中");
        bullet->kill();
    }
};

NERVE_IMPL(GamaneBullet, Brake)
NERVE_IMPL(GamaneBullet, Fall)

NERVES_MAKE_NOSTRUCT(GamaneBullet, Straight, Hit, Brake, Fall)
}  // namespace

GamaneBullet::GamaneBullet(const char* name, const al::LiveActor* parent) : al::LiveActor(name) {
    mParentActor = parent;
}

void GamaneBullet::init(const al::ActorInitInfo& info) {
    al::initNerve(this, &Straight, 0);
    al::initActorWithArchiveName(this, info, "GamaneBullet", nullptr);
    makeActorDead();
}

void GamaneBullet::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isNerve(this, &Hit))
        return;

    if (al::isSensorMapObj(other)) {
        if (rs::sendMsgGamaneBullet(other, self)) {
            al::startHitReaction(this, "オブジェヒット");
            al::setNerve(this, &Hit);
            return;
        }

        if (rs::sendMsgWeaponItemGet(other, self))
            return;
    }

    if (rs::sendMsgGamaneBulletThrough(other, self)) {
        al::startHitReaction(this, "オブジェヒット");
        return;
    }

    if (rs::sendMsgGamaneBulletForCoinFlower(other, self)) {
        al::startHitReaction(this, "コインで伸びる草ヒット");
        kill();
        return;
    }

    if (rs::sendMsgGamaneBullet(other, self) || rs::sendMsgBreakPartsBreak(other, self)) {
        al::startHitReaction(this, "オブジェヒット");
        al::setNerve(this, &Hit);
    }
}

bool GamaneBullet::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                              al::HitSensor* self) {
    if (al::isNerve(this, &Hit))
        return false;

    if (al::isMsgPlayerDisregard(message) || rs::isMsgPlayerDisregardHomingAttack(message))
        return true;

    return rs::isMsgPlayerDisregardTargetMarker(message);
}

void GamaneBullet::appear() {
    al::LiveActor::appear();
    al::setNerve(this, &Straight);
}

void GamaneBullet::control() {
    if (al::isNerve(this, &Hit))
        return;

    if (al::isInDeathArea(this) || al::isCollidedFloorCode(this, "DamageFire") ||
        al::isCollidedFloorCode(this, "Needle") || al::isCollidedFloorCode(this, "Poison")) {
        al::startHitReaction(this, "消滅");
        kill();
        return;
    }

    if (al::isCollided(this)) {
        al::HitSensor* collidedSensor = al::tryGetCollidedSensor(this);
        if (collidedSensor) {
            if (rs::sendMsgHackAttack(collidedSensor, al::getHitSensor(this, "Body")) ||
                rs::sendMsgGamaneBullet(collidedSensor, al::getHitSensor(this, "Body"))) {
                al::startHitReaction(this, "オブジェヒット");
            }
        }

        if (al::isCollided(this))
            al::setNerve(this, &Hit);
    }
}

void GamaneBullet::shoot(f32 speed, const sead::Vector3f& direction) {
    sead::Vector3f velocity;
    velocity.x = direction.x * speed + 0.0f;
    velocity.y = direction.y * speed + 0.0f;
    velocity.z = direction.z * speed + 0.0f;
    al::setVelocity(this, velocity);
    al::invalidateClipping(this);
    al::makeQuatFrontUp(al::getQuatPtr(this), sead::Vector3f::ey, direction);
    mRotateDegree = 60.0f;
    appear();
}

void GamaneBullet::rotate(f32 degree) {
    sead::Vector3f up = sead::Vector3f::ez;
    al::calcUpDir(&up, this);
    al::rotateVectorDegreeY(&up, mRotateDegree);
    al::makeQuatFrontUp(al::getQuatPtr(this), sead::Vector3f::ey, up);
    f32 nextRotateDegree = mRotateDegree - degree;
    mRotateDegree = nextRotateDegree < 0.0f ? 0.0f : nextRotateDegree;
}

void GamaneBullet::exeStraight() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");

    sead::Vector3f up = sead::Vector3f::ez;
    al::calcUpDir(&up, this);
    al::rotateVectorDegreeY(&up, mRotateDegree);
    al::makeQuatFrontUp(al::getQuatPtr(this), sead::Vector3f::ey, up);
    f32 nextRotateDegree = mRotateDegree - 0.5f;
    mRotateDegree = nextRotateDegree < 0.0f ? 0.0f : nextRotateDegree;

    if (al::isGreaterEqualStep(this, 15))
        al::setNerve(this, &Brake);
}

void GamaneBullet::exeBrake() {
    sead::Vector3f up = sead::Vector3f::ez;
    al::calcUpDir(&up, this);
    al::rotateVectorDegreeY(&up, mRotateDegree);
    al::makeQuatFrontUp(al::getQuatPtr(this), sead::Vector3f::ey, up);
    f32 nextRotateDegree = mRotateDegree - 2.5f;
    mRotateDegree = nextRotateDegree < 0.0f ? 0.0f : nextRotateDegree;
    al::scaleVelocityHV(this, 0.98f, 0.96f);

    if (al::isGreaterEqualStep(this, 10))
        al::setNerve(this, &Fall);
}

void GamaneBullet::exeFall() {
    sead::Vector3f up = sead::Vector3f::ez;
    al::calcUpDir(&up, this);
    al::rotateVectorDegreeY(&up, mRotateDegree);
    al::makeQuatFrontUp(al::getQuatPtr(this), sead::Vector3f::ey, up);
    f32 nextRotateDegree = mRotateDegree - 2.5f;
    mRotateDegree = nextRotateDegree < 0.0f ? 0.0f : nextRotateDegree;
    al::addVelocityY(this, -0.7f);

    if (al::isGreaterEqualStep(this, 300))
        kill();
}

void GamaneBullet::exeHit() {
    al::startHitReaction(this, "命中");
    kill();
}
