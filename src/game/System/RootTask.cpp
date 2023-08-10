#include "game/System/RootTask.h"

#include <heap/seadHeapMgr.h>
#include "al/Library/Memory/HeapUtil.h"
#include "game/System/GameSystem.h"

void RootTask::enter() {}
void RootTask::calc() {
    if (!mGameSystem) {
        sead::ScopedCurrentHeapSetter heapSetter(al::getStationedHeap());
        mGameSystem = new GameSystem();
        mGameSystem->init();
    }
    mGameSystem->movement();
}
