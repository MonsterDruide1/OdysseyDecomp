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
    bool requestLoadWorldResource(s32 loadWorldId);
    bool isEndLoadAny() const;
    void tryCreateExHeap(s32 loadWorldId);
    void tryDestroyWorldResource(sead::Heap* heap);
    void printHeapInfo() const;
    void loadHomeStageResourceByWorld(const char* categoryName, sead::Heap* _heap, s32 worldId,
                                      s32 scenarioId);

private:
    al::AsyncFunctorThread* mLoadHomeStageResourceThread = nullptr;
    al::AsyncFunctorThread* mWorldResourceThread = nullptr;
    sead::Heap* mWorldResourceHeap = nullptr;
    sead::FrameHeap* mSandWorldHomeStageResource = nullptr;
    sead::FrameHeap* mCityWorldHomeStageResource = nullptr;
    sead::FrameHeap* mWorldExResource = nullptr;
    bool mIsCancelLoadWorldResource = false;
    bool mIsLoaded = false;
    bool mHasCreatedResourceCategory = false;
    s32 mLoadWorldId = -1;
};

static_assert(sizeof(E3ResourceLoader) == 0x40);
