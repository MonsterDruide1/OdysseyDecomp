#include "Project/Resource/ResourceSystem.h"

#include <container/seadStrTreeMap.h>
#include <heap/seadHeapMgr.h>

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

ALWAYS_INLINE void ResourceSystem::createResourceCore(ResourceSystem* self, Resource* resource) {
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
            resFile = self->findOrCreateResource(filePath, nullptr)->getResFile();
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

    createResourceCore(this, resource);

    return resource->getFileArchive() ? resource : nullptr;
}

void cleanupResGraphicsFile(const sead::SafeString& key, Resource* resource) {
    resource->cleanupResGraphicsFile();
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

            sead::FixedSafeString<0x80> localizedName;
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
