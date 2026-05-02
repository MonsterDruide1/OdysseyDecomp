#pragma once

#include <heap/seadHeap.h>
#include <heap/seadHeapMgr.h>

namespace al {

class SceneHeapSetter : public sead::ScopedCurrentHeapSetter {
public:
    SceneHeapSetter();

    sead::Heap* getSceneHeap() const { return mSceneHeap; }

private:
    sead::Heap* mSceneHeap = nullptr;
};

}  // namespace al

static_assert(sizeof(al::SceneHeapSetter) == 0x10);
