#include "Library/MapObj/GateMapParts.h"

#include "Library/Audio/System/SimpleAudioUser.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchKeeper.h"
#include "Library/Thread/FunctorV0M.h"

namespace {
using namespace al;

NERVE_ACTION_IMPL(GateMapParts, Wait)
NERVE_ACTION_IMPL(GateMapParts, Open)
NERVE_ACTION_IMPL(GateMapParts, Bound)
NERVE_ACTION_IMPL(GateMapParts, End)

NERVE_ACTIONS_MAKE_STRUCT(GateMapParts, Wait, Open, Bound, End)
}  // namespace

namespace al {
GateMapParts::GateMapParts(const char* name) : LiveActor(name) {}

// TODO: Non Matching
void GateMapParts::init(const ActorInitInfo& info) {
    using GateMapPartsFunctor = FunctorV0M<GateMapParts*, void (GateMapParts::*)()>;

    initNerveAction(this, "Wait", &NrvGateMapParts.mCollector, 0);
    initMapPartsActor(this, info, nullptr);
    tryGetQuatPtr(this);

    mTrans = getTrans(this);
    mQuat = getQuat(this);

    tryGetLinksTrans(&mMoveNextTrans, info, "MoveNext");
    tryGetLinksQuat(&mMoveNextQuat, info, "MoveNext");

    tryGetArg(&mOpenTime, info, "OpenTime");
    tryGetArg(&mBoundRate, info, "BoundRate");
    tryGetArg(&mHitReactionCount, info, "HitReactionCount");

    listenStageSwitchOnStart(this, GateMapPartsFunctor(this, &GateMapParts::start));

    bool isPlaySuccessSe = false;
    tryGetArg(&isPlaySuccessSe, info, "IsPlaySuccessSe");

    if (isPlaySuccessSe)
        mAudioUser = new SimpleAudioUser("SuccessSeObj", info);

    makeActorAlive();
}

void GateMapParts::start() {
    if (!isNerve(this, NrvGateMapParts.Wait.data()))
        return;

    appearAndSetStart();
}

void GateMapParts::appearAndSetStart() {
    invalidateClipping(this);
    startNerveAction(this, "Open");
}

void GateMapParts::exeWait() {
    if (isFirstStep(this))
        validateClipping(this);
}

// TODO: Depends on GateMapParts::updatePose

// void GateMapParts::exeOpen() {}

// TODO: Non Matching
void GateMapParts::updatePose(f32 rate) {
    rate = sead::Mathf::min3(1.0f, rate, 0.0f);
    //    f32 tmp1 = rate;
    //    if (rate > 1.0f)
    //        tmp1 = 1.0f;
    //
    //    f32 tmp2 = 0.0f;
    //    if (rate >= 0.0f)
    //        tmp2 = tmp1;

    lerpVec(getTransPtr(this), mTrans, mMoveNextTrans, rate);
    slerpQuat(getQuatPtr(this), mQuat, mMoveNextQuat, rate);
}
}  // namespace al
