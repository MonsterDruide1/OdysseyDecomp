#include "Sequence/WorldResourceLoader.h"

#include <thread/seadThread.h>

#include "Library/Base/StringUtil.h"
#include "Library/File/FileUtil.h"
#include "Library/Memory/HeapUtil.h"
#include "Library/Resource/ResourceHolder.h"
#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/ByamlUtil.h"

s32 priority = sead::Thread::cDefaultPriority;

// for some reason tools/check doesn't show this?
WorldResourceLoader::WorldResourceLoader(GameDataHolder* dataHolder) {
    mDataHolder = dataHolder;

    auto functor = al::FunctorV0M<WorldResourceLoader*, void (WorldResourceLoader::*)()>(
        this, &WorldResourceLoader::loadResource);

    mWorldResourceLoader = new al::AsyncFunctorThread("WorldResourceLoader", functor, priority,
                                                      0x100000, sead::CoreId::cMain);
}
WorldResourceLoader::~WorldResourceLoader() {
    mIsCancelled = true;
    mCurLoadCount = 0;

    if (mWorldResourceLoader) {
        delete mWorldResourceLoader;
    }

    tryDestroyWorldResource();
}

void WorldResourceLoader::loadResource() {}

void WorldResourceLoader::cancelLoadWorldResource() {
    mIsCancelled = true;
    mCurLoadCount = 0;
}

void WorldResourceLoader::tryDestroyWorldResource() {
    if (mWorldResourceHeap) {
        bool isUseSpecialHeap;

        if (mCapWorldHeap) {
            al::removeResourceCategory("ワールド常駐");
            mCapWorldHeap->destroy();
            mCapWorldHeap = nullptr;
            al::removeNamedHeap("WorldResourceCap");
            isUseSpecialHeap = false;
        } else {
            isUseSpecialHeap = true;
        }

        if (mWaterfallWorldHeap) {
            al::removeResourceCategory("ホーム常駐[Waterfall]");
            mWaterfallWorldHeap->destroy();
            mWaterfallWorldHeap = nullptr;
            al::removeNamedHeap("WorldResourceWaterfall");
        }

        al::destroyWorldResourceHeap(isUseSpecialHeap);
        mWorldResourceHeap = nullptr;
        mLoadWorldId = -1;
    }

    al::resetCurrentCategoryName();
}

bool WorldResourceLoader::requestLoadWorldHomeStageResource(s32 loadWorldId, s32 scenario) {
    if (mWaterfallWorldHeap)
        return false;
    if (!isEndLoadWorldResource())
        return false;
    if (mLoadWorldId == loadWorldId)
        return false;
    if (!al::tryGetBymlFromArcName("SystemData/WorldList", "WorldResource"))
        return false;
    if (!mWaterfallWorldHeap)
        tryDestroyWorldResource();

    if (loadWorldId || scenario != 1) {
        if (!mWorldResourceHeap) {
            al::createWorldResourceHeap(true);
            mWorldResourceHeap = al::getWorldResourceHeap();
        }
    } else {
        al::createWorldResourceHeap(false);

        mWorldResourceHeap = al::getWorldResourceHeap();

        mCapWorldHeap =
            sead::FrameHeap::create(0x1F400000, "ワールド常駐[帽子]", mWorldResourceHeap, 8,
                                    sead::Heap::HeapDirection::cHeapDirection_Forward, false);
        al::addNamedHeap(mCapWorldHeap, "WorldResourceCap");

        mWaterfallWorldHeap =
            sead::FrameHeap::create(0, "ワールド常駐[滝]", mWorldResourceHeap, 8,
                                    sead::Heap::HeapDirection::cHeapDirection_Reverse, false);
        al::addResourceCategory("ホーム常駐[Waterfall]", 512, mWaterfallWorldHeap);
        al::addNamedHeap(mWaterfallWorldHeap, "WorldResourceWaterfall");

        al::addResourceCategory("ワールド常駐", 512, mCapWorldHeap);
    }

    if (loadWorldId < 0)
        return false;

    mScenarioNo = scenario;
    mIsScenarioRes = true;
    unkInt3 = 0;
    unkPtr4 = nullptr;
    unkPtr5 = nullptr;
    requestLoadWorldResourceCommon(loadWorldId);
    return true;
}

bool WorldResourceLoader::isEndLoadWorldResource() const {
    return mWorldResourceLoader->isDone();
}

