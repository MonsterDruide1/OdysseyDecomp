#include "Library/MapObj/SeesawMapParts.h"

#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/MapObj/ChildStep.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Se/SeFunction.h"

namespace {
using namespace al;

NERVE_ACTION_IMPL(SeesawMapParts, Wait)

NERVE_ACTIONS_MAKE_STRUCT(SeesawMapParts, Wait)
}  // namespace

namespace al {
SeesawMapParts::SeesawMapParts(const char* name) : LiveActor(name) {}

void SeesawMapParts::init(const ActorInitInfo& info) {
    tryInitSubActorKeeperChildStep(this, info);
    initNerveAction(this, "Wait", &NrvSeesawMapParts.mCollector, 0);
    initMapPartsActor(this, info, nullptr);
    tryGetQuatPtr(this);

    registerAreaHostMtx(this, info);
    tryGetQuatPtr(this);

    mQuat = getQuat(this);
    calcQuatSide(&mSide, mQuat);
    calcQuatFront(&mFront, mQuat);

    tryGetArg(&mMaxDegree, info, "MaxDegree");
    tryGetArg(&mRotateAccelOn, info, "RotateAccelOn");
    tryGetArg(&mRotateAccelOff, info, "RotateAccelOff");

    initMaterialCode(this, info);
    createChildStep(info, this, true);
    trySyncStageSwitchAppear(this);
}

inline bool isGreaterThanOrEqualToZero(f32 val) {  // sead::Mathf::assertGreaterThanOrEqualToZero_ ?
    return val >= 0.0f;
}

bool SeesawMapParts::receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) {
    if (isMsgFloorTouch(message)) {
        sead::Vector3f pos;
        if (isMySensor(self, this))
            pos.set(getSensorPos(other));
        else
            pos.set(getActorTrans(self));

        f32 weight = isMsgEnemyFloorTouch(message) ? 0.9f : 1.0f;
        if (!isGreaterThanOrEqualToZero((pos - getTrans(this)).dot(mFront)))
            weight = -weight;

        mWeight += weight;

        return true;
    }

    if (isMsgShowModel(message)) {
        showModelIfHide(this);

        return true;
    }

    if (isMsgHideModel(message)) {
        hideModelIfShow(this);

        return true;
    }

    if (isMsgRestart(message)) {
        appearAndSetStart();

        return true;
    }

    return false;
}

void SeesawMapParts::appearAndSetStart() {
    mRotateDegree = 0.0f;
    mRotateSpeed = 0.0f;
    mWeight = 0.0f;
    mRemainingAccelOnFrames = 0;

    setQuat(this, mQuat);

    makeActorAlive();
}

void SeesawMapParts::exeWait() {
    if (mWeight > 0.0f)
        mRemainingAccelOnFrames = mRemainingAccelOnFrames > 59 ? 60 : mRemainingAccelOnFrames + 1;
    else if (mWeight < 0.0f)
        mRemainingAccelOnFrames = mRemainingAccelOnFrames < -59 ? -60 : mRemainingAccelOnFrames - 1;
    else if (mRemainingAccelOnFrames > 0)
        mRemainingAccelOnFrames--;
    else if (mRemainingAccelOnFrames < 0)
        mRemainingAccelOnFrames++;

    mWeight = 0.0f;

    if (mRemainingAccelOnFrames > 0)
        mRotateSpeed += mRotateAccelOn;
    else if (mRemainingAccelOnFrames < 0)
        mRotateSpeed -= mRotateAccelOn;
    else if (mRotateDegree >= 0.0f)
        mRotateSpeed -= mRotateAccelOff;
    else
        mRotateSpeed += mRotateAccelOff;

    mRotateSpeed *= 0.95f;
    mRotateDegree += mRotateSpeed;
    f32 rotateSpeed = sead::Mathf::abs(mRotateSpeed);

    if (sead::Mathf::abs(mRotateDegree) > mMaxDegree) {
        if (isSameSign(mRotateSpeed, mRotateDegree))
            mRotateSpeed *= -0.5f;

        mRotateDegree = sead::Mathf::clamp(mRotateDegree, -mMaxDegree, mMaxDegree);

        if (rotateSpeed > 0.2f)
            tryStartSeWithParam(this, "Stop", rotateSpeed, "");
    }

    if (rotateSpeed > 0.1f)
        tryHoldSeWithParam(this, "Rotate", rotateSpeed, "");

    rotateQuatRadian(getQuatPtr(this), mQuat, mSide, sead::Mathf::deg2rad(mRotateDegree));
}
}  // namespace al
