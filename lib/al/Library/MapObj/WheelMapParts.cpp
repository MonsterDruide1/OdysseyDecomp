#include "Library/MapObj/WheelMapParts.h"

#include "Library/Area/SwitchKeepOnAreaGroup.h"
#include "Library/Area/SwitchOnAreaGroup.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/MapObj/ChildStep.h"
#include "Library/Math/MathUtil.h"
#include "Library/Matrix/MatrixUtil.h"
#include "Library/Movement/WheelMovement.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Se/SeFunction.h"

namespace {
using namespace al;

NERVE_ACTION_IMPL(WheelMapParts, Wait)
NERVE_ACTION_IMPL(WheelMapParts, Move)
NERVE_ACTION_IMPL(WheelMapParts, AssistStop)

NERVE_ACTIONS_MAKE_STRUCT(WheelMapParts, Wait, Move, AssistStop)
}  // namespace

namespace al {
WheelMapParts::WheelMapParts(const char* name) : LiveActor(name) {}

void WheelMapParts::init(const ActorInitInfo& info) {
    tryInitSubActorKeeperChildStep(this, info);
    initNerveAction(this, "Wait", &NrvWheelMapParts.mCollector, 0);
    initMapPartsActor(this, info, nullptr);
    tryGetQuatPtr(this);
    registerAreaHostMtx(this, info);

    mWheelMovement = new WheelMovement(this, info);

    initMaterialCode(this, info);
    trySetEffectNamedMtxPtr(this, "Surface", &mSurfaceEffectMtx);
    createChildStep(info, this, true);

    mSwitchKeepOnAreaGroup = tryCreateSwitchKeepOnAreaGroup(this, info);
    mSwitchOnAreaGroup = tryCreateSwitchOnAreaGroup(this, info);

    trySyncStageSwitchAppear(this);
}

void WheelMapParts::control() {
    sead::Vector3f moveDir = mWheelMovement->getMoveDir();
    if (mWheelMovement->get_50() < 0.0f)
        moveDir *= -1;

    makeMtxUpFrontPos(&mSurfaceEffectMtx, sead::Vector3f::ey, moveDir, getTrans(this));

    if (mSwitchKeepOnAreaGroup != nullptr)
        mSwitchKeepOnAreaGroup->update(getTrans(this));

    if (mSwitchOnAreaGroup != nullptr)
        mSwitchOnAreaGroup->update(getTrans(this));
}

bool WheelMapParts::receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) {
    if (isMsgTouchAssist(message)) {
        mAssistStopTimer = 45;
        if (!isNerve(this, NrvWheelMapParts.AssistStop.data()))
            startNerveAction(this, "AssistStop");

        return true;
    }

    if (mWheelMovement->receiveMsg(this, message, other, self))
        return true;

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

void WheelMapParts::appearAndSetStart() {
    mAssistStopTimer = 0;
    mWheelMovement->reset(this);

    makeActorAlive();
}

void WheelMapParts::exeWait() {
    mWheelMovement->update(this);

    if (mWheelMovement->get_66())
        startHitReaction(this, "端点接触");

    if (!isNearZero(mWheelMovement->get_48(), 0.2f))
        startNerveAction(this, "Move");
}

void WheelMapParts::exeMove() {
    mWheelMovement->update(this);

    if (mWheelMovement->get_66())
        startHitReaction(this, "端点接触");

    f32 fVar3 = mWheelMovement->get_48();
    if (isNearZero(fVar3, 0.2f))
        startNerveAction(this, "Wait");
    else
        tryHoldSeWithParam(this, "Rotate", sead::Mathf::abs(fVar3), "回転速度");
}

void WheelMapParts::exeAssistStop() {
    mAssistStopTimer--;
    if (mAssistStopTimer <= 0) {
        mAssistStopTimer = 0;
        startNerveAction(this, "Wait");
    }
}
}  // namespace al
