#include "Library/Execute/ExecutorListFunctor.h"

#include "Library/Thread/FunctorV0M.h"

namespace al {
ExecutorListFunctor::ExecutorListFunctor(const char* name, s32 size) : ExecutorListBase(name) {
    mList.allocBuffer(size, nullptr);
}

bool ExecutorListFunctor::isActive() const {
    return mList.size() != 0;
}

void ExecutorListFunctor::executeList() const {
    for (s32 i = 0; i < mList.size(); i++)
        (*mList[i])();
}

void ExecutorListFunctor::registerFunctor(const FunctorBase& functor) {
    mList.pushBack(functor.clone());
}

}  // namespace al
