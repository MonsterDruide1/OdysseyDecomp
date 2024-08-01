#include "Library/MapObj/FloaterMapParts.h"

#include "Library/Effect/EffectSystemInfo.h"
#include "Library/KeyPose/KeyPoseKeeper.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/Math/MathLengthUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
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

bool FloaterMapParts::receiveMsg(const SensorMsg* message, HitSensor* source, HitSensor* target) {
    if (isMsgFloorTouch(message)) {
        mMoveType = MoveType::Stop;

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
    _110 = 0.0f;
    mMoveType = MoveType::Loop;

    calcLerpKeyTrans(getTransPtr(this), mKeyPoseKeeper, 0.0f);
    calcSlerpKeyQuat(getQuatPtr(this), mKeyPoseKeeper, 0.0f);

    resetPosition(this);
    startNerveAction(this, "Wait");

    makeActorAlive();
}

void FloaterMapParts::control() {
    f32 rate = isNearZero(mMaxCoord, 0.001f) ? 0.0f : _110 / mMaxCoord;

    calcLerpKeyTrans(getTransPtr(this), mKeyPoseKeeper, rate);
    calcSlerpKeyQuat(getQuatPtr(this), mKeyPoseKeeper, rate);

    if (mMoveType > MoveType::Loop)
        mMoveType = static_cast<MoveType>(static_cast<s32>(mMoveType) - 1);
}

void FloaterMapParts::exeWait() {
    if (isFirstStep(this))
        validateClipping(this);

    if (mMoveType > MoveType::Loop) {
        invalidateClipping(this);
        startNerveAction(this, "Sink");
    }
}

void FloaterMapParts::exeSink() {
    if (isFirstStep(this))
        _130 = 0;

    if (mMoveType == MoveType::Loop) {
        s32 copy130 = _130;
        _124++;
        if (copy130 > 0)
            _130--;
    } else {
        _110 += (mMaxAccelCount < 1 ? 1.0f : (f32)_130 / (f32)mMaxAccelCount) * mSinkSpeed;

        if (_130 < mMaxAccelCount)
            _130++;

        if (_110 > mMaxCoord)
            _110 = mMaxCoord;

        _124 = 0;
    }

    if (_124 >= mSinkKeepTime)
        startNerveAction(this, "Back");
}

// TODO: Mismatch
void FloaterMapParts::exeBack() {
    if (isFirstStep(this))
        _130 = 0;

    _110 -= (mMaxAccelCount < 1 ? 1.0f : (f32)_130 / (f32)mMaxAccelCount) * mBackSpeed;

    if (_130 < mMaxAccelCount)
        _130++;

    if (_110 < 0.0f)
        _110 = 0.0f;

    if (mMoveType < MoveType::Turn) {
        if (_110 < 0.0f)
            startNerveAction(this, "Wait");

        return;
    }

    startNerveAction(this, "Sink");
}
}  // namespace al
