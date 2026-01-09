#include "Library/Execute/ExecutorListActorExecute.h"

#include "Library/Base/StringUtil.h"
#include "Library/Execute/ExecutorActorExecuteBase.h"
#include "Library/LiveActor/LiveActor.h"

namespace al {

ExecutorListActorExecuteBase::ExecutorListActorExecuteBase(const char* name, s32 size)
    : ExecutorListBase(name), mCapacity(size) {
    mList = new ExecutorActorExecuteBase*[size];
    for (s32 i = 0; i < mCapacity; i++)
        mList[i] = nullptr;
}

bool ExecutorListActorExecuteBase::isActive() const {
    return mSize > 0;
}

void ExecutorListActorExecuteBase::executeList() const {
    for (s32 i = 0; i < mSize; i++)
        mList[i]->execute();
}

void ExecutorListActorExecuteBase::registerActor(LiveActor* actor) {
    for (s32 i = 0; i < mSize; i++) {
        ExecutorActorExecuteBase* executor = mList[i];
        if (isEqualString(executor->getName(), actor->getName())) {
            executor->registerActor(actor);
            return;
        }
    }

    ExecutorActorExecuteBase* executor = createExecutor(actor->getName());
    executor->registerActor(actor);
    mList[mSize] = executor;
    mSize++;
}

void ExecutorListActorExecuteBase::createList() {
    for (s32 i = 0; i < mSize; i++)
        mList[i]->createExecutorTable();
}

}  // namespace al
