#include "Library/MapObj/RollingCubeMapParts.h"

#include "Library/Base/StringUtil.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorResourceFunction.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Obj/PartsModel.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/ByamlUtil.h"

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

    _150 = getQuat(this);
    _160 = getTrans(this);

    bool isUseMoveLimit = false;
    tryGetArg(&isUseMoveLimit, info, "IsUseMoveLimit");
    if (isUseMoveLimit) {
        _110 = new sead::Matrix34f();
        _110->makeQT(_150, _160);

        mPartsModel = new PartsModel("");
        sead::FixedSafeString<256> model;
        sead::FixedSafeString<256> ukn;
        makeMapPartsModelName(&model, &ukn, info);
        mPartsModel->initPartsSuffix(this, info, model.cstr(), "MoveLimit", _110, false);
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

    trySetEffectNamedMtxPtr(this, "Land", &_120);
    initMaterialCode(this, info);

    f32 ukn = 0.0f;
    calcRollingCubeClippingInfo(&_188, &ukn, mRollingCubePoseKeeper, 0.0f);
    setClippingInfo(this, ukn, &_188);

    tryListenStageSwitchKill(this);
    makeActorAlive();
}

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

void RollingCubeMapParts::control() {
    if (_110 != nullptr)
        _110->makeQT(_150, getTrans(this));

    calcMtxLandEffect(&_120, mRollingCubePoseKeeper, getQuat(this), getTrans(this));
}

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

    s32 axis = sead::Mathi::abs(calcNearVecFromAxis3(nullptr, sead::Vector3f::ey, getQuat(this)));
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
    tryGetArg(&movementTime, getCurrentKeyPlacementInfo(mRollingCubePoseKeeper), "MovementTime");

    return movementTime;
}

void RollingCubeMapParts::setNerveNextLand() {
    s32 axis = sead::Mathi::abs(calcNearVecFromAxis3(nullptr, sead::Vector3f::ey, getQuat(this)));
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
    s32 axis = sead::Mathi::abs(calcNearVecFromAxis3(nullptr, sead::Vector3f::ey, getQuat(this)));
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
    tryGetArg(&landTime, getCurrentKeyPlacementInfo(mRollingCubePoseKeeper), "LandTime");

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
