#include "Library/MapObj/RollingCubeMapParts.h"

#include "Library/KeyPose/KeyPoseKeeper.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Placement/PlacementFunction.h"

namespace {
using namespace al;

NERVE_ACTION_IMPL(RollingCubeMapParts, Wait)
NERVE_ACTION_IMPL(RollingCubeMapParts, Start)
NERVE_ACTION_IMPL(RollingCubeMapParts, Rotate)
NERVE_ACTION_IMPL(RollingCubeMapParts, Fall)
NERVE_ACTION_IMPL_(RollingCubeMapParts, SlideX, Slide)
NERVE_ACTION_IMPL_(RollingCubeMapParts, SlideY, Slide)
NERVE_ACTION_IMPL_(RollingCubeMapParts, SlideZ, Slide)
NERVE_ACTION_IMPL_(RollingCubeMapParts, LandX, Land)
NERVE_ACTION_IMPL_(RollingCubeMapParts, LandY, Land)
NERVE_ACTION_IMPL_(RollingCubeMapParts, LandZ, Land)
NERVE_ACTION_IMPL_(RollingCubeMapParts, FallLandX, FallLand)
NERVE_ACTION_IMPL_(RollingCubeMapParts, FallLandY, FallLand)
NERVE_ACTION_IMPL_(RollingCubeMapParts, FallLandZ, FallLand)
NERVE_ACTION_IMPL(RollingCubeMapParts, Stop)

NERVE_ACTIONS_MAKE_STRUCT(RollingCubeMapParts, Wait, Start, Rotate, Fall, SlideX, SlideY, SlideZ,
                          LandX, LandY, LandZ, FallLandX, FallLandY, FallLandZ, Stop)
}  // namespace

