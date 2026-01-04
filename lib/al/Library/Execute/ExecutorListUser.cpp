#include "Library/Execute/ExecutorListUser.h"

#include "Library/Execute/IUseExecutor.h"

namespace al {
ExecutorListIUseExecutorBase::ExecutorListIUseExecutorBase(const char* name, s32 size)
    : ExecutorListBase(name), mCapacity(size) {
    mList = new IUseExecutor*[size];
    for (s32 i = 0; i < mCapacity; i++)
        mList[i] = nullptr;
}

bool ExecutorListIUseExecutorBase::isActive() const {
    return mSize > 0;
}

void ExecutorListIUseExecutorBase::registerUser(IUseExecutor* user) {
    mList[mSize] = user;
    mSize++;
}

ExecutorListIUseExecutorUpdate::ExecutorListIUseExecutorUpdate(const char* name, s32 size)
    : ExecutorListIUseExecutorBase(name, size) {}

void ExecutorListIUseExecutorUpdate::executeList() const {
    for (s32 i = 0; i < getSize(); i++)
        getExecutor(i)->execute();
}

ExecutorListIUseExecutorDraw::ExecutorListIUseExecutorDraw(const char* name, s32 size)
    : ExecutorListIUseExecutorBase(name, size) {}

void ExecutorListIUseExecutorDraw::executeList() const {
    for (s32 i = 0; i < getSize(); i++)
        getExecutor(i)->draw();
}

}  // namespace al
