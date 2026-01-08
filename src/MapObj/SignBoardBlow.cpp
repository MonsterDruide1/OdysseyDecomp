#include "MapObj/SignBoardBlow.h"

#include <basis/seadTypes.h>
#include <math/seadQuat.h>

#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(SignBoardBlow, Wait);
NERVE_IMPL(SignBoardBlow, Blow);

NERVES_MAKE_NOSTRUCT(SignBoardBlow, Wait, Blow);
}  // namespace

SignBoardBlow::SignBoardBlow(const char* actorName, const char* signBoardBlowName)
    : al::LiveActor(actorName), mArchiveName(signBoardBlowName) {}

void SignBoardBlow::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, mArchiveName, nullptr);
    al::initNerve(this, &Wait, 0);
    makeActorDead();
}

bool SignBoardBlow::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                               al::HitSensor* self) {
    return false;
}

void SignBoardBlow::startBlow(const sead::Vector3f& attackerPos) {
    appear();
    al::startHitReaction(this, "出現");
    al::setVelocityBlowAttack(this, attackerPos, 18.0f, 38.0f);
    al::setNerve(this, &Blow);
    sead::Vector3f attackDir = al::getTrans(this) - attackerPos;
    attackDir.y = 0.0f;
    mRotationAxis = sead::Vector3f::ey.cross(attackDir);
    al::tryNormalizeOrDirZ(&mRotationAxis);
}

void SignBoardBlow::exeWait() {}

void SignBoardBlow::exeBlow() {
    al::addVelocityToGravity(this, 1.8f);
    f32 nerveRate = al::calcNerveRate(this, 0, 30);
    f32 expValue = al::lerpExponentValueEaseOut(0.0f, 400.0f, nerveRate, 2.0f);
    sead::Quatf rotatedQuat;
    al::makeQuatRotateDegree(&rotatedQuat, mRotationAxis, expValue);
    sead::Vector3f rotateDegree;
    al::calcQuatRotateDegree(&rotateDegree, rotatedQuat);
    al::setRotate(this, rotateDegree);

    if (al::isGreaterEqualStep(this, 2)) {
        sead::Vector3f upDir;
        al::calcUpDir(&upDir, this);
        if (alCollisionUtil::checkStrikeSphere(this, al::getTrans(this) + upDir * 0.0f, 80.0f,
                                               nullptr, nullptr)) {
            al::startHitReaction(this, "消滅");
            kill();
            return;
        }
    }

    if (al::isGreaterEqualStep(this, 30)) {
        al::startHitReaction(this, "消滅");
        kill();
    }
}
