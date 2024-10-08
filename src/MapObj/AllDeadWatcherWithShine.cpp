#include "MapObj/AllDeadWatcherWithShine.h"

#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/LiveActorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"

#include "Item/Shine.h"
#include "Util/ItemUtil.h"

namespace {
NERVE_IMPL(AllDeadWatcherWithShine, Watch);
NERVE_IMPL(AllDeadWatcherWithShine, Wait);

NERVES_MAKE_NOSTRUCT(AllDeadWatcherWithShine, Watch, Wait);
}  // namespace

AllDeadWatcherWithShine::AllDeadWatcherWithShine(const char* name) : al::LiveActor(name) {}

void AllDeadWatcherWithShine::init(const al::ActorInitInfo& info) {
    al::initActorSceneInfo(this, info);
    al::initExecutorWatchObj(this, info);
    al::initNerve(this, &Watch, 0);

    mChildCount = al::calcLinkChildNum(info, "WatchTargetEnemy");
    mChildActors = new LiveActor*[mChildCount];

    for (s32 i = 0; i < mChildCount; i++)
        mChildActors[i] = al::createLinksActorFromFactory(info, "WatchTargetEnemy", i);

    al::tryGetArg(&mSwitchOnDelayStep, info, "SwitchOnDelayStep");
    mIsNotControlActorAppear = al::tryGetBoolArgOrFalse(info, "IsNotControlActorAppear");

    mShine = rs::tryInitLinkShine(info, "ShineActor", 0);

    makeActorAlive();
}

void AllDeadWatcherWithShine::appear() {
    al::LiveActor::appear();

    if (!mIsNotControlActorAppear)
        for (s32 i = 0; i < mChildCount; i++)
            mChildActors[i]->appear();
}

void AllDeadWatcherWithShine::kill() {
    al::LiveActor::kill();
}

void AllDeadWatcherWithShine::exeWatch() {
    for (s32 i = 0; i < mChildCount; i++) {
        if (al::isAlive(mChildActors[i])) {
            rs::updateHintTrans(mShine, al::getTrans(mChildActors[i]));
            return;
        }
    }

    al::setNerve(this, &Wait);
}

void AllDeadWatcherWithShine::exeWait() {
    if (al::isGreaterEqualStep(this, mSwitchOnDelayStep)) {
        kill();

        mShine->appearPopup();
        rs::updateHintTrans(mShine, al::getTrans(mShine));
    }
}
