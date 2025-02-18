#include "Library/MapObj/SwitchOpenMapParts.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Stage/StageSwitchKeeper.h"
#include "Library/Thread/FunctorV0M.h"

namespace {
using namespace al;

NERVE_ACTION_IMPL(SwitchOpenMapParts, Wait)
NERVE_ACTION_IMPL(SwitchOpenMapParts, DelayOpen)
NERVE_ACTION_IMPL(SwitchOpenMapParts, Open)
NERVE_ACTION_IMPL(SwitchOpenMapParts, WaitOpend)
NERVE_ACTION_IMPL(SwitchOpenMapParts, DelayClose)
NERVE_ACTION_IMPL(SwitchOpenMapParts, Close)

NERVE_ACTIONS_MAKE_STRUCT(SwitchOpenMapParts, Wait, DelayOpen, Open, WaitOpend, DelayClose, Close)
}  // namespace

namespace al {
SwitchOpenMapParts::SwitchOpenMapParts(const char* name) : LiveActor(name) {}

void SwitchOpenMapParts::init(const ActorInitInfo& info) {
    using SwitchOpenMapPartsFunctor =
        FunctorV0M<SwitchOpenMapParts*, void (SwitchOpenMapParts::*)()>;

    initNerveAction(this, "Wait", &NrvSwitchOpenMapParts.mCollector, 0);

    const char* suffix = nullptr;
    tryGetStringArg(&suffix, info, "SuffixName");
    initMapPartsActor(this, info, suffix);

    tryGetQuatPtr(this);
    getTrans(&mTrans, info);

    PlacementInfo endPointInfo;
    getLinksInfo(&endPointInfo, info, "EndPoint");

    sead::Vector3f endPointTrans;
    getTrans(&endPointTrans, endPointInfo);
    endPointTrans.set(endPointTrans - mTrans);
    mEndPointDist = endPointTrans.length();
    normalize(&mEndPointDir, endPointTrans);

    registerAreaHostMtx(this, info);

    tryGetArg(&mDelayTimeOpen, info, "DelayTimeOpen");
    tryGetArg(&mDelayTimeClose, info, "DelayTimeClose");
    tryGetArg(&mSpeedOpen, info, "SpeedOpen");
    tryGetArg(&mSpeedClose, info, "SpeedClose");

    listenStageSwitchOnOff(this, "SwitchStart",
                           SwitchOpenMapPartsFunctor(this, &SwitchOpenMapParts::open),
                           SwitchOpenMapPartsFunctor(this, &SwitchOpenMapParts::close));

    makeActorAlive();
}

void SwitchOpenMapParts::open() {
    if (isNerve(this, NrvSwitchOpenMapParts.Wait.data()))
        if (mDelayTimeOpen > 0)
            startNerveAction(this, "DelayOpen");
        else
            startNerveAction(this, "Open");
    else if (isNerve(this, NrvSwitchOpenMapParts.DelayClose.data()))
        startNerveAction(this, "WaitOpend");
    else if (isNerve(this, NrvSwitchOpenMapParts.Close.data()))
        startNerveAction(this, "Open");
}

void SwitchOpenMapParts::close() {
    if (isNerve(this, NrvSwitchOpenMapParts.WaitOpend.data()))
        if (mDelayTimeClose > 0)
            startNerveAction(this, "DelayOpen");
        else
            startNerveAction(this, "Close");
    else if (isNerve(this, NrvSwitchOpenMapParts.DelayOpen.data()))
        startNerveAction(this, "Wait");
    else if (isNerve(this, NrvSwitchOpenMapParts.Open.data()))
        startNerveAction(this, "Close");
}

void SwitchOpenMapParts::exeWait() {
    if (isFirstStep(this))
        validateClipping(this);
}

void SwitchOpenMapParts::exeDelayOpen() {
    if (isGreaterEqualStep(this, mDelayTimeOpen - 1))
        startNerveAction(this, "Open");
}

void SwitchOpenMapParts::exeOpen() {
    if (isFirstStep(this)) {
        invalidateClipping(this);
        if (isExistAction(this, "MoveLoop"))
            tryStartActionIfNotPlaying(this, "MoveLoop");
    }

    mCoord += mSpeedOpen;
    if (mCoord > mEndPointDist) {
        mCoord = mEndPointDist;
        setTrans(this, mCoord * mEndPointDir + mTrans);
        startNerveAction(this, "WaitOpend");
    } else {
        setTrans(this, mCoord * mEndPointDir + mTrans);
    }
}

void SwitchOpenMapParts::exeWaitOpend() {}

void SwitchOpenMapParts::exeDelayClose() {
    if (isGreaterEqualStep(this, mDelayTimeClose - 1))
        startNerveAction(this, "Close");
}

void SwitchOpenMapParts::exeClose() {
    if (isFirstStep(this) && isExistAction(this, "MoveLoop"))
        tryStartActionIfNotPlaying(this, "MoveLoop");

    mCoord -= mSpeedClose;
    if (mCoord < 0.0f) {
        mCoord = 0.0f;
        setTrans(this, mTrans);
        startNerveAction(this, "Wait");
    } else {
        setTrans(this, mCoord * mEndPointDir + mTrans);
    }
}
}  // namespace al
