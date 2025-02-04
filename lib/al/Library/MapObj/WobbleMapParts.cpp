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

    _108 = getQuat(this);
    _124 = _108;
    calcQuatUp(&_118, _108);
    _140.set(_118);
    tryGetArg(&mMaxRotate, info, "MaxRotate");

    initMaterialCode(this, info);
    createChildStep(info, this, true);

    trySyncStageSwitchAppear(this);
}

// NON_MATCHING
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
            pos.set(getSensorPos(self));
        else
            pos.set(getActorTrans(other));
        pos.set(pos - getTrans(this));

        sead::Vector3f up;
        calcQuatUp(&up, _124);

        sead::Vector3f local_60;
        verticalizeVec(&local_60, up, pos);

        f32 length = local_60.length();
        f32 fVar3 = normalize(length, 0.0f, 100.0f);

        if (isNearZero(length, 0.001f))
            local_60 = sead::Vector3f::zero;
        else
            local_60 *= sead::Mathf::sin(fVar3 * sead::Mathf::deg2rad(mMaxRotate)) / length;

        f32 cos = sead::Mathf::cos(fVar3 * sead::Mathf::deg2rad(mMaxRotate));
        _140.set(cos * _118 + local_60);

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
    _124.set(_108);
    _140.set(_118);

    setQuat(this, _108);
    startNerveAction(this, "Wait");

    makeActorAlive();
}

void WobbleMapParts::exeWait() {
    updateMove();

    if (_15c > 0.1f)
        startNerveAction(this, "Move");
}

void WobbleMapParts::updateMove() {
    sead::Vector3f up1;
    calcQuatUp(&up1, _124);

    sead::Vector3f local_40;
    local_40.setCross(up1, _140);
    local_40 *= 180.0f / sead::Mathf::pi();
    limitLength(&local_40, local_40, mMaxRotate * 0.001333333f);

    _134 += local_40 * 0.92f;
    rotateQuatMomentDegree(&_124, _124, _134);

    sead::Vector3f up2;
    calcQuatUp(&up2, _124);

    sead::Vector3f vStack_70;
    bool isStop = !turnVecToVecDegree(&vStack_70, _118, up2, mMaxRotate);
    turnQuatYDirRate(getQuatPtr(this), _108, vStack_70, 1.0f);

    if (isStop)
        _124 = getQuat(this);

    _140.set(_118);
    _15c = _134.length();
    if (mIsStop != isStop)
        tryStartSeWithParam(this, "Stop", _15c, "");

    mIsStop = isStop;
}
}  // namespace al
