#pragma once

#include <container/seadRingBuffer.h>
#include <heap/seadHeap.h>
#include <prim/seadSafeString.h>

namespace al {
class Resource;
class ByamlIter;
class SeadAudioPlayer;

class ResourceSystem {
public:
    struct ResourceCategory;

    ResourceSystem(const char*);

    const sead::SafeString& addCategory(const sead::SafeString&, s32, sead::Heap*);
    Resource* findOrCreateResourceCategory(const sead::SafeString&, const sead::SafeString&,
                                           const char*);
    s64 findResourceCategoryIter(const sead::SafeString&);
    bool isEmptyCategoryResource(const sead::SafeString&);
    void createCategoryResourceAll(const sead::SafeString&);
    Resource* createResource(const sead::SafeString&, ResourceCategory*, const char*);
    void removeCategory(const sead::SafeString&);
    Resource* findResource(const sead::SafeString&);
    Resource* findResourceCore(const sead::SafeString&,
                               sead::RingBuffer<ResourceCategory*>::iterator*);
    Resource* findOrCreateResource(const sead::SafeString&, const char*);
    ResourceCategory* findResourceCategory(const sead::SafeString&);
    void loadCategoryArchiveAll(const sead::SafeString&);
    void setCurrentCategory(const char*);
    const char* findCategoryNameFromTable(const sead::SafeString&) const;
    bool tryGetTableCategoryIter(ByamlIter*, const sead::SafeString&) const;
    bool tryGetGraphicsInfoIter(ByamlIter*, const sead::SafeString&) const;

    void resetCurrentCategoryName() { mCurrentCategoryName = nullptr; }

    void setAudioPlayer(SeadAudioPlayer* audioPlayerA, SeadAudioPlayer* audioPlayerB) {
        mAudioPlayerA = audioPlayerA;
        mAudioPlayerB = audioPlayerB;
    }

private:
    char filler[0xb0];
    const char* mCurrentCategoryName;
    // TODO: proper names for these two
    SeadAudioPlayer* mAudioPlayerA;
    SeadAudioPlayer* mAudioPlayerB;
};

static_assert(sizeof(ResourceSystem) == 0xc8);
}  // namespace al
