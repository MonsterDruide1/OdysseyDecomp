#include "al/Library/Execute/ActorExecuteInfo.h"

namespace al {
ActorExecuteInfo::ActorExecuteInfo(ExecuteRequestKeeper* keeper) {
    mRequestKeeper = keeper;
}
void ActorExecuteInfo::addUpdater(ExecutorActorExecuteBase* updater) {
    mUpdaters[mUpdaterCount] = updater;
    mUpdaterCount++;
}
void ActorExecuteInfo::addDrawer(ModelDrawerBase* drawer) {
    mDrawers[mDrawerCount] = drawer;
    mDrawerCount++;
}
}  // namespace al
