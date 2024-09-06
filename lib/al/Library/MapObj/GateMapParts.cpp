#include "Library/MapObj/GateMapParts.h"

#include "Library/Audio/System/SimpleAudioUser.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
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
}  // namespace al
