#include "Project/Execute/ExecutorActorCalcView.h"

#include "Library/LiveActor/ActorModelFunction.h"

namespace al {

ExecutorActorCalcView::ExecutorActorCalcView(const char* name) : ExecutorActorExecuteBase(name) {}

void ExecutorActorCalcView::execute() const {
    s32 actorCount = getActorCount();
    for (s32 i = 0; i < actorCount; i++)
        calcViewModelSystem(getActorArray()[i]);
}

}  // namespace al
