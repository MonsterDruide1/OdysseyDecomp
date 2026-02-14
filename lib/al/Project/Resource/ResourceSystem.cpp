#include "Project/Resource/ResourceSystem.h"

#include <container/seadStrTreeMap.h>
#include <heap/seadHeapMgr.h>

#include "Library/Audio/System/AudioKeeperFunction.h"
#include "Library/Base/Macros.h"
#include "Library/Base/StringUtil.h"
#include "Library/File/FileUtil.h"
#include "Library/LiveActor/ActorInitResourceData.h"
#include "Library/LiveActor/ActorResourceFunction.h"
#include "Library/Resource/Resource.h"
#include "Library/Yaml/ByamlIter.h"

namespace al {

ResourceSystem::ResourceSystem(const char* name) {
    addCategory("リソースシステム", 1, sead::HeapMgr::instance()->getCurrentHeap());

    const char* resourceName = "SystemData/ResourceSystem";
    if (name)
        resourceName = name;

    if (isExistArchive(resourceName)) {
        Resource* resource =
            findOrCreateResourceCategory(resourceName, "リソースシステム", nullptr);
        if (resource)
            mResourceCategoryTable = new ByamlIter(resource->getByml("ResourceCategoryTable"));
    }
}

ResourceSystem::ResourceCategory* ResourceSystem::addCategory(const sead::SafeString& name,
                                                              s32 size, sead::Heap* heap) {
    sead::RingBuffer<ResourceCategory*>::iterator iter = findResourceCategoryIter(name);
    if (iter != mCategories.end())
        return *iter;

    sead::ScopedCurrentHeapSetter heapSetter(heap);
    ResourceCategory* category = new ResourceCategory(name, heap);
    category->treeMap.allocBuffer(size, nullptr);

    mCategories.pushBack(category);
    return category;
}

Resource* ResourceSystem::findOrCreateResourceCategory(const sead::SafeString& name,
                                                       const sead::SafeString& category,
                                                       const char* ext) {
    Resource* resource = findResource(name);
    if (resource)
        return resource;

    sead::RingBuffer<ResourceCategory*>::iterator iter = findResourceCategoryIter(category);
    if (iter == mCategories.end())
        return nullptr;

    return createResource(name, *iter, ext);
}

sead::RingBuffer<ResourceSystem::ResourceCategory*>::iterator
ResourceSystem::findResourceCategoryIter(const sead::SafeString& name) {
    for (auto iter = mCategories.begin(); iter != mCategories.end(); ++iter)
        if (isEqualString((*iter)->name.cstr(), name.cstr()))
            return iter;
    return mCategories.end();
}

bool ResourceSystem::isEmptyCategoryResource(const sead::SafeString& name) {
    sead::RingBuffer<ResourceCategory*>::iterator iter = findResourceCategoryIter(name);
    if (iter == mCategories.end())
        return true;

    return (*iter)->treeMap.isEmpty();
}

void ResourceSystem::createCategoryResourceAll(const sead::SafeString& name) {
    if (!mResourceCategoryTable)
        return;

    sead::RingBuffer<ResourceCategory*>::iterator iter = findResourceCategoryIter(name);
    if (iter == mCategories.end())
        return;

    for (s32 i = 0; i < mResourceCategoryTable->getSize(); i++) {
        ByamlIter categoryIter;
        if (!mResourceCategoryTable->tryGetIterByIndex(&categoryIter, i))
            continue;

        const char* iterName = nullptr;
        if (!categoryIter.tryGetStringByKey(&iterName, "Category"))
            continue;

        if (!isEqualString(iterName, name.cstr()))
            continue;

        ByamlIter arcsIter;
        if (!categoryIter.tryGetIterByKey(&arcsIter, "Arcs"))
            continue;

        bool isLocalized = false;
        categoryIter.tryGetBoolByKey(&isLocalized, "Localized");

        for (s32 j = 0; j < arcsIter.getSize(); j++) {
            ByamlIter subArcIter;
            arcsIter.tryGetIterByIndex(&subArcIter, j);

            const char* arcName = nullptr;
            const char* arcExt = nullptr;

            if (!subArcIter.tryGetStringByKey(&arcName, "Name"))
                continue;

            subArcIter.tryGetStringByKey(&arcExt, "Ext");

            StringTmp<128> localizedName;
            if (isLocalized) {
                makeLocalizedArchivePath(&localizedName, arcName);
                arcName = localizedName.cstr();
            }

            createResource(arcName, *iter, arcExt);
        }
    }
}

ALWAYS_INLINE void ResourceSystem::createResourceCore(Resource* resource) {
    StringTmp<256> fileName = StringTmp<256>("%s.bfres", resource->getArchiveName());

    if (!resource->isExistFile(fileName))
        return;

    ByamlIter iter;
    nn::g3d::ResFile* resFile = nullptr;
    if (tryGetActorInitFileIter(&iter, resource, "InitModel", nullptr)) {
        const char* textureArc = nullptr;
        iter.tryGetStringByKey(&textureArc, "TextureArc");

        if (textureArc) {
            StringTmp<256> filePath = StringTmp<256>("ObjectData/%s", textureArc);
            resFile = findOrCreateResource(filePath, nullptr)->getResFile();
        }
    }
    resource->tryCreateResGraphicsFile(fileName, resFile);
    resource->setActorInitResourceData(new ActorInitResourceData(resource));
}

Resource* ResourceSystem::createResource(const sead::SafeString& name, ResourceCategory* category,
                                         const char* ext) {
    sead::ScopedCurrentHeapSetter setter(category->heap);

    Resource* resource = nullptr;
    resource = ext ? new Resource(name, loadArchiveWithExt(name, ext)) : new Resource(name);
    category->treeMap.insert(name, resource);

    createResourceCore(resource);

    return resource->getFileArchive() ? resource : nullptr;
}

void cleanupResGraphicsFile(sead::SafeString& key, Resource* resource) {
    resource->cleanupResGraphicsFile();
}

class ResourceAudio {
public:
    ResourceAudio(ResourceSystem::ResourceAudioInfo* info) : mAudioPlayerInfo(info) {}

