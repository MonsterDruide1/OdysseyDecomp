#include "Sequence/E3ResourceLoader.h"

#include <heap/seadFrameHeap.h>
#include <thread/seadThread.h>

#include "Library/Base/StringUtil.h"
#include "Library/File/FileUtil.h"
#include "Library/Memory/HeapUtil.h"
#include "Library/Resource/ResourceFunction.h"
#include "Library/Thread/AsyncFunctorThread.h"
#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/ByamlUtil.h"

#include "System/GameDataFunction.h"

s32 cPriority = sead::Thread::cDefaultPriority + 6;
static s32 _cDefaultPriority = sead::Thread::cDefaultPriority;

// NON_MATCHING: Compiler creates the first functor in the wrong place
// https://decomp.me/scratch/fyi2M
E3ResourceLoader::E3ResourceLoader() {
    using E3ResourceLoaderFunctor = al::FunctorV0M<E3ResourceLoader*, void (E3ResourceLoader::*)()>;

    mLoadHomeStageResourceThread = new al::AsyncFunctorThread(
        "LoadHomeStageResourceThread",
        E3ResourceLoaderFunctor(this, &E3ResourceLoader::loadHomeStageResource), cPriority,
        0x100000, sead::CoreId::cMain);

    mWorldResourceThread = new al::AsyncFunctorThread(
        "WolrdResourceThread", E3ResourceLoaderFunctor(this, &E3ResourceLoader::loadWorldResource),
        cPriority, 0x100000, sead::CoreId::cMain);

    al::createWorldResourceHeap(false);
    mWorldResourceHeap = al::getWorldResourceHeap();
}

E3ResourceLoader::~E3ResourceLoader() {
    cancelLoadWorldResource();

    if (mLoadHomeStageResourceThread) {
        delete mLoadHomeStageResourceThread;
        mLoadHomeStageResourceThread = nullptr;
    }

    if (mWorldResourceThread) {
        delete mWorldResourceThread;
        mWorldResourceThread = nullptr;
    }

    al::removeResourceCategory("E3常駐");

    if (mSandWorldHomeStageResource) {
        al::removeResourceCategory("砂ワールドホーム");
        mSandWorldHomeStageResource->destroy();
        mSandWorldHomeStageResource = nullptr;
    }

    if (mCityWorldHomeStageResource) {
        al::removeResourceCategory("都市ワールドホーム");
        mCityWorldHomeStageResource->destroy();
        mCityWorldHomeStageResource = nullptr;
    }

    if (mWorldExResource) {
        al::removeResourceCategory("ワールド常駐");
        mWorldExResource->destroy();
        mWorldExResource = nullptr;
    }

    if (mWorldResourceHeap) {
        al::destroyWorldResourceHeap(false);
        mWorldResourceHeap = nullptr;
    }

    al::resetCurrentCategoryName();
}

void E3ResourceLoader::loadHomeStageResource() {
    if (!mHasCreatedResourceCategory) {
        al::createCategoryResourceAll("プレイヤーモデル");
        al::addResourceCategory("E3常駐", 0x400, mWorldResourceHeap);
        al::createCategoryResourceAll("E3常駐");
        mHasCreatedResourceCategory = true;
    }
    if (mWorldExResource) {
        al::removeResourceCategory("ワールド常駐");
        mWorldExResource->destroy();
        mWorldExResource = nullptr;
    }
    if (!mSandWorldHomeStageResource) {
        // 280 MB
        sead::FrameHeap* heap = sead::FrameHeap::create(
            280 * 1024 * 1024, "SandWorldHomeStageResource", mWorldResourceHeap, 8,
            sead::Heap::cHeapDirection_Forward, false);
        mSandWorldHomeStageResource = heap;
        al::addResourceCategory("砂ワールドホーム", 512, heap);
        s32 worldIndexSand = GameDataFunction::getWorldIndexSand();
        loadHomeStageResourceByWorld("砂ワールドホーム", mWorldResourceHeap, worldIndexSand, 8);
    }
    if (!mCityWorldHomeStageResource) {
        // 290 MB
        sead::FrameHeap* heap = sead::FrameHeap::create(
            290 * 1024 * 1024, "CityWorldHomeStageResource", mWorldResourceHeap, 8,
            sead::Heap::cHeapDirection_Reverse, false);
        mCityWorldHomeStageResource = heap;
        al::addResourceCategory("都市ワールドホーム", 512, heap);
        s32 worldIndexCity = GameDataFunction::getWorldIndexCity();
        loadHomeStageResourceByWorld("都市ワールドホーム", heap, worldIndexCity, 12);
    }
}

void E3ResourceLoader::loadWorldResource() {
    al::setCurrentCategoryNameDefault();

    const u8* byml = (al::tryGetBymlFromArcName("SystemData/WorldList", "WorldResource"));
    al::ByamlIter byamlIter = al::ByamlIter(byml);

    al::ByamlIter worldIter;
    al::getByamlIterByIndex(&worldIter, byamlIter, mLoadWorldId);
    al::ByamlIter worldResourceIter;
    al::getByamlIterByKey(&worldResourceIter, worldIter, "WorldResource");

    s32 size = worldResourceIter.getSize();

    for (s32 i = 0; i < size; i++) {
        al::ByamlIter v8;
        worldResourceIter.tryGetIterByIndex(&v8, i);
        const char* name = nullptr;
        const char* ext = nullptr;
        v8.tryGetStringByKey(&name, "Name");
        v8.tryGetStringByKey(&ext, "Ext");

        if (!al::findResource(name))
            al::findOrCreateResource(name, ext);
        if (mHasLoadedWorldResource)
            return;
    }
    mIsLoaded = true;
}

