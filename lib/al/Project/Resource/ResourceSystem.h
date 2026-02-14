#pragma once

#include <container/seadRingBuffer.h>
#include <container/seadStrTreeMap.h>
#include <prim/seadSafeString.h>

namespace sead {
class Heap;
}  // namespace sead

namespace al {
class Resource;
class ByamlIter;
class SeadAudioPlayer;

class ResourceSystem {
public:
    struct ResourceCategory {
        ResourceCategory(const sead::SafeString& categoryName, sead::Heap* categoryHeap) {
            name = categoryName;
            heap = categoryHeap;
        }

        sead::FixedSafeString<0x80> name;
        sead::Heap* heap;
        sead::StrTreeMap<156, Resource*> treeMap;
    };

    static_assert(sizeof(ResourceCategory) == 0xc0);

    struct ResourceAudioInfo {
        ResourceAudioInfo(SeadAudioPlayer* playerA, SeadAudioPlayer* playerB, const char* path)
            : audioPlayerA(playerA), audioPlayerB(playerB) {
            filePath.format(path);
        }

        // TODO: proper names for these two
        SeadAudioPlayer* audioPlayerA;
        SeadAudioPlayer* audioPlayerB;
        sead::FixedSafeString<0x40> filePath;
    };

    static_assert(sizeof(ResourceAudioInfo) == 0x68);

    ResourceSystem(const char* name);

    ResourceCategory* addCategory(const sead::SafeString& name, s32 size, sead::Heap* heap);
    Resource* findOrCreateResourceCategory(const sead::SafeString& name,
                                           const sead::SafeString& category, const char* ext);
    sead::RingBuffer<ResourceCategory*>::iterator
    findResourceCategoryIter(const sead::SafeString& name);
    bool isEmptyCategoryResource(const sead::SafeString& name);
    void createCategoryResourceAll(const sead::SafeString& name);
    inline void createResourceCore(Resource* resource);
    Resource* createResource(const sead::SafeString& name, ResourceCategory* category,
                             const char* ext);
    void removeCategory(const sead::SafeString& name);
    Resource* findResource(const sead::SafeString& categoryName);
    Resource* findResourceCore(const sead::SafeString& name,
                               sead::RingBuffer<ResourceCategory*>::iterator* outIter);
    Resource* findOrCreateResource(const sead::SafeString& categoryName, const char* name);
    ResourceCategory* findResourceCategory(const sead::SafeString& name);
    void loadCategoryArchiveAll(const sead::SafeString& name);
    void setCurrentCategory(const char* name);
    const char* findCategoryNameFromTable(const sead::SafeString& name) const;
    bool tryGetTableCategoryIter(ByamlIter* iter, const sead::SafeString& name) const;
    bool tryGetGraphicsInfoIter(ByamlIter* iter, const sead::SafeString& name) const;

    void resetCurrentCategoryName() { mCurrentCategoryName = nullptr; }

    void setAudioPlayer(SeadAudioPlayer* audioPlayerA, SeadAudioPlayer* audioPlayerB) {
        mAudioPlayerA = audioPlayerA;
        mAudioPlayerB = audioPlayerB;
    }

private:
    sead::FixedRingBuffer<ResourceCategory*, 18> mCategories;
    ByamlIter* mResourceCategoryTable = nullptr;
    const char* mCurrentCategoryName = nullptr;
    // TODO: proper names for these two
    SeadAudioPlayer* mAudioPlayerA = nullptr;
    SeadAudioPlayer* mAudioPlayerB = nullptr;
};

static_assert(sizeof(ResourceSystem) == 0xc8);
}  // namespace al
