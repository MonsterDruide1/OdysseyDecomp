#pragma once

#include <heap/seadFrameHeap.h>

#include "Library/Resource/Resource.h"
#include "Library/Thread/AsyncFunctorThread.h"

#include "System/GameDataHolder.h"

class WorldResourceLoader : public al::HioNode {
public:
    WorldResourceLoader(GameDataHolder* dataHolder);
    virtual ~WorldResourceLoader();
    void loadResource();
    void cancelLoadWorldResource();
    void tryDestroyWorldResource();
    bool requestLoadWorldHomeStageResource(s32 loadWorldId, s32 scenario);
    bool isEndLoadWorldResource() const;
    void requestLoadWorldResourceCommon(s32 loadWorldId);
    bool requestLoadWorldResource(s32 loadWorldId);
    void createResourcePlayer();
    void tryDestroyWorldResourceOnlyCap();
    f32 calcLoadPercent() const;
    s32 getLoadWorldId() const;
    al::Resource* tryLoadResource(const char* resPath, const char* ext, const char* category);
    void loadWorldResource(s32 loadWorldId, s32 scenario, bool isScenarioResources,
                           const char* resourceCategory);
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
