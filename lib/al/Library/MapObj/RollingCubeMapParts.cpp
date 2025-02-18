#include "Library/MapObj/RollingCubeMapParts.h"

#include "Library/Effect/EffectSystemInfo.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorResourceFunction.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Obj/PartsModel.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/ByamlUtil.h"
#include "Project/Joint/RollingCubePoseKeeper.h"

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

void RollingCubeMapParts::init(const ActorInitInfo& info) {
    initNerveAction(this, "Wait", &NrvRollingCubeMapParts.mCollector, 0);
    initMapPartsActor(this, info, nullptr);
    tryGetQuatPtr(this);

    mInitialPoseQuat = getQuat(this);
    mInitialPoseTrans = getTrans(this);

    bool isUseMoveLimit = false;
    tryGetArg(&isUseMoveLimit, info, "IsUseMoveLimit");
    if (isUseMoveLimit) {
        mMoveLimitMtx = new sead::Matrix34f();
        mMoveLimitMtx->makeQT(mInitialPoseQuat, mInitialPoseTrans);

        mMoveLimitPartsModel = new PartsModel("");
        sead::FixedSafeString<256> model;
        sead::FixedSafeString<256> archive;
        makeMapPartsModelName(&model, &archive, info);
        mMoveLimitPartsModel->initPartsSuffix(this, info, model.cstr(), "MoveLimit", mMoveLimitMtx,
                                              false);
    }

    sead::BoundBox3f boundBox;
    if (isExistModelResourceYaml(this, "BoxInfo", nullptr) &&
        tryGetByamlBox3f(&boundBox, ByamlIter(getModelResourceYaml(this, "BoxInfo", nullptr))))
        mRollingCubePoseKeeper = createRollingCubePoseKeeper(boundBox, info);
    else
        mRollingCubePoseKeeper = createRollingCubePoseKeeper(this, info);

    bool isFloorTouchStart = true;
    tryGetArg(&isFloorTouchStart, info, "IsFloorTouchStart");
    if (!isFloorTouchStart)
        startNerveAction(this, "Start");

    trySetEffectNamedMtxPtr(this, "Land", &mLandEffectMtx);
    initMaterialCode(this, info);

    f32 clippingRadius = 0.0f;
    calcRollingCubeClippingInfo(&mClippingTrans, &clippingRadius, mRollingCubePoseKeeper, 0.0f);
    setClippingInfo(this, clippingRadius, &mClippingTrans);

    tryListenStageSwitchKill(this);
    makeActorAlive();
}

void RollingCubeMapParts::kill() {
    startHitReaction(this, "消滅");

    LiveActor::kill();
}

bool RollingCubeMapParts::receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) {
    if (isMsgPlayerFloorTouch(message) && isNerve(this, NrvRollingCubeMapParts.Wait.data())) {
        startNerveAction(this, "Start");

        return true;
    }

    return false;
}

void RollingCubeMapParts::control() {
    if (mMoveLimitMtx != nullptr)
        mMoveLimitMtx->makeQT(mInitialPoseQuat, getTrans(this));

    calcMtxLandEffect(&mLandEffectMtx, mRollingCubePoseKeeper, getQuat(this), getTrans(this));
}

void RollingCubeMapParts::appearAndSetStart() {
    mRollingCubePoseKeeper->setStart();
    setQuat(this, mInitialPoseQuat);
    setTrans(this, mInitialPoseTrans);
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

    Axis axis = (Axis)sead::Mathi::abs(
        (s32)calcNearVecFromAxis3(nullptr, sead::Vector3f::ey, getQuat(this)));
    switch (axis) {
    default:
        return;
    case Axis::X:
        startNerveAction(this, "SlideX");

        return;
    case Axis::Y:
        startNerveAction(this, "SlideY");

        return;
    case Axis::Z:
        startNerveAction(this, "SlideZ");

        return;
    }
}

bool RollingCubeMapParts::isNextFallKey() const {
    bool isNextFall = false;
    tryGetArg(&isNextFall, getCurrentKeyPlacementInfo(mRollingCubePoseKeeper), "isNextFall");

    return isNextFall;
}

void RollingCubeMapParts::exeWait() {}

void RollingCubeMapParts::exeStart() {
    if (isGreaterEqualStep(this, 40))
        setNerveNextMovement(isNextFallKey());
}

void RollingCubeMapParts::exeRotate() {
    if (isFirstStep(this)) {
        fittingToCurrentKeyBoundingBox(getQuatPtr(this), getTransPtr(this), mRollingCubePoseKeeper);
        mCurrentPoseQuat = getQuat(this);
        mCurrentPoseTrans = getTrans(this);
        mMovementTime = getMovementTime() - 1;
    }

    calcCurrentKeyQT(getQuatPtr(this), getTransPtr(this), mRollingCubePoseKeeper, mCurrentPoseQuat,
                     mCurrentPoseTrans, calcNerveSquareInRate(this, mMovementTime));

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
    tryGetArg(&movementTime, getCurrentKeyPlacementInfo(mRollingCubePoseKeeper), "MovementTime");

    return movementTime;
}

void RollingCubeMapParts::setNerveNextLand() {
    Axis axis = (Axis)sead::Mathi::abs(
        (s32)calcNearVecFromAxis3(nullptr, sead::Vector3f::ey, getQuat(this)));
    switch (axis) {
    default:
        return;
    case Axis::X:
        startNerveAction(this, "LandX");

        return;
    case Axis::Y:
        startNerveAction(this, "LandY");

        return;
    case Axis::Z:
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
        mCurrentPoseQuat = getQuat(this);
        mCurrentPoseTrans = getTrans(this);
        mMovementTime = getMovementTime() - 1;
    }

    calcCurrentKeyQT(getQuatPtr(this), getTransPtr(this), mRollingCubePoseKeeper, mCurrentPoseQuat,
                     mCurrentPoseTrans, calcNerveSquareInRate(this, mMovementTime));

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
    Axis axis = (Axis)sead::Mathi::abs(
        (s32)calcNearVecFromAxis3(nullptr, sead::Vector3f::ey, getQuat(this)));
    switch (axis) {
    default:
        return;
    case Axis::X:
        startNerveAction(this, "FallLandX");

        return;
    case Axis::Y:
        startNerveAction(this, "FallLandY");

        return;
    case Axis::Z:
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
    tryGetArg(&landTime, getCurrentKeyPlacementInfo(mRollingCubePoseKeeper), "LandTime");

    return landTime;
}

void RollingCubeMapParts::exeFallLand() {
    if (isGreaterEqualStep(this, getLandTime()))
        setNerveNextMovement(false);
}

void RollingCubeMapParts::exeStop() {
    if (mIsStoppable) {
        if (isFirstStep(this))
            startHitReaction(this, "消滅");

        if (isStep(this, 1)) {
            setQuat(this, mInitialPoseQuat);
            setTrans(this, mInitialPoseTrans);
            resetPosition(this);

            makeActorDead();
        }
    }
}

bool RollingCubeMapParts::isStop() const {
    return isNerve(this, NrvRollingCubeMapParts.Stop.data());
}
}  // namespace al
