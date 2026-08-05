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

static s32 _cDefaultPriority = sead::Thread::cDefaultPriority;
static s32 cPriority = _cDefaultPriority + 6;

static void destroyResource(sead::FrameHeap** resource, const char* category) {
    al::removeResourceCategory(category);
    (*resource)->destroy();
    (*resource) = nullptr;
}

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

    if (mSandWorldHomeStageResource)
        destroyResource(&mSandWorldHomeStageResource, "砂ワールドホーム");

    if (mCityWorldHomeStageResource)
        destroyResource(&mCityWorldHomeStageResource, "都市ワールドホーム");

    if (mWorldExResource)
        destroyResource(&mWorldExResource, "ワールド常駐");

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
    if (mWorldExResource)
        destroyResource(&mWorldExResource, "ワールド常駐");
    if (!mSandWorldHomeStageResource) {
        // 280 MB
        sead::FrameHeap* heap = sead::FrameHeap::create(
            280 * 1024 * 1024, "SandWorldHomeStageResource", mWorldResourceHeap);
        mSandWorldHomeStageResource = heap;
        al::addResourceCategory("砂ワールドホーム", 1024, heap);
        s32 worldIndexSand = GameDataFunction::getWorldIndexSand();
        loadHomeStageResourceByWorld("砂ワールドホーム", mWorldResourceHeap, worldIndexSand, 8);
    }
    if (!mCityWorldHomeStageResource) {
        // 290 MB
        sead::FrameHeap* heap =
            sead::FrameHeap::create(290 * 1024 * 1024, "CityWorldHomeStageResource",
                                    mWorldResourceHeap, 8, sead::Heap::cHeapDirection_Reverse);
        mCityWorldHomeStageResource = heap;
        al::addResourceCategory("都市ワールドホーム", 1024, heap);
        s32 worldIndexCity = GameDataFunction::getWorldIndexCity();
        loadHomeStageResourceByWorld("都市ワールドホーム", heap, worldIndexCity, 12);
    }
}

void E3ResourceLoader::loadWorldResource() {
    al::setCurrentCategoryNameDefault();

    const u8* byml = al::tryGetBymlFromArcName("SystemData/WorldList", "WorldResource");
    al::ByamlIter byamlIter = al::ByamlIter(byml);

    al::ByamlIter worldIter;
    al::getByamlIterByIndex(&worldIter, byamlIter, mLoadWorldId);
    al::ByamlIter worldResourceIter;
    al::getByamlIterByKey(&worldResourceIter, worldIter, "WorldResource");

    s32 size = worldResourceIter.getSize();

    for (s32 i = 0; i < size; i++) {
        al::ByamlIter resourceIter;
        worldResourceIter.tryGetIterByIndex(&resourceIter, i);
        const char* name = nullptr;
        const char* ext = nullptr;
        resourceIter.tryGetStringByKey(&name, "Name");
        resourceIter.tryGetStringByKey(&ext, "Ext");

        if (!al::findResource(name))
            al::findOrCreateResource(name, ext);
        if (mIsCancelLoadWorldResource)
            return;
    }
    mIsLoaded = true;
}

void E3ResourceLoader::cancelLoadWorldResource() {
    mIsCancelLoadWorldResource = true;
}

bool E3ResourceLoader::requestLoadWorldHomeStageResource() {
    if (mSandWorldHomeStageResource && mCityWorldHomeStageResource &&
        isEndLoadWorldHomeStageResource())
        return false;

    if (!isEndLoadWorldHomeStageResource())
        return false;

    mIsCancelLoadWorldResource = false;
    mIsLoaded = false;
    mLoadWorldId = -1;
    al::clearFileLoaderEntry();
    mLoadHomeStageResourceThread->start();
    return true;
}

bool E3ResourceLoader::isEndLoadWorldHomeStageResource() const {
    return mLoadHomeStageResourceThread->isDone();
}

bool E3ResourceLoader::requestLoadWorldResource(s32 loadWorldId) {
    if (mLoadWorldId == loadWorldId && mIsLoaded)
        return false;
    if (!isEndLoadAny())
        return false;

    mIsCancelLoadWorldResource = false;
    al::clearFileLoaderEntry();
    mIsLoaded = false;
    mLoadWorldId = loadWorldId;
    mWorldResourceThread->start();
    return true;
}

bool E3ResourceLoader::isEndLoadAny() const {
    return mWorldResourceThread->isDone() && isEndLoadWorldHomeStageResource();
}

void E3ResourceLoader::tryCreateExHeap(s32 loadWorldId) {
    if (mWorldExResource)
        return;

    if (GameDataFunction::getWorldIndexSand() == loadWorldId)
        tryDestroyWorldResource(mCityWorldHomeStageResource);

    else if (GameDataFunction::getWorldIndexCity() == loadWorldId)
        tryDestroyWorldResource(mSandWorldHomeStageResource);

    mWorldExResource = sead::FrameHeap::create(0, "WorldExResource", mWorldResourceHeap, 8);

    al::addResourceCategory("ワールド常駐", 1024, mWorldExResource);
}

void E3ResourceLoader::tryDestroyWorldResource(sead::Heap* heap) {
    if (!heap)
        return;

    if (mSandWorldHomeStageResource == heap)
        destroyResource(&mSandWorldHomeStageResource, "砂ワールドホーム");

    if (mCityWorldHomeStageResource == heap)
        destroyResource(&mCityWorldHomeStageResource, "都市ワールドホーム");
}

void E3ResourceLoader::printHeapInfo() const {}

void E3ResourceLoader::loadHomeStageResourceByWorld(const char* categoryName, sead::Heap* _heap,
                                                    s32 worldId, s32 scenarioId) {
    const u8* byml = al::tryGetBymlFromArcName("SystemData/WorldList", "WorldResource");
    al::ByamlIter byamlIter = al::ByamlIter(byml);

    al::ByamlIter worldIter;
    al::getByamlIterByIndex(&worldIter, byamlIter, worldId);
    al::ByamlIter worldResourceIter;

    if (!al::tryGetByamlIterByKey(&worldResourceIter, worldIter,
                                  al::StringTmp<32>{"Scenario%d", scenarioId}.cstr()))
        return;

    al::setCurrentCategoryName(categoryName);

    s32 size = worldResourceIter.getSize();
    for (s32 i = 0; i < size; i++) {
        al::ByamlIter iter;
        worldResourceIter.tryGetIterByIndex(&iter, i);
        const char* name = nullptr;
        const char* ext = nullptr;
        iter.tryGetStringByKey(&name, "Name");
        iter.tryGetStringByKey(&ext, "Ext");

        al::findOrCreateResource(name, ext);
        if (mIsCancelLoadWorldResource)
            return;
    }
}
