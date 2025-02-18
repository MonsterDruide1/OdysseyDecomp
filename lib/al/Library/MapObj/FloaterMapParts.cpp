#include "Library/MapObj/FloaterMapParts.h"

#include "Library/Effect/EffectSystemInfo.h"
#include "Library/KeyPose/KeyPoseKeeper.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"

namespace {
using namespace al;

NERVE_ACTION_IMPL(FloaterMapParts, Wait)
NERVE_ACTION_IMPL(FloaterMapParts, Sink)
NERVE_ACTION_IMPL(FloaterMapParts, Back)

NERVE_ACTIONS_MAKE_STRUCT(FloaterMapParts, Wait, Sink, Back)
}  // namespace

namespace al {
FloaterMapParts::FloaterMapParts(const char* name) : LiveActor(name) {}

void FloaterMapParts::init(const ActorInitInfo& info) {
    initNerveAction(this, "Wait", &NrvFloaterMapParts.mCollector, 0);
    initMapPartsActor(this, info, nullptr);

    tryGetQuatPtr(this);
    registerAreaHostMtx(this, info);

    mKeyPoseKeeper = createKeyPoseKeeper(info);

    tryGetArg(&mSinkSpeed, info, "SinkSpeed");
    tryGetArg(&mBackSpeed, info, "BackSpeed");
    tryGetArg(&mMaxAccelCount, info, "MaxAccelCount");
    tryGetArg(&mSinkKeepTime, info, "SinkKeepTime");
    tryGetArg(&mMaxCoord, info, "MaxCoord");

    initMaterialCode(this, info);

    trySetEffectNamedMtxPtr(this, "Surface", &mMtxTR);
    tryGetLinksMatrixTR(&mMtxTR, info, "SurfaceEffectPos");

    if (mMaxCoord <= 0.0f)
        mMaxCoord = calcDistanceNextKeyTrans(mKeyPoseKeeper);

    trySyncStageSwitchAppear(this);
}

bool FloaterMapParts::receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) {
    if (isMsgFloorTouch(message)) {
        mMoveType = FloaterMoveType::Sink;

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

void FloaterMapParts::appearAndSetStart() {
    mCoord = 0.0f;
    mMoveType = FloaterMoveType::Wait;

    calcLerpKeyTrans(getTransPtr(this), mKeyPoseKeeper, 0.0f);
    calcSlerpKeyQuat(getQuatPtr(this), mKeyPoseKeeper, 0.0f);

    resetPosition(this);
    startNerveAction(this, "Wait");

    makeActorAlive();
}

void FloaterMapParts::control() {
    f32 rate = isNearZero(mMaxCoord, 0.001f) ? 0.0f : mCoord / mMaxCoord;

    calcLerpKeyTrans(getTransPtr(this), mKeyPoseKeeper, rate);
    calcSlerpKeyQuat(getQuatPtr(this), mKeyPoseKeeper, rate);

    if (mMoveType > FloaterMoveType::Wait)
        mMoveType = static_cast<FloaterMoveType>(static_cast<s32>(mMoveType) - 1);
}

void FloaterMapParts::exeWait() {
    if (isFirstStep(this))
        validateClipping(this);

    if (mMoveType > FloaterMoveType::Wait) {
        invalidateClipping(this);
        startNerveAction(this, "Sink");
    }
}

void FloaterMapParts::exeSink() {
    if (isFirstStep(this))
        mAccelCount = 0;

    if (mMoveType != FloaterMoveType::Wait) {
        mCoord += (mMaxAccelCount < 1 ? 1.0f : (f32)mAccelCount / (f32)mMaxAccelCount) * mSinkSpeed;

        if (mAccelCount < mMaxAccelCount)
            mAccelCount++;

        if (mCoord > mMaxCoord)
            mCoord = mMaxCoord;

        mSinkTime = 0;
    } else {
        s32 prevAccelCount = mAccelCount;
        mSinkTime++;
        if (prevAccelCount > 0)
            mAccelCount--;
    }

    if (mSinkTime >= mSinkKeepTime)
        startNerveAction(this, "Back");
}

void FloaterMapParts::exeBack() {
    if (isFirstStep(this))
        mAccelCount = 0;

    mCoord -= (mMaxAccelCount < 1 ? 1.0f : (f32)mAccelCount / (f32)mMaxAccelCount) * mBackSpeed;

    if (mAccelCount < mMaxAccelCount)
        mAccelCount++;

    bool isCoordLesserThan0;
    if (mCoord < 0.0f) {
        isCoordLesserThan0 = true;
        mCoord = 0.0f;
    } else {
        isCoordLesserThan0 = false;
    }

    if (mMoveType >= FloaterMoveType::Back)
        startNerveAction(this, "Sink");
    else if (isCoordLesserThan0)
        startNerveAction(this, "Wait");
}
}  // namespace al
