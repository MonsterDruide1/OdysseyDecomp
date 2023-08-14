#include "Library/Thread/AsyncFunctorThread.h"

#include <prim/seadDelegate.h>
#include <thread/seadDelegateThread.h>
#include <thread/seadMessageQueue.h>
#include <thread/seadThread.h>

namespace al {

AsyncFunctorThread::AsyncFunctorThread(const sead::SafeString& functor_name,
                                       const al::FunctorBase& functor, s32 priority, s32 stack_size,
                                       sead::CoreId id) {
    s32 size = stack_size < 0 ? 4096 : stack_size;
    mDelegateThread = new sead::DelegateThread(
        functor_name,
        new sead::Delegate2<AsyncFunctorThread, sead::Thread*, sead::MessageQueue::Element>(
            this, &AsyncFunctorThread::threadFunction),
        nullptr, priority, sead::MessageQueue::BlockType::Blocking, 0x7FFFFFFF, size, 4);

    if (id)
        mDelegateThread->setAffinity(sead::CoreIdMask(id));

    mFunctor = functor.clone();
    mDelegateThread->start();
}

AsyncFunctorThread::~AsyncFunctorThread() {
    mDelegateThread->quitAndWaitDoneSingleThread(false);
}

void AsyncFunctorThread::threadFunction(sead::Thread* unused_1, s64 unused_2) {
    (*mFunctor)();
    mIsDone = true;
}

void AsyncFunctorThread::start() {
    mDelegateThread->sendMessage(1, sead::MessageQueue::BlockType::NonBlocking);
    mIsDone = false;
}

bool AsyncFunctorThread::isDone() const {
    return mIsDone;
}

}  // namespace al
