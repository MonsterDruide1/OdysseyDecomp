#include "Library/Obj/AllDeadWatcher.h"

#include "Library/LiveActor/ActorDrawFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/LiveActorUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchKeeper.h"

namespace al {
AllDeadWatcher::AllDeadWatcher(const char* name) : LiveActor(name) {}

void AllDeadWatcher::appear() {
    LiveActor::appear();

    if (mIsNotControlActorAppear)
        return;

    for (s32 i = 0; i < mTargetCount; i++)
        mTargets[i]->appear();
}

void AllDeadWatcher::exeWait() {
    if (!isGreaterEqualStep(this, mSwitchOnDelayStep))
        return;

    kill();
    tryOnSwitchDeadOn(this);
}

void AllDeadWatcher::exeWatch() {
    for (s32 i = 0; i < mTargetCount; i++)
        if (isAlive(mTargets[i]))
            return;

    setNerve(this, &NrvAllDeadWatcher.Watch);
}

void AllDeadWatcher::init(const ActorInitInfo& info) {
    initActorSceneInfo(this, info);
    initStageSwitch(this, info);
    initExecutorWatchObj(this, info);
    initNerve(this, &NrvAllDeadWatcher.Watch, 0);

    mTargetCount = calcLinkChildNum(info, "WatchTargetEnemy");
    mTargets = new LiveActor*[mTargetCount];

    for (s32 i = 0; i < mTargetCount; i++)
        mTargets[i] = createLinksActorFromFactory(info, "WatchTargetEnemy", i);

    tryGetArg(&mSwitchOnDelayStep, info, "SwitchOnDelayStep");
    mIsNotControlActorAppear = tryGetBoolArgOrFalse(info, "IsNotControlActorAppear");

    if (!trySyncStageSwitchAppear(this) || mIsNotControlActorAppear)
        return;

    for (s32 i = 0; i < mTargetCount; i++)
        mTargets[i]->makeActorDead();
}

void AllDeadWatcher::kill() {
    LiveActor::kill();
}
}  // namespace al