    void disableSoundMemoryPoolHandler(sead::TreeMapImpl<sead::SafeString>::Node* node) {
        ResourceSystem::ResourceAudioInfo* info = mAudioPlayerInfo;

        if (node->key().comparen(info->filePath, info->filePath.calcLength()) != 0)
            return;

        SeadAudioPlayer* audioPlayer =
            alAudioSystemFunction::tryFindAudioPlayerRegistedSoundMemoryPoolHandler(
                node->key().cstr(), info->audioPlayerA, info->audioPlayerB);

        if (audioPlayer) {
            while (!alAudioSystemFunction::tryDisableSoundMemoryPoolHandlerByFilePath(
                node->key().cstr(), audioPlayer)) {
            }
        }
    }

private:
    ResourceSystem::ResourceAudioInfo* mAudioPlayerInfo;
};

// NON_MATCHING: https://decomp.me/scratch/R5MuA
void ResourceSystem::removeCategory(const sead::SafeString& name) {
    ResourceAudioInfo audioPlayerInfo(mAudioPlayerA, mAudioPlayerB, "SoundData/");

    sead::RingBuffer<ResourceCategory*>::iterator iter = findResourceCategoryIter(name);
    if (iter == mCategories.end())
        return;

    (*iter)->treeMap.forEach(&cleanupResGraphicsFile);

    // TODO: Find the correct implementation for this part
    ResourceCategory* category = *iter;
    {
        ResourceAudio ctx(&audioPlayerInfo);

        using MapImpl = sead::TreeMapImpl<sead::SafeString>;
        sead::Delegate1<ResourceAudio, MapImpl::Node*> delegate(
            &ctx, &ResourceAudio::disableSoundMemoryPoolHandler);

        category->treeMap.MapImpl::forEach(delegate);
    }

    (*iter)->treeMap.clear();
    mCategories.remove(iter.getIndex());
}

Resource* ResourceSystem::findResource(const sead::SafeString& categoryName) {
    return findResourceCore(categoryName, nullptr);
}

Resource* ResourceSystem::findResourceCore(const sead::SafeString& name,
                                           sead::RingBuffer<ResourceCategory*>::iterator* outIter) {
    for (auto iter = mCategories.begin(); iter != mCategories.end(); ++iter) {
        auto* node = (*iter)->treeMap.find(name);
        if (!node)
            continue;
        if (outIter)
            *outIter = iter;
        return node->value();
    }

    return nullptr;
}

Resource* ResourceSystem::findOrCreateResource(const sead::SafeString& categoryName,
                                               const char* name) {
    Resource* resource = findResource(categoryName);
    if (resource)
        return resource;

    return createResource(categoryName, findResourceCategory(categoryName), name);
}

ResourceSystem::ResourceCategory*
ResourceSystem::findResourceCategory(const sead::SafeString& name) {
    sead::RingBuffer<ResourceCategory*>::iterator iter(nullptr, 0);
    bool found = false;

    const char* categoryName = findCategoryNameFromTable(name);
    if (categoryName) {
        iter = findResourceCategoryIter(categoryName);
        if (iter != mCategories.end())
            found = true;
    }

    if (!found) {
        const char* fallbackCategoryName = "シーン";
        iter = findResourceCategoryIter(mCurrentCategoryName ?: fallbackCategoryName);
    }

    return *iter;
}

void ResourceSystem::loadCategoryArchiveAll(const sead::SafeString& name) {
    sead::RingBuffer<ResourceCategory*>::iterator iter = findResourceCategoryIter(name);
    if (iter == mCategories.end())
        return;

    for (s32 i = 0; i < mResourceCategoryTable->getSize(); i++) {
        ByamlIter categoryIter;
        if (!mResourceCategoryTable->tryGetIterByIndex(&categoryIter, i))
            continue;

        const char* category = nullptr;
        if (!categoryIter.tryGetStringByKey(&category, "Category"))
            continue;

        if (!isEqualString(category, name.cstr()))
            continue;

        ByamlIter arcsIter;
        if (!categoryIter.tryGetIterByKey(&arcsIter, "Arcs"))
            continue;

        bool isLocalized = false;
        categoryIter.tryGetBoolByKey(&isLocalized, "Localized");

        for (s32 j = 0; j < arcsIter.getSize(); j++) {
            ByamlIter subArcIter;
            arcsIter.tryGetIterByIndex(&subArcIter, j);

            const char* arcName = nullptr;
            const char* arcExt = nullptr;

            if (!subArcIter.tryGetStringByKey(&arcName, "Name"))
                continue;

            subArcIter.tryGetStringByKey(&arcExt, "Ext");

            StringTmp<128> localizedName;
            if (isLocalized) {
                if (isEqualString(arcName, "TrialRating"))
                    makeLocalizedArchivePathByCountryCode(&localizedName, arcName);
                else
                    makeLocalizedArchivePath(&localizedName, arcName);
                arcName = localizedName.cstr();
            }

            if (arcExt)
                loadArchiveWithExt(arcName, arcExt);
            else
                loadArchive(arcName);
        }
    }
}

void ResourceSystem::setCurrentCategory(const char* name) {
    mCurrentCategoryName = name;
}

const char* ResourceSystem::findCategoryNameFromTable(const sead::SafeString& name) const {
    if (!mResourceCategoryTable)
        return nullptr;

    for (s32 i = 0; i < mResourceCategoryTable->getSize(); i++) {
        ByamlIter categoryIter;
        if (!mResourceCategoryTable->tryGetIterByIndex(&categoryIter, i))
            continue;

        const char* categoryName = nullptr;
        if (!categoryIter.tryGetStringByKey(&categoryName, "Category"))
            continue;

        ByamlIter arcsIter;
        categoryIter.tryGetIterByKey(&arcsIter, "Arcs");

        bool isLocalized = false;
        categoryIter.tryGetBoolByKey(&isLocalized, "Localized");

        for (s32 j = 0; j < arcsIter.getSize(); j++) {
            ByamlIter subArcIter;
            arcsIter.tryGetIterByIndex(&subArcIter, j);

            const char* arcName = nullptr;
            if (!subArcIter.tryGetStringByKey(&arcName, "Name"))
                continue;

            StringTmp<128> localizedName;
            if (isLocalized) {
                if (isEqualString(name, "TrialRating"))
                    makeLocalizedArchivePathByCountryCode(&localizedName, name);
                else
                    makeLocalizedArchivePath(&localizedName, arcName);
                arcName = localizedName.cstr();
            }

            if (isEqualString(arcName, name.cstr()))
                return categoryName;
        }
    }

    return nullptr;
}

bool ResourceSystem::tryGetTableCategoryIter(ByamlIter* iter, const sead::SafeString& name) const {
    if (!mResourceCategoryTable)
        return false;

    for (s32 i = 0; i < mResourceCategoryTable->getSize(); i++) {
        ByamlIter categoryIter;
        if (!mResourceCategoryTable->tryGetIterByIndex(&categoryIter, i))
            continue;

        const char* categoryName = nullptr;
        if (!categoryIter.tryGetStringByKey(&categoryName, "Category"))
            continue;

        if (isEqualString(categoryName, name.cstr()) &&
            mResourceCategoryTable->tryGetIterByIndex(iter, i)) {
            return true;
        }
    }

    return false;
}

bool ResourceSystem::tryGetGraphicsInfoIter(ByamlIter* iter, const sead::SafeString& name) const {
    if (!mResourceCategoryTable)
        return false;

    for (s32 i = 0; i < mResourceCategoryTable->getSize(); i++) {
        ByamlIter categoryIter;
        if (!mResourceCategoryTable->tryGetIterByIndex(&categoryIter, i))
            continue;

        ByamlIter graphicsInfoIter;
        if (!categoryIter.tryGetIterByKey(&graphicsInfoIter, "GraphicsInfo"))
            continue;

        for (s32 j = 0; j < graphicsInfoIter.getSize(); j++) {
            graphicsInfoIter.tryGetIterByIndex(iter, j);

            const char* arcName = nullptr;
            iter->tryGetStringByKey(&arcName, "Arc");
            if (isEqualString(name, arcName))
                return true;
        }
    }

    return false;
}

}  // namespace al
