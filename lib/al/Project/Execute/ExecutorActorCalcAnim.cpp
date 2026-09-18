#include "Project/Execute/ExecutorActorCalcAnim.h"

#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/LiveActor.h"

namespace al {

ExecutorActorCalcAnim::ExecutorActorCalcAnim(const char* name) : ExecutorActorExecuteBase(name) {}

void ExecutorActorCalcAnim::execute() const {
    s32 actorCount = getActorCount();
    for (s32 i = 0; i < actorCount; i++) {
        LiveActor* actor = getActorArray()[i];
        if (!isDead(actor))
            actor->calcAnim();
    }
}

}  // namespace al
