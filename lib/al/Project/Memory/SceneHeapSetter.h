#pragma once

#include <heap/seadHeapMgr.h>

namespace al {

class SceneHeapSetter : public sead::ScopedCurrentHeapSetter {
public:
    explicit SceneHeapSetter();

    sead::Heap* getSceneHeap() const { return mSceneHeap; }

private:
    sead::Heap* mSceneHeap = nullptr;
};

}  // namespace al
