#include "MapObj/MoonBasementGateParts.h"

#include <math/seadMatrix.h>

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Obj/BreakModel.h"
#include "Library/Obj/CollisionObj.h"
#include "Library/Obj/PartsFunction.h"

#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(MoonBasementGateParts, Wait)
NERVE_IMPL(MoonBasementGateParts, Invalid)
NERVE_IMPL(MoonBasementGateParts, BreakDelay)

NERVES_MAKE_STRUCT(MoonBasementGateParts, Wait, Invalid, BreakDelay)
}  // namespace

MoonBasementGateParts::MoonBasementGateParts(const char* name) : al::LiveActor(name) {}

void MoonBasementGateParts::initParts(const al::ActorInitInfo& info, const char* modelName,
                                      const char* breakModelName, const sead::Vector3f& trans,
                                      bool isStartInvalid) {
    al::initActorWithArchiveName(this, info, modelName, nullptr);
    al::initNerve(this, &NrvMoonBasementGateParts.Wait, 0);
    al::setTrans(this, trans);

    sead::Matrix34f* matrix = new sead::Matrix34f;
    al::makeMtxSRT(matrix, this);

    al::HitSensor* collisionSensor = al::getHitSensor(this, "Collision");
    mCollisionObj =
        al::createCollisionObjMtx(this, info, modelName, collisionSensor, matrix, nullptr);
    mCollisionObj->makeActorAlive();

    mBreakModel = al::createBreakModel(this, info, "崩落ステージのゲートパーツ壊れモデル",
                                       breakModelName, nullptr, nullptr, "Break");

    if (isStartInvalid)
        al::setNerve(this, &NrvMoonBasementGateParts.Invalid);

    makeActorAlive();
}

void MoonBasementGateParts::kill() {
    al::LiveActor::kill();
    mCollisionObj->kill();

    if (mPrevParts != nullptr)
        mPrevParts->validate();
}

bool MoonBasementGateParts::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                       al::HitSensor* self) {
    if (!al::isNerve(this, &NrvMoonBasementGateParts.Wait))
        return false;

    if (!rs::isMsgKoopaHackPunchCollide(message) && !al::isMsgExplosionCollide(message))
        return false;

    if (mPrevParts != nullptr)
        mPrevParts->moveSlideToTarget(al::getActorTrans(other));

    al::hideModelIfShow(this);
    mBreakModel->appear();
    al::startHitReaction(this, "破壊");
    al::setNerve(this, &NrvMoonBasementGateParts.BreakDelay);
    return true;
}

void MoonBasementGateParts::validate() {
    al::setNerve(this, &NrvMoonBasementGateParts.Wait);
}

void MoonBasementGateParts::moveSlideToTarget(const sead::Vector3f& targetTrans) {
    sead::Vector3f direction = targetTrans - al::getTrans(this);
    al::verticalizeVec(&direction, al::getFront(this), direction);

    sead::Vector3f nextTrans = al::getTrans(this) + direction;
    al::resetPosition(this, nextTrans);
}

void MoonBasementGateParts::exeInvalid() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
}

void MoonBasementGateParts::exeWait() {
    if (al::isFirstStep(this))
        al::tryStartActionIfNotPlaying(this, "Wait");
}

void MoonBasementGateParts::exeBreakDelay() {
    if (al::isGreaterEqualStep(this, 5)) {
        al::LiveActor* actor = this;
        actor->kill();
    }
}
