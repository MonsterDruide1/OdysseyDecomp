#pragma once

#include <heap/seadFrameHeap.h>

namespace al {
class AsyncFunctorThread;
}

class E3ResourceLoader {
public:
    E3ResourceLoader();
    virtual ~E3ResourceLoader();

    void loadHomeStageResource();
    void loadWorldResource();
    void cancelLoadWorldResource();
    bool requestLoadWorldHomeStageResource();
    bool isEndLoadWorldHomeStageResource() const;
    bool requestLoadWorldResource(s32);
    bool isEndLoadAny() const;
    void tryCreateExHeap(s32);
    void tryDestroyWorldResource(sead::Heap*);
    void printHeapInfo() const;
    void loadHomeStageResourceByWorld(const char*, sead::Heap*, s32, s32);

private:
    al::AsyncFunctorThread* mLoadHomeStageResourceThread = nullptr;
    al::AsyncFunctorThread* mWorldResourceThread = nullptr;
    sead::Heap* mWorldResourceHeap = nullptr;
    sead::FrameHeap* mSandWorldHomeStageResource = nullptr;
    sead::FrameHeap* mCityWorldHomeStageResource = nullptr;
    sead::FrameHeap* mWorldExResource = nullptr;
    bool mHasLoadedWorldResource = false;
    bool _39 = false;
    bool _30 = false;
    s32 _3c = -1;
};