namespace al {
RollingCubeMapParts::RollingCubeMapParts(const char* name) : LiveActor(name) {}

// void RollingCubeMapParts::init(const ActorInitInfo& info) {}

void RollingCubeMapParts::kill() {
    startHitReaction(this, "消滅");

    LiveActor::kill();
}

bool RollingCubeMapParts::receiveMsg(const SensorMsg* message, HitSensor* source,
                                     HitSensor* target) {
    if (isMsgPlayerFloorTouch(message) && isNerve(this, NrvRollingCubeMapParts.Wait.data())) {
        startNerveAction(this, "Start");

        return true;
    }

    return false;
}

// void RollingCubeMapParts::control() {}

void RollingCubeMapParts::appearAndSetStart() {
    mRollingCubePoseKeeper->setStart();
    setQuat(this, _150);
    setTrans(this, _160);
    resetPosition(this);
    setNerveNextMovement(isNextFallKey());

    appear();
}

void RollingCubeMapParts::setNerveNextMovement(bool isNextFallKey) {
    if (isMovementCurrentKeyRotate(mRollingCubePoseKeeper)) {
        startNerveAction(this, "Rotate");

        return;
    }

    if (isNextFallKey) {
        startNerveAction(this, "Fall");

        return;
    }

    u32 tmp = calcNearVecFromAxis3(nullptr, sead::Vector3f::ey, getQuat(this));
    s32 axis = ((s32)tmp >> 31 ^ (s32)tmp) -
               ((s32)tmp >> 31);  // TODO: Find Why the compiler doesn't want to generate this.
    switch (axis) {
    default:
        return;
    case 1:
        startNerveAction(this, "SlideX");
        return;
    case 2:
        startNerveAction(this, "SlideY");
        return;
    case 3:
        startNerveAction(this, "SlideZ");
        return;
    }
}

bool RollingCubeMapParts::isNextFallKey() const {
    bool isNextFall = false;
    tryGetArg(&isNextFall, getCurrentKeyPlacementInfo((KeyPoseKeeper*)mRollingCubePoseKeeper),
              "isNextFall");

    return isNextFall;
}

void RollingCubeMapParts::exeWait() {}

void RollingCubeMapParts::exeStart() {
    if (isGreaterEqualStep(this, 40))
        setNerveNextMovement(isNextFallKey());
}

// This function is exeSlide with updateSlide inlined
// I didn't find a better match
void RollingCubeMapParts::exeRotate() {
    if (isFirstStep(this)) {
        fittingToCurrentKeyBoundingBox(getQuatPtr(this), getTransPtr(this), mRollingCubePoseKeeper);
        _16c = getQuat(this);
        _17c = getTrans(this);
        mMovementTime = getMovementTime() - 1;
    }

    calcCurrentKeyQT(getQuatPtr(this), getTransPtr(this), mRollingCubePoseKeeper, _16c, _17c,
                     calcNerveSquareInRate(this, mMovementTime));

    if (isGreaterEqualStep(this, mMovementTime)) {
        if (nextRollingCubeKey(mRollingCubePoseKeeper)) {
            if (!isNextFallKey())
                setNerveNextLand();
            else if (isMovementCurrentKeyRotate(mRollingCubePoseKeeper))
                startNerveAction(this, "Rotate");
            else
                startNerveAction(this, "Fall");
        } else {
            startNerveAction(this, "Stop");
        }
    }
}

s32 RollingCubeMapParts::getMovementTime() const {
    s32 movementTime = 40;
    tryGetArg(&movementTime, getCurrentKeyPlacementInfo((KeyPoseKeeper*)mRollingCubePoseKeeper),
              "MovementTime");

    return movementTime;
}

void RollingCubeMapParts::setNerveNextLand() {
    u32 tmp = calcNearVecFromAxis3(nullptr, sead::Vector3f::ey, getQuat(this));
    s32 axis = ((s32)tmp >> 31 ^ (s32)tmp) -
               ((s32)tmp >> 31);  // TODO: Find Why the compiler doesn't want to generate this.
    switch (axis) {
    default:
        return;
    case 1:
        startNerveAction(this, "LandX");
        return;
    case 2:
        startNerveAction(this, "LandY");
        return;
    case 3:
        startNerveAction(this, "LandZ");
        return;
    }
}

void RollingCubeMapParts::exeSlide() {
    if (updateSlide()) {
        if (nextRollingCubeKey(mRollingCubePoseKeeper)) {
            if (!isNextFallKey())
                setNerveNextLand();
            else if (isMovementCurrentKeyRotate(mRollingCubePoseKeeper))
                startNerveAction(this, "Rotate");
            else
                startNerveAction(this, "Fall");
        } else {
            startNerveAction(this, "Stop");
        }
    }
}

bool RollingCubeMapParts::updateSlide() {
    if (isFirstStep(this)) {
        fittingToCurrentKeyBoundingBox(getQuatPtr(this), getTransPtr(this), mRollingCubePoseKeeper);
        _16c = getQuat(this);
        _17c = getTrans(this);
        mMovementTime = getMovementTime() - 1;
    }

    calcCurrentKeyQT(getQuatPtr(this), getTransPtr(this), mRollingCubePoseKeeper, _16c, _17c,
                     calcNerveSquareInRate(this, mMovementTime));

    return isGreaterEqualStep(this, mMovementTime);
}

void RollingCubeMapParts::exeFall() {
    if (updateSlide()) {
        if (nextRollingCubeKey(mRollingCubePoseKeeper)) {
            if (!isNextFallKey())
                setNerveNextFallLand();
            else if (isMovementCurrentKeyRotate(mRollingCubePoseKeeper))
                startNerveAction(this, "Rotate");
            else
                startNerveAction(this, "Fall");
        } else {
            startNerveAction(this, "Stop");
        }
    }
}

void RollingCubeMapParts::setNerveNextFallLand() {
    u32 tmp = calcNearVecFromAxis3(nullptr, sead::Vector3f::ey, getQuat(this));
    s32 axis = ((s32)tmp >> 31 ^ (s32)tmp) -
               ((s32)tmp >> 31);  // TODO: Find Why the compiler doesn't want to generate this.
    switch (axis) {
    default:
        return;
    case 1:
        startNerveAction(this, "FallLandX");
        return;
    case 2:
        startNerveAction(this, "FallLandY");
        return;
    case 3:
        startNerveAction(this, "FallLandZ");
        return;
    }
}

void RollingCubeMapParts::exeLand() {
    if (isGreaterEqualStep(this, getLandTime() - 1))
        setNerveNextMovement(false);
}

s32 RollingCubeMapParts::getLandTime() const {
    s32 landTime = 40;
    tryGetArg(&landTime, getCurrentKeyPlacementInfo((KeyPoseKeeper*)mRollingCubePoseKeeper),
              "LandTime");

    return landTime;
}

void RollingCubeMapParts::exeFallLand() {
    if (isGreaterEqualStep(this, getLandTime()))
        setNerveNextMovement(false);
}

void RollingCubeMapParts::exeStop() {
    if (_198) {
        if (isFirstStep(this))
            startHitReaction(this, "消滅");

        if (isStep(this, 1)) {
            setQuat(this, _150);
            setTrans(this, _160);
            resetPosition(this);

            makeActorDead();
        }
    }
}

bool RollingCubeMapParts::isStop() const {
    return isNerve(this, NrvRollingCubeMapParts.Stop.data());
}
}  // namespace al
