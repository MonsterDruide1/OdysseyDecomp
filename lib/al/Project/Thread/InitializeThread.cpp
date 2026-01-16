#include "Project/Thread/InitializeThread.h"

#include <thread/seadDelegateThread.h>

#include "Library/Thread/FunctorV0M.h"

namespace al {
InitializeThread::InitializeThread(const sead::SafeString& threadName, const FunctorBase& functor,
                                   sead::Heap* heap, s32 priority, s32 stackSize)
    : mHeap(heap) {
    mThread = new sead::DelegateThread(
        threadName,
        new sead::Delegate2<InitializeThread, sead::Thread*, sead::MessageQueue::Element>(
            this, &InitializeThread::threadFunction),
        heap, priority, sead::MessageQueue::BlockType::NonBlocking, 0x7fffffff, stackSize, 0x20);

    mFunctor = functor.clone();
}

void InitializeThread::start() {
    mThread->start();
    mIsDone = false;
}

bool InitializeThread::tryWaitDoneAndDestroy() {
    if (mIsDone)
        return true;

    if (mThread->isDone()) {
        mThread->destroy();
        mIsDone = true;
        return true;
    }

    return false;
}

void InitializeThread::threadFunction(sead::Thread* thread, sead::MessageQueue::Element message) {
    sead::ScopedCurrentHeapSetter heapSetter(mHeap);

    (*mFunctor)();
    mThread->quit(false);
}
}  // namespace al
