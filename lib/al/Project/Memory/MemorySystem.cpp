#include "Project/Memory/MemorySystem.h"

#include <filedevice/seadFileDeviceMgr.h>
#include <heap/seadHeapMgr.h>

#include "Library/Base/StringUtil.h"
#include "Library/Resource/Resource.h"
#include "Library/Resource/ResourceUtil.h"
#include "Library/Yaml/ByamlIter.h"
#include "Project/Memory/SceneHeapSetter.h"
#include "basis/seadRawPrint.h"
#include "heap/seadFrameHeap.h"

namespace al {

MemorySystem::MemorySystem(sead::Heap* heap)
    : mDelegate(this, &MemorySystem::allocFailedCallbackFunc) {
    sead::HeapMgr::instance()->setAllocFailedCallback(&mDelegate);
    mStationedHeap =
        sead::ExpHeap::tryCreate((u32)heap->getMaxAllocatableSize(8) - 0x1900000, "StationedHeap",
                                 heap, 8, sead::Heap::cHeapDirection_Forward, false);

    {
        sead::ScopedCurrentHeapSetter setter(mStationedHeap);
        {
            sead::ScopedCurrentHeapSetter setter(mStationedHeap);
            mHeapList.allocBuffer(32, nullptr);
        }

        mIsExistFileResource = sead::FileDeviceMgr::instance()->getMainFileDevice()->isExistFile(
            al::StringTmp<64>("%s.szs", "SystemData/MemorySystem"));
    }
}

void MemorySystem::allocFailedCallbackFunc(const sead::HeapMgr::AllocFailedCallbackArg* arg) {
    if (al::isEqualString(arg->heap->getName().cstr(), "gpu"))
        return;
    arg->heap->dump();
    sead::system::Halt();
}

void MemorySystem::createSequenceHeap() {
    mSequenceHeap = sead::ExpHeap::tryCreate(0, "SequenceHeap", nullptr, 8,
                                             sead::Heap::cHeapDirection_Forward, false);
}

void MemorySystem::freeAllSequenceHeap() {
    mSequenceHeap->freeAll();
}

void MemorySystem::printSequenceHeap() {
    mSequenceHeap->dumpFreeList();
    mSequenceHeap->dumpUseList();
    mSequenceHeap->dump();
}

bool MemorySystem::createSceneHeap(const char* stageName, bool backwards) {
    sead::Heap* currentSceneResourceHeap = mSceneResourceHeap;
    if (!currentSceneResourceHeap)
        createSceneResourceHeap(stageName, backwards);

    u64 size = 0x1EA00000;
    sead::Heap::HeapDirection direction =
        backwards ? sead::Heap::cHeapDirection_Reverse : sead::Heap::cHeapDirection_Forward;

    sead::Heap* currentHeap = al::getCurrentHeap();
    if (currentHeap && currentHeap->getMaxAllocatableSize(8) < size)
        size = currentHeap->getMaxAllocatableSize(8);

    mSceneHeap = sead::ExpHeap::tryCreate(size, "SceneHeap", nullptr, 8, direction, false);
    mSceneHeap->mFlag.reset(sead::Heap::Flag::cEnableDebugFillUser);

    return currentSceneResourceHeap == nullptr;
}

// NON_MATCHING
void MemorySystem::createSceneResourceHeap(const char* stageName, bool backwards) {
    bool isStaffOrDemoStage = stageName && (al::isEqualString(stageName, "StaffRollStage") ||
                                            al::isEqualString(stageName, "DemoEndingStage"));

    u64 size = 0x80000LL;
    if (isStaffOrDemoStage || !mWorldResourceHeap) {
        size = 0x6400000LL;
        if (stageName && mIsExistFileResource) {
            al::ByamlIter v19{al::findOrCreateResource("SystemData/MemorySystem", 0LL)
                                  ->getByml("HeapSizeDefine")};

            for (s32 i = 0; i < v19.getSize(); i++) {
                al::ByamlIter v18;
                v19.tryGetIterByIndex(&v18, i);
                
                const char* v17 = nullptr;
                v18.tryGetStringByKey(&v17, "Stage");
                if (al::isEqualString(v17, stageName)) {
                    f32 sceneResourceMB = 0.0;
                    if (!v18.tryGetFloatByKey(&sceneResourceMB, "SceneResourceNx"))
                        v18.tryGetFloatByKey(&sceneResourceMB, "SceneResource");

                    size = sceneResourceMB * 1024.0f * 1024.0f;
                    break;
                }
            }
        }
    }

    sead::Heap::HeapDirection direction = backwards ? sead::Heap::cHeapDirection_Reverse : sead::Heap::cHeapDirection_Forward;

    if (size != 0) {
        sead::Heap* currentHeap = al::getCurrentHeap();
        if (currentHeap && currentHeap->getMaxAllocatableSize(8) < size)
            size = currentHeap->getMaxAllocatableSize(8);
    }

    mSceneResourceHeap =
        sead::FrameHeap::tryCreate(size, "SceneHeapResource", nullptr, 8, direction, true);
    mSceneResourceHeap->mFlag.reset(sead::Heap::Flag::cEnableDebugFillUser);
}

void MemorySystem::destroySceneHeap() {
    mSceneHeap->destroy();
    mSceneHeap = nullptr;
}

void MemorySystem::destroySceneResourceHeap() {
    mSceneResourceHeap->destroy();
    mSceneResourceHeap = nullptr;
}

void MemorySystem::createCourseSelectHeap() {
    {
        u64 size = 0xA00000;
        sead::Heap* currentHeap = al::getCurrentHeap();
        if (currentHeap && currentHeap->getMaxAllocatableSize(8) < size)
            size = currentHeap->getMaxAllocatableSize(8);

        mCourseSelectResourceHeap = sead::ExpHeap::tryCreate(size, "CourseSelectHeapResource", nullptr, 8,
                                              sead::Heap::cHeapDirection_Reverse, true);
        mCourseSelectResourceHeap->mFlag.reset(sead::Heap::Flag::cEnableDebugFillUser);
    }
    {
        u64 size = 0x2800000;
        sead::Heap* currentHeap = al::getCurrentHeap();
        if (currentHeap && currentHeap->getMaxAllocatableSize(8) < size)
            size = currentHeap->getMaxAllocatableSize(8);

        mCourseSelectHeap = sead::ExpHeap::tryCreate(size, "CourseSelectHeapScene", nullptr, 8,
                                              sead::Heap::cHeapDirection_Reverse, false);
        mCourseSelectHeap->mFlag.reset(sead::Heap::Flag::cEnableDebugFillUser);
    }
}

void MemorySystem::destroyCourseSelectHeap() {
    mCourseSelectResourceHeap->destroy();
    mCourseSelectResourceHeap = nullptr;
    mCourseSelectHeap->destroy();
    mCourseSelectHeap = nullptr;
}

void MemorySystem::createWorldResourceHeap() {
    sead::Heap* parent = mSequenceHeap;
    mWorldResourceHeap = sead::ExpHeap::create(0x35700000, "WorldHeapResource", parent, 8, sead::Heap::cHeapDirection_Forward, true);
}

void MemorySystem::destroyWorldResourceHeap() {
    mWorldResourceHeap->destroy();
    mWorldResourceHeap = nullptr;
}

void MemorySystem::freeAllPlayerHeap() {
    mPlayerResourceHeap->freeAll();
}

sead::Heap* MemorySystem::tryFindNamedHeap(const char* heapName) const {
    sead::StrTreeMap<32, sead::Heap*>::Node* result = mHeapList.find(heapName);
    if (!result) return nullptr;
    return result->value();
}

sead::Heap* MemorySystem::findNamedHeap(const char* heapName) const {
    return tryFindNamedHeap(heapName);
}

void MemorySystem::addNamedHeap(sead::Heap* heap, const char* heapName) {
    mHeapList.insert(heapName ?: heap->getName().cstr(), heap);
}

void MemorySystem::removeNamedHeap(const char* heapName) {
    sead::SafeString name = heapName;
    if (mHeapList.find(name))
        mHeapList.erase(name);
}

}  // namespace al
