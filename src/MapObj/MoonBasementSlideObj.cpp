#include "MapObj/MoonBasementSlideObj.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(MoonBasementSlideObj, Wait)
NERVE_IMPL(MoonBasementSlideObj, Slide)

NERVES_MAKE_NOSTRUCT(MoonBasementSlideObj, Wait, Slide)
}  // namespace

MoonBasementSlideObj::MoonBasementSlideObj(const char* name) : al::LiveActor(name) {}

void MoonBasementSlideObj::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    al::initNerve(this, &Wait, 0);
    makeActorAlive();
}

bool MoonBasementSlideObj::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                      al::HitSensor* self) {
    if (rs::isMsgKoopaHackPunchCollide(message)) {
        mPunchHits++;

        sead::Vector3f sideDir = sead::Vector3f(0, 0, 0);
        sead::Vector3f frontDir = sead::Vector3f(0, 0, 0);

        al::calcSideDir(&sideDir, this);
        al::calcFrontDir(&frontDir, this);

        sead::Vector3f dir = sead::Vector3f(0, 0, 0);

        al::calcDirBetweenSensorsH(&dir, other, self);

        sead::Vector3f sideComponent = sead::Vector3f(0, 0, 0);
        sead::Vector3f frontComponent = sead::Vector3f(0, 0, 0);

        al::parallelizeVec(&sideComponent, sideDir, dir);
        al::parallelizeVec(&frontComponent, frontDir, dir);

        dir.set(sideComponent.length() < frontComponent.length() ? frontComponent : sideComponent);

        al::tryNormalizeOrZero(&dir);

        al::addVelocity(this, dir * 45.0f);
        al::startHitReaction(this, "命中");
        al::setNerve(this, &Slide);

        return true;
    }
    return false;
}

void MoonBasementSlideObj::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");

    al::addVelocityToGravity(this, 2.0f);
    al::scaleVelocityHV(this, 0.95f, 0.99f);
    al::limitVelocityDir(this, al::getGravity(this), 5.0f);
}

void MoonBasementSlideObj::exeSlide() {
    al::addVelocityToGravity(this, 2.0f);
    al::scaleVelocityHV(this, 0.95f, 0.99f);

    if (al::isOnGround(this, 0))
        al::limitVelocityDir(this, al::getGravity(this), 5.0f);

    if (al::isVelocitySlowH(this, 0.2f)) {
        sead::Vector3f* velocityPtr = al::getVelocityPtr(this);
        al::parallelizeVec(velocityPtr, al::getGravity(this), *velocityPtr);

        al::startHitReaction(this, "停止");
        al::setNerve(this, &Wait);
    } else if (al::isInDeathArea(this))
        kill();
}
