#include "Library/Memory/HeapUtil.h"

#include <heap/seadHeapMgr.h>

#include "Library/File/FileUtil.h"
#include "Library/Resource/ResourceHolder.h"
#include "Library/System/SystemKit.h"
#include "Project/Memory/MemorySystem.h"

#include "System/ProjectInterface.h"

namespace al {
sead::Heap* getStationedHeap() {
    return alProjectInterface::getSystemKit()->getMemorySystem()->getStationedHeap();
}

sead::Heap* getSequenceHeap() {
    return alProjectInterface::getSystemKit()->getMemorySystem()->getSequenceHeap();
}

sead::Heap* getSceneResourceHeap() {
    return alProjectInterface::getSystemKit()->getMemorySystem()->getSceneResourceHeap();
}

sead::Heap* getSceneHeap() {
    return alProjectInterface::getSystemKit()->getMemorySystem()->getSceneHeap();
}

sead::Heap* getCourseSelectResourceHeap() {
    return alProjectInterface::getSystemKit()->getMemorySystem()->getCourseSelectResourceHeap();
}

sead::Heap* getCourseSelectHeap() {
    return alProjectInterface::getSystemKit()->getMemorySystem()->getCourseSelectHeap();
}

sead::Heap* getWorldResourceHeap() {
    return alProjectInterface::getSystemKit()->getMemorySystem()->getWorldResourceHeap();
}

sead::Heap* tryFindNamedHeap(const char* heapName) {
    return alProjectInterface::getSystemKit()->getMemorySystem()->tryFindNamedHeap(heapName);
}

sead::Heap* findNamedHeap(const char* heapName) {
    return alProjectInterface::getSystemKit()->getMemorySystem()->findNamedHeap(heapName);
}

void addNamedHeap(sead::Heap* heap, const char* heapName) {
    alProjectInterface::getSystemKit()->getMemorySystem()->addNamedHeap(heap, heapName);
}

void removeNamedHeap(const char* heapName) {
    alProjectInterface::getSystemKit()->getMemorySystem()->removeNamedHeap(heapName);
}

void createSequenceHeap() {
    alProjectInterface::getSystemKit()->getMemorySystem()->createSequenceHeap();

    addResourceCategory("シーケンス", 0x18, getSequenceHeap());
    setCurrentCategoryName("シーケンス");
    clearFileLoaderEntry();
}

void freeAllSequenceHeap() {
    removeResourceCategory("シーケンス");
    alProjectInterface::getSystemKit()->getMemorySystem()->freeAllSequenceHeap();

    addResourceCategory("シーケンス", 0x18, getSequenceHeap());
    setCurrentCategoryName("シーケンス");
    clearFileLoaderEntry();
}

bool printAllSequenceHeap() {
    return alProjectInterface::getSystemKit()->getMemorySystem()->printSequenceHeap();
}

void createSceneHeap(const char* stageName, bool backwards) {
    sead::ScopedCurrentHeapSetter heapSetter = sead::ScopedCurrentHeapSetter(getSequenceHeap());

    SystemKit* systemKit = alProjectInterface::getSystemKit();
    bool isSceneHeapCreated = systemKit->getMemorySystem()->createSceneHeap(stageName, backwards);
    if (isSceneHeapCreated) {
        addResourceCategory("シーン", 0x200, getSceneResourceHeap());
        addResourceCategory("シーン", 0x200, getSceneResourceHeap());
        setCurrentCategoryName("シーン");
        clearFileLoaderEntry();
    }
}

void createSceneResourceHeap(const char* stageName) {
    sead::ScopedCurrentHeapSetter heapSetter = sead::ScopedCurrentHeapSetter(getSequenceHeap());

    alProjectInterface::getSystemKit()->getMemorySystem()->createSceneResourceHeap(stageName,
                                                                                   false);

    addResourceCategory("シーン", 0x200, getSceneResourceHeap());
    addResourceCategory("シーン", 0x200, getSceneResourceHeap());
    setCurrentCategoryName("シーン");
    clearFileLoaderEntry();
}

bool isCreatedSceneResourceHeap() {
    return getSceneResourceHeap() != nullptr;
}

void destroySceneHeap(bool removeCategory) {
    if (removeCategory) {
        removeResourceCategory("シーン");
        removeResourceCategory("シーン[デバッグ]");
        alProjectInterface::getSystemKit()->getMemorySystem()->destroySceneHeap();
        alProjectInterface::getSystemKit()->getMemorySystem()->destroySceneResourceHeap();

        return;
    }

    alProjectInterface::getSystemKit()->getMemorySystem()->destroySceneHeap();
}

void createCourseSelectHeap() {
    sead::ScopedCurrentHeapSetter heapSetter = sead::ScopedCurrentHeapSetter(getSequenceHeap());

    alProjectInterface::getSystemKit()->getMemorySystem()->createCourseSelectHeap();

    addResourceCategory("コースセレクト", 0x40, getCourseSelectResourceHeap());
    setCurrentCategoryName("コースセレクト");
    clearFileLoaderEntry();
}

void destroyCourseSelectHeap() {
    removeResourceCategory("コースセレクト");
    alProjectInterface::getSystemKit()->getMemorySystem()->destroyCourseSelectHeap();
}

void createWorldResourceHeap(bool useCategory) {
    sead::ScopedCurrentHeapSetter heapSetter = sead::ScopedCurrentHeapSetter(getSequenceHeap());

    alProjectInterface::getSystemKit()->getMemorySystem()->createWorldResourceHeap();

    if (useCategory) {
        addResourceCategory("ワールド常駐", 0x400, getWorldResourceHeap());
        setCurrentCategoryName("ワールド常駐");
    }
    clearFileLoaderEntry();
}

void destroyWorldResourceHeap(bool removeCategory) {
    if (removeCategory) {
        removeResourceCategory("ワールド常駐");
    }

    clearFileLoaderEntry();
    alProjectInterface::getSystemKit()->getMemorySystem()->destroyWorldResourceHeap();
}

void loadPlayerResource(const char* categoryName) {
    addResourceCategory(
        categoryName, 0x14,
        alProjectInterface::getSystemKit()->getMemorySystem()->getPlayerResourceHeap());
    createCategoryResourceAll(categoryName);
}

void freePlayerResource(const char* categoryName) {
    removeResourceCategory(categoryName);
    alProjectInterface::getSystemKit()->getMemorySystem()->freeAllPlayerHeap();
}

void setAudioResourceDirectorToMemorySystem(AudioResourceDirector* audioResourceDirector) {
    alProjectInterface::getSystemKit()->getMemorySystem()->setAudioResourceDirector(
        audioResourceDirector);
}
}  // namespace al
