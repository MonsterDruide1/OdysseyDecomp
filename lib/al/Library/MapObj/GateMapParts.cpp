#include "Library/MapObj/GateMapParts.h"

#include "Library/Audio/System/SimpleAudioUser.h"
#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Se/SeFunction.h"
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
        mSuccessSeObj = new SimpleAudioUser("SuccessSeObj", info);

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

void GateMapParts::exeOpen() {
    updatePose(calcNerveSquareInRate(this, mOpenTime - 1));

    if (isGreaterEqualStep(this, mOpenTime - 1)) {
        mCurrentBoundRate = mBoundRate;
        mCurrentBoundSteps = (s32)(mBoundRate * (f32)mOpenTime * 2);
        mHitReactionCurrent = 0;

        if (mMaxHitReactions > mHitReactionCurrent && mCurrentBoundSteps > 1) {
            startAction(this, "Bound");

            return;
        }

        if (mSuccessSeObj != nullptr)
            startSe(mSuccessSeObj, "Riddle");

        startAction(this, "End");

        if (mHitReactionCount < 2)
            startHitReaction(this, "バウンド1回目");
    }
}

void GateMapParts::updatePose(f32 rate) {
    rate = sead::Mathf::clamp(rate, 0.0f, 1.0f);

    lerpVec(getTransPtr(this), mTrans, mMoveNextTrans, rate);
    slerpQuat(getQuatPtr(this), mQuat, mMoveNextQuat, rate);
}

void GateMapParts::exeBound() {
    if (isFirstStep(this)) {
        if (mHitReactionCurrent++ < mHitReactionCount)
            startHitReaction(this, StringTmp<32>("バウンド%d回目", mHitReactionCurrent).cstr());

        tryStartSeWithParam(this, "BoundStart", (f32)(mMaxHitReactions - mHitReactionCurrent), "");
    }

    f32 rate = calcNerveRate(this, mCurrentBoundSteps - 1);
    rate = sead::Mathf::pow(mCurrentBoundRate * (rate * 2 - 1.0f), 2);
    rate += (1.0f - sead::Mathf::pow(mCurrentBoundRate, 2));

    updatePose(rate);

    if (isGreaterEqualStep(this, mCurrentBoundSteps - 1)) {
        mCurrentBoundRate *= mBoundRate;
        mCurrentBoundSteps = (s32)(mBoundRate * (f32)mCurrentBoundSteps);

        if (mMaxHitReactions > mHitReactionCurrent && mCurrentBoundSteps > 1) {
            startAction(this, "Bound");

            return;
        }

        if (mSuccessSeObj != nullptr)
            startSe(mSuccessSeObj, "Riddle");

        startAction(this, "End");
    }
}

void GateMapParts::exeEnd() {
    if (isFirstStep(this)) {
        validateClipping(this);
        updatePose(1.0f);
    }
}
}  // namespace al