void E3ResourceLoader::cancelLoadWorldResource() {
    mHasLoadedWorldResource = true;
}

bool E3ResourceLoader::requestLoadWorldHomeStageResource() {
    if (mSandWorldHomeStageResource && mCityWorldHomeStageResource &&
        isEndLoadWorldHomeStageResource())
        return false;
    if (isEndLoadWorldHomeStageResource()) {
        mHasLoadedWorldResource = false;
        mIsLoaded = false;
        mLoadWorldId = -1;
        al::clearFileLoaderEntry();
        mLoadHomeStageResourceThread->start();
        return true;
    }
    return false;
}

bool E3ResourceLoader::isEndLoadWorldHomeStageResource() const {
    return mLoadHomeStageResourceThread->isDone();
}

bool E3ResourceLoader::requestLoadWorldResource(s32 loadWorldId) {
    if (mLoadWorldId == loadWorldId && mIsLoaded)
        return false;
    if (isEndLoadAny()) {
        mHasLoadedWorldResource = false;
        al::clearFileLoaderEntry();
        mIsLoaded = false;
        mLoadWorldId = loadWorldId;
        mWorldResourceThread->start();
        return true;
    }
    return false;
}

bool E3ResourceLoader::isEndLoadAny() const {
    return mWorldResourceThread->isDone() && isEndLoadWorldHomeStageResource();
}

void E3ResourceLoader::tryCreateExHeap(s32 loadWorldId) {
    if (mWorldExResource)
        return;

    if (GameDataFunction::getWorldIndexSand() == loadWorldId) {
        sead::FrameHeap* cityResource = mCityWorldHomeStageResource;

        if (cityResource) {
            if (mSandWorldHomeStageResource == cityResource) {
                al::removeResourceCategory("砂ワールドホーム");
                mSandWorldHomeStageResource->destroy();
                mSandWorldHomeStageResource = nullptr;
            }
            if (mCityWorldHomeStageResource == cityResource) {
                al::removeResourceCategory("都市ワールドホーム");
                mCityWorldHomeStageResource->destroy();
                mCityWorldHomeStageResource = nullptr;
            }
        }

    } else if (GameDataFunction::getWorldIndexCity() == loadWorldId) {
        sead::FrameHeap* sandResource = mSandWorldHomeStageResource;

        if (sandResource) {
            al::removeResourceCategory("砂ワールドホーム");
            mSandWorldHomeStageResource->destroy();
            mSandWorldHomeStageResource = nullptr;

            if (mCityWorldHomeStageResource == sandResource) {
                al::removeResourceCategory("都市ワールドホーム");
                mCityWorldHomeStageResource->destroy();
                mCityWorldHomeStageResource = nullptr;
            }
        }
    }

    sead::FrameHeap* newHeap = sead::FrameHeap::create(0, "WorldExResource", mWorldResourceHeap, 8,
                                                       sead::Heap::cHeapDirection_Forward, false);
    mWorldExResource = newHeap;

    al::addResourceCategory("ワールド常駐", 512, newHeap);
}

void E3ResourceLoader::tryDestroyWorldResource(sead::Heap* heap) {
    if (!heap)
        return;

    if (mSandWorldHomeStageResource == heap) {
        al::removeResourceCategory("砂ワールドホーム");
        mSandWorldHomeStageResource->destroy();
        mSandWorldHomeStageResource = nullptr;
    }

    if (mCityWorldHomeStageResource == heap) {
        al::removeResourceCategory("都市ワールドホーム");
        mCityWorldHomeStageResource->destroy();
        mCityWorldHomeStageResource = nullptr;
    }
}

void E3ResourceLoader::printHeapInfo() const {}

void E3ResourceLoader::loadHomeStageResourceByWorld(const char* worldName, sead::Heap* _heap,
                                                    s32 loadWorldId, s32 scenarioId) {
    const u8* byml = al::tryGetBymlFromArcName("SystemData/WorldList", "WorldResource");
    al::ByamlIter byamlIter = al::ByamlIter(byml);

    al::ByamlIter worldIter;
    al::getByamlIterByIndex(&worldIter, byamlIter, loadWorldId);
    al::ByamlIter worldResourceIter;

    if (!al::tryGetByamlIterByKey(&worldResourceIter, worldIter,
                                  al::StringTmp<32>{"Scenario%d", scenarioId}.cstr()))
        return;

    al::setCurrentCategoryName(worldName);

    s32 size = worldResourceIter.getSize();
    for (s32 i = 0; i < size; i++) {
        al::ByamlIter iter;
        worldResourceIter.tryGetIterByIndex(&iter, i);
        const char* name = nullptr;
        const char* ext = nullptr;
        iter.tryGetStringByKey(&name, "Name");
        iter.tryGetStringByKey(&ext, "Ext");

        al::findOrCreateResource(name, ext);
        if (mHasLoadedWorldResource)
            return;
    }
}
