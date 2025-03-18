#pragma once

#include <basis/seadTypes.h>

#include "Library/Thread/AsyncFunctorThread.h"

class GameDataHolder;
namespace al {
class AsyncFunctorThread;
class Resource;
}  // namespace al
namespace sead {
class FrameHeap;
class Heap;
}  // namespace sead

class WorldResourceLoader {
public:
    WorldResourceLoader(GameDataHolder*);
    virtual ~WorldResourceLoader();
    void loadResource();
    void cancelLoadWorldResource();
    void tryDestroyWorldResource();
    bool requestLoadWorldHomeStageResource(s32 worldIndex, s32 scenario);
    bool isEndLoadWorldResource() const;
    void requestLoadWorldResourceCommon(s32);
    bool requestLoadWorldResource(s32);
    void createResourcePlayer();
    void tryDestroyWorldResourceOnlyCap();
    f32 calcLoadPercent() const;
    s32 getLoadWorldId() const;
    al::Resource* tryLoadResource(const char*, const char*, const char*);
    void loadWorldResource(s32, s32, bool, const char*);
    f32 calcWorldResourceHeapSize() const;

private:
    al::AsyncFunctorThread* mWorldResourceLoader = nullptr;  // WorldResourceLoader::loadResource
    sead::Heap* mWorldResourceHeap = nullptr;
    sead::FrameHeap* mCapWorldHeap = nullptr;
    sead::FrameHeap* mWaterfallWorldHeap = nullptr;
    s32 mLoadWorldId = -1;
    s32 mScenarioNo = -1;
    bool mIsScenarioRes = true;
    bool mIsCancelled = false;
    void* unkPtr4 = nullptr;
    void* unkPtr5 = nullptr;
    s32 unkInt3 = 0;
    bool mIsLoadResTable = true;
    s32 mCurLoadCount = 0;
    s32 mMaxLoadCount = 1;
    GameDataHolder* mDataHolder = nullptr;
    bool mIsLoadedPlayerModel = false;
    s32 unkInt6 = 0;
};