void WorldResourceLoader::requestLoadWorldResourceCommon(s32 loadWorldId) {
    mLoadWorldId = loadWorldId;
    mIsCancelled = false;
    mWorldResourceLoader->start();
}

bool WorldResourceLoader::requestLoadWorldResource(s32 loadWorldId) {
    if (!isEndLoadWorldResource())
        return false;

    mIsScenarioRes = false;
    requestLoadWorldResourceCommon(loadWorldId);
    return true;
}
// unused func
void WorldResourceLoader::createResourcePlayer() {}

void WorldResourceLoader::tryDestroyWorldResourceOnlyCap() {
    if (mWorldResourceHeap) {
        if (mCapWorldHeap) {
            al::removeResourceCategory("ワールド常駐");
            mCapWorldHeap->destroy();
            mCapWorldHeap = nullptr;
            al::removeNamedHeap("WorldResourceCap");
        }
        al::addResourceCategory("ワールド常駐", 512, mWorldResourceHeap);
        mLoadWorldId = -1;
    }
    al::clearFileLoaderEntry();
    al::resetCurrentCategoryName();
}

f32 WorldResourceLoader::calcLoadPercent() const {
    if (mCurLoadCount >= mMaxLoadCount)
        return 101.0f;
    else
        return (mCurLoadCount * 100.0f) / mMaxLoadCount;
}

s32 WorldResourceLoader::getLoadWorldId() const {
    return mLoadWorldId;
}

al::Resource* WorldResourceLoader::tryLoadResource(const char* resPath, const char* category,
                                                   const char* str3) {
    if (category) {
        if (str3) {
            return al::findOrCreateResourceCategory(resPath, str3, category);
        }
        return al::findOrCreateResource(resPath, category);
    } else {
        return al::findOrCreateResource(resPath, nullptr);
    }
}

void WorldResourceLoader::loadWorldResource(s32 loadWorldId, s32 scenario, bool isScenarioResources,
                                            const char* resourceCategory) {
    nn::os::GetSystemTick();
    nn::os::GetSystemTick();

    u8* bymlData = al::tryGetBymlFromArcName("SystemData/WorldList", "WorldResource");

    al::ByamlIter worldResourceIter(bymlData);
    al::ByamlIter loadWorldIter;
    al::getByamlIterByIndex(&loadWorldIter, worldResourceIter, loadWorldId);
    al::ByamlIter resourceListIter;

    if (isScenarioResources) {
        if (!al::tryGetByamlIterByKey(&resourceListIter, loadWorldIter,
                                      al::StringTmp<32>("Scenario%d", scenario).cstr()))
            return;
    } else {
        al::getByamlIterByKey(&resourceListIter, loadWorldIter, "WorldResource");
    }

    s32 resSize = resourceListIter.getSize();
    mMaxLoadCount = resSize;

    if (resSize > 0) {
        if (resourceCategory) {
            for (s32 i = 0; i < resSize; i++) {
                al::ByamlIter resEntry;
                resourceListIter.tryGetIterByIndex(&resEntry, i);

                const char* resName = nullptr;
                const char* resExt = nullptr;
                resEntry.tryGetStringByKey(&resName, "Name");
                resEntry.tryGetStringByKey(&resExt, "Ext");

                if (resExt) {
                    al::findOrCreateResourceCategory(resName, resourceCategory, resExt);
                } else {
                    al::findOrCreateResource(resName, nullptr);
                }

                if (mIsCancelled)
                    return;

                mCurLoadCount = i;
            }
        } else {
            for (s32 i = 0; i < resSize; i++) {
                al::ByamlIter resEntry;
                resourceListIter.tryGetIterByIndex(&resEntry, i);

                const char* resName = nullptr;
                const char* resExt = nullptr;
                resEntry.tryGetStringByKey(&resName, "Name");
                resEntry.tryGetStringByKey(&resExt, "Ext");

                al::findOrCreateResource(resName, resExt);

                if (mIsCancelled)
                    return;

                mCurLoadCount = i;
            }
        }
    }

    mCurLoadCount = mMaxLoadCount;
}

f32 WorldResourceLoader::calcWorldResourceHeapSize() const {
    return ((mWorldResourceHeap->getSize() - mWorldResourceHeap->getFreeSize()) * 0.00097656f) *
           0.00097656f;
}
