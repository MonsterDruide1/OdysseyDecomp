#pragma once

#include <container/seadStrTreeMap.h>
#include <heap/seadExpHeap.h>
#include <heap/seadFrameHeap.h>
#include <heap/seadHeapMgr.h>

namespace al {
class AudioResourceDirector;

class MemorySystem {
public:
    MemorySystem(sead::Heap* heap);

    void allocFailedCallbackFunc(const sead::HeapMgr::AllocFailedCallbackArg*);
    void createSequenceHeap();
    void freeAllSequenceHeap();
    void printSequenceHeap();
    bool createSceneHeap(const char* stageName, bool backwards);
    void createSceneResourceHeap(const char* stageName, bool backwards);
    void destroySceneHeap();
    void destroySceneResourceHeap();
    void createCourseSelectHeap();
    void destroyCourseSelectHeap();
    void createWorldResourceHeap();
    void destroyWorldResourceHeap();
    void freeAllPlayerHeap();
    sead::Heap* tryFindNamedHeap(const char* heapName) const;
    sead::Heap* findNamedHeap(const char* heapName) const;
    void addNamedHeap(sead::Heap* heap, const char* heapName);
    void removeNamedHeap(const char* heapName);

    sead::Heap* getStationedHeap() { return mStationedHeap; }

    sead::Heap* getSequenceHeap() { return mSequenceHeap; }

    sead::Heap* getSceneResourceHeap() { return mSceneResourceHeap; }

    sead::Heap* getSceneHeap() { return mSceneHeap; }

    sead::Heap* getPlayerResourceHeap() { return mPlayerResourceHeap; }

    sead::Heap* getCourseSelectResourceHeap() { return mCourseSelectResourceHeap; }

    sead::Heap* getCourseSelectHeap() { return mCourseSelectHeap; }

    sead::Heap* getWorldResourceHeap() { return mWorldResourceHeap; }

    AudioResourceDirector* getAudioResourceDirector() { return mAudioResourceDirector; }

    void setAudioResourceDirector(AudioResourceDirector* audioResourceDirector) {
        mAudioResourceDirector = audioResourceDirector;
    }

private:
    inline u64 getSceneResourceHeapSize(const char* stageName) const;

    sead::ExpHeap* mStationedHeap = nullptr;
    sead::ExpHeap* mSequenceHeap = nullptr;
    sead::FrameHeap* mSceneResourceHeap = nullptr;
    sead::ExpHeap* mSceneHeap = nullptr;
    sead::ExpHeap* mPlayerResourceHeap = nullptr;
    sead::ExpHeap* mCourseSelectResourceHeap = nullptr;
    sead::ExpHeap* mCourseSelectHeap = nullptr;
    sead::ExpHeap* mWorldResourceHeap = nullptr;
    sead::StrTreeMap<32, sead::Heap*> mHeapList;
    AudioResourceDirector* mAudioResourceDirector = nullptr;
    bool mIsExistFileResource = false;
    sead::Delegate1<MemorySystem, const sead::HeapMgr::AllocFailedCallbackArg*> mDelegate;
};

static_assert(sizeof(MemorySystem) == 0x90);
}  // namespace al
