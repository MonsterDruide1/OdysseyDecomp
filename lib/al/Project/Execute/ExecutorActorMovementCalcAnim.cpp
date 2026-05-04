#include "Project/Execute/ExecutorActorMovementCalcAnim.h"

#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/LiveActor.h"

namespace al {

ExecutorActorMovementCalcAnim::ExecutorActorMovementCalcAnim(const char* name)
    : ExecutorActorExecuteBase(name) {}

void ExecutorActorMovementCalcAnim::execute() const {
    s32 actorCount = getActorCount();
    for (s32 i = 0; i < actorCount; i++) {
        getActorArray()[i]->movement();
        LiveActor* actor = getActorArray()[i];
        if (!isDead(actor))
            actor->calcAnim();
    }
}

}  // namespace al
