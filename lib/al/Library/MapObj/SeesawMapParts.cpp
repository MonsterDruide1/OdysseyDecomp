#include "Library/MapObj/SeesawMapParts.h"

#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/MapObj/ChildStep.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/VectorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Placement/PlacementFunction.h"

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

        f32 fVar3 = !isMsgEnemyFloorTouch(message) ? 1.0f : 0.9f;
        if (!isGreaterThanOrEqualToZero((pos - getTrans(this)).dot(mFront)))
            fVar3 = -fVar3;

        _144 += fVar3;

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
    _130 = 0.0f;
    _134 = 0.0f;
    _144 = 0.0f;
    _148 = 0.0f;

    setQuat(this, mQuat);

    makeActorAlive();
}
}  // namespace al
