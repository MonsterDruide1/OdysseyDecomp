#include "Library/MapObj/WobbleMapParts.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/MapObj/ChildStep.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Se/SeFunction.h"

namespace {
using namespace al;

NERVE_ACTION_IMPL(WobbleMapParts, Wait)
NERVE_ACTION_IMPL(WobbleMapParts, Move)
NERVE_ACTION_IMPL(WobbleMapParts, AssistStop)

NERVE_ACTIONS_MAKE_STRUCT(WobbleMapParts, Wait, Move, AssistStop)
}  // namespace

namespace al {
WobbleMapParts::WobbleMapParts(const char* name) : LiveActor(name) {}

void WobbleMapParts::init(const ActorInitInfo& info) {
    tryInitSubActorKeeperChildStep(this, info);
    initNerveAction(this, "Wait", &NrvWobbleMapParts.mCollector, 0);
    initMapPartsActor(this, info, nullptr);
    tryGetQuatPtr(this);
    registerAreaHostMtx(this, info);

    mInitialQuat = getQuat(this);
    mCurrentQuat = mInitialQuat;
    calcQuatUp(&mInitialUp, mInitialQuat);
    _140.set(mInitialUp);
    tryGetArg(&mMaxRotate, info, "MaxRotate");

    initMaterialCode(this, info);
    createChildStep(info, this, true);

    trySyncStageSwitchAppear(this);
}

bool WobbleMapParts::receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) {
    if (isMsgTouchAssist(message)) {
        mAssistStopTimer = 45;
        if (!isNerve(this, NrvWobbleMapParts.AssistStop.data()))
            startNerveAction(this, "AssistStop");

        return true;
    }

    if (isMsgFloorTouch(message)) {
        sead::Vector3f pos;
        if (isMySensor(self, this))
            pos.set(getSensorPos(other));
        else
            pos.set(getActorTrans(self));

        sead::Vector3f currentHorizontal;
        sead::Vector3f currentUp;
        calcQuatUp(&currentUp, mCurrentQuat);
        verticalizeVec(&currentHorizontal, currentUp, pos - getTrans(this));

        f32 hDist = currentHorizontal.length();
        f32 normHDist = normalize(hDist, 0.0f, 100.0f);

        if (isNearZero(hDist, 0.001f))
            currentHorizontal = sead::Vector3f::zero;
        else
            currentHorizontal *=
                sead::Mathf::sin(sead::Mathf::deg2rad(normHDist * mMaxRotate)) / hDist;

        f32 cos = sead::Mathf::cos(sead::Mathf::deg2rad(normHDist * mMaxRotate));
        _140.set(cos * mInitialUp + currentHorizontal);

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

void WobbleMapParts::appearAndSetStart() {
    mIsStop = false;
    _15c = 0.0f;
    mAssistStopTimer = 0;
    _134.set(sead::Vector3f::zero);
    mCurrentQuat.set(mInitialQuat);
    _140.set(mInitialUp);

    setQuat(this, mInitialQuat);
    startNerveAction(this, "Wait");

    makeActorAlive();
}

void WobbleMapParts::exeWait() {
    updateMove();

    if (_15c > 0.1f)
        startNerveAction(this, "Move");
}

void WobbleMapParts::updateMove() {
    sead::Vector3f currentHorizontal;
    sead::Vector3f currentUp;
    calcQuatUp(&currentUp, mCurrentQuat);
    currentHorizontal.setCross(currentUp, _140);
    currentHorizontal *= 180.0f / sead::Mathf::pi();
    limitLength(&currentHorizontal, currentHorizontal, mMaxRotate * 0.001333333f);

    _134 = (_134 + currentHorizontal) * 0.92f;
    rotateQuatMomentDegree(&mCurrentQuat, mCurrentQuat, _134);

    sead::Vector3f currentUp2;
    calcQuatUp(&currentUp2, mCurrentQuat);

    sead::Vector3f vStack_70;
    bool isStop = !turnVecToVecDegree(&vStack_70, mInitialUp, currentUp2, mMaxRotate);
    turnQuatYDirRate(getQuatPtr(this), mInitialQuat, vStack_70, 1.0f);

    if (isStop)
        mCurrentQuat = getQuat(this);

    _140.set(mInitialUp);
    _15c = _134.length();
    if (mIsStop != isStop)
        tryStartSeWithParam(this, "Stop", _15c, "");

    mIsStop = isStop;
}

void WobbleMapParts::exeMove() {
    updateMove();

    tryStartSeWithParam(this, "Rotate", _15c, "");
    if (_15c < 0.1f)
        startNerveAction(this, "Wait");
}

void WobbleMapParts::exeAssistStop() {
    mAssistStopTimer--;
    if (mAssistStopTimer <= 0) {
        mAssistStopTimer = 0;
        if (_15c > 0.1f)
            startNerveAction(this, "Move");
        else
            startNerveAction(this, "Wait");
    }
}
}  // namespace al
