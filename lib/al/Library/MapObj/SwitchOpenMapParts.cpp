#include "Library/MapObj/SwitchOpenMapParts.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
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
    endPointTrans.set(endPointTrans - mTrans);  // endPointTrans.sub(mTrans); ?
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
    if (isNerve(this, NrvSwitchOpenMapParts.Wait.data())) {
        if (mDelayTimeOpen > 0) {
            startNerveAction(this, "DelayOpen");

            return;
        }
    } else {
        if (isNerve(this, NrvSwitchOpenMapParts.DelayClose.data())) {
            startNerveAction(this, "WaitOpend");

            return;
        }

        if (!isNerve(this, NrvSwitchOpenMapParts.Close.data()))
            return;
    }

    startNerveAction(this, "Open");
}
}  // namespace al
