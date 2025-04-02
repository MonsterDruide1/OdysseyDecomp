#include "Library/LiveActor/ActorInitFunction.h"

#include "Library/LiveActor/LiveActor.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

namespace al {
bool trySyncStageSwitchKill(LiveActor* actor) {
    using LiveActorFunctor = FunctorV0M<LiveActor*, void (LiveActor::*)()>;

    bool result = listenStageSwitchOnOffKill(actor, LiveActorFunctor(actor, &LiveActor::kill),
                                             LiveActorFunctor(actor, &LiveActor::appear));
    actor->makeActorAlive();

    return result;
}
}  // namespace al
