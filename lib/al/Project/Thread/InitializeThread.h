#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>
#include <thread/seadMessageQueue.h>

namespace sead {
class Heap;
class DelegateThread;
class Thread;
}  // namespace sead

namespace al {
class FunctorBase;

class InitializeThread {
public:
    InitializeThread(const sead::SafeString& threadName, const FunctorBase& functor,
                     sead::Heap* heap, s32 priority, s32 stackSize);
    void start();
    bool tryWaitDoneAndDestroy();

private:
    void threadFunction(sead::Thread* thread, sead::MessageQueue::Element message);

    sead::DelegateThread* mThread = nullptr;
    sead::Heap* mHeap = nullptr;
    FunctorBase* mFunctor = nullptr;
    bool mIsDone = false;
};
}  // namespace al
