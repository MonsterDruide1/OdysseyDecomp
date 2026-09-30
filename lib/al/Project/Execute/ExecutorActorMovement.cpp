#include "Project/Execute/ExecutorActorMovement.h"

#include "Library/LiveActor/LiveActor.h"

namespace al {

ExecutorActorMovement::ExecutorActorMovement(const char* name) : ExecutorActorExecuteBase(name) {}

void ExecutorActorMovement::execute() const {
    s32 actorCount = getActorCount();
    for (s32 i = 0; i < actorCount; i++)
        getActorArray()[i]->movement();
}

}  // namespace al
