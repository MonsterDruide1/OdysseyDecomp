#include "Library/Resource/ResourceFunction.h"

#include "Library/Base/StringUtil.h"
#include "Library/File/FileUtil.h"
#include "Library/LiveActor/ActorResourceFunction.h"
#include "Library/Memory/HeapUtil.h"
#include "Library/Resource/ActorResourceHolder.h"
#include "Library/Resource/Resource.h"
#include "Library/System/SystemKit.h"
#include "Library/Yaml/ByamlIter.h"
#include "Project/Resource/ResourceSystem.h"

#include "System/ProjectInterface.h"

namespace al {

void addResourceCategory(const sead::SafeString& resourceName, s32 category, sead::Heap* heap) {
    alProjectInterface::getSystemKit()->getResourceSystem()->addCategory(resourceName, category,
                                                                         heap);
}

bool isEmptyCategoryResource(const sead::SafeString& resourceName) {
    return alProjectInterface::getSystemKit()->getResourceSystem()->isEmptyCategoryResource(
        resourceName);
}

void createCategoryResourceAll(const sead::SafeString& resourceName) {
    alProjectInterface::getSystemKit()->getResourceSystem()->createCategoryResourceAll(
        resourceName);
}

void removeResourceCategory(const sead::SafeString& resourceName) {
    alProjectInterface::getSystemKit()->getResourceSystem()->removeCategory(resourceName);
}

const char* getResourceName(const Resource* resource) {
    return resource->getArchiveName();
}

const char* getResourcePath(const Resource* resource) {
    return resource->getPath();
}

bool isExistResGraphicsFile(const Resource* resource) {
    return resource->getResFile() != nullptr;
}

Resource* findResource(const sead::SafeString& resourceName) {
    return alProjectInterface::getSystemKit()->getResourceSystem()->findResource(resourceName);
}

Resource* findOrCreateResource(const sead::SafeString& resourceName, const char* ext) {
    return alProjectInterface::getSystemKit()->getResourceSystem()->findOrCreateResource(
        resourceName, ext);
}

Resource* findOrCreateResourceCategory(const sead::SafeString& resourceName,
                                       const sead::SafeString& category, const char* ext) {
    return alProjectInterface::getSystemKit()->getResourceSystem()->findOrCreateResourceCategory(
        resourceName, category, ext);
}

Resource* findOrCreateResourceEventData(const char* eventDataName, const char* resourceName) {
    StringTmp<128> eventName = {"EventData/%s", eventDataName};
    return findOrCreateResource(eventName, resourceName);
}

Resource* findOrCreateResourceSystemData(const char* systemDataName, const char* resourceName) {
    StringTmp<128> systemName = {"SystemData/%s", systemDataName};
    return findOrCreateResource(systemName, resourceName);
}

ActorResource* findOrCreateActorResource(ActorResourceHolder* resourceHolder,
                                         const char* actorResourceName, const char* suffix) {
    Resource* resource = findOrCreateResource(actorResourceName, nullptr);
    const char* animArc = nullptr;
    bool isMergeAnim = false;
    ByamlIter fileIter;
    if (tryGetActorInitFileIter(&fileIter, resource, "InitModel", suffix)) {
        fileIter.tryGetStringByKey(&animArc, "AnimArc");
        fileIter.tryGetBoolByKey(&isMergeAnim, "IsMergeAnim");
    }

    StringTmp<256> actorResourceFile("");
    getActorResourceFile(&actorResourceFile, actorResourceName, animArc, suffix);
    ActorResource* actorResource = resourceHolder->tryFindActorResource(actorResourceFile);
    if (actorResource == nullptr) {
        Resource* objectResource = nullptr;
        if (animArc)
            objectResource =
                findOrCreateResource(StringTmp<256>("ObjectData/%s", animArc).cstr(), nullptr);
        actorResource =
            resourceHolder->createActorResource(actorResourceFile, resource, objectResource);
        actorResource->initResourceData(suffix, isMergeAnim);
    }

    return actorResource;
}

void getActorResourceFile(StringTmp<256>* actorResourceFile, const char* actorResourceName,
                          const char* animArc, const char* suffix) {
    actorResourceFile->copy(actorResourceName);

    if (animArc) {
        actorResourceFile->append("::");
        actorResourceFile->append(animArc);
    }

    if (suffix) {
        actorResourceFile->append("::");
        actorResourceFile->append(suffix);
    }
}

ActorResource* findOrCreateActorResourceWithAnimResource(ActorResourceHolder* resourceHolder,
                                                         const char* actorResourceName,
                                                         const char* animArc, const char* suffix,
                                                         bool isMergeAnim) {
    StringTmp<256> actorResourceFile("");
    getActorResourceFile(&actorResourceFile, actorResourceName, animArc, suffix);
    ActorResource* actorResource = resourceHolder->tryFindActorResource(actorResourceFile);
    if (actorResource == nullptr) {
        Resource* resource = findOrCreateResource(actorResourceName, nullptr);
        Resource* objectResource = nullptr;
        if (animArc)
            objectResource = findOrCreateResource(animArc, nullptr);
        actorResource =
            resourceHolder->createActorResource(actorResourceFile, resource, objectResource);
        actorResource->initResourceData(suffix, isMergeAnim);
    }
    return actorResource;
}

bool isExistResourceYaml(const Resource* resource, const char* resourceName, const char* suffix) {
    if (suffix) {
        sead::FixedSafeString<128> fileName;
        createFileNameBySuffix(&fileName, resourceName, suffix);
        return resource->isExistFile(StringTmp<64>("%s.byml", fileName.cstr()));
    }

    return resource->isExistFile(StringTmp<64>("%s.byml", resourceName));
}

bool isExistResourceYaml(const ActorResource* actorResource, const char* resourceName,
                         const char* suffix) {
    Resource* resource =
        actorResource->hasAnimData() ? actorResource->getAnimRes() : actorResource->getModelRes();
    return isExistResourceYaml(resource, resourceName, suffix);
}

const u8* findResourceYaml(const Resource* resource, const char* resourceName, const char* suffix) {
    if (suffix) {
        sead::FixedSafeString<128> fileName;
        createFileNameBySuffix(&fileName, resourceName, suffix);
        resourceName = fileName.cstr();
    }

    return resource->getByml(resourceName);
}

const u8* findResourceYaml(const ActorResource* actorResource, const char* resourceName,
                           const char* suffix) {
    Resource* resource =
        actorResource->hasAnimData() ? actorResource->getAnimRes() : actorResource->getModelRes();
    return findResourceYaml(resource, resourceName, suffix);
}

const void* tryFindStageParameterFileDesign(const sead::SafeString& stageName,
                                            const sead::SafeString& fileName, s32) {
    StringTmp<128> resourceName{"StageData/%sDesign", stageName.cstr()};
    if (!isExistArchive(resourceName))
        return nullptr;

    Resource* resource = findOrCreateResource(resourceName, nullptr);
    if (!resource->isExistFile(fileName))
        return nullptr;

    return resource->getOtherFile(fileName);
}

void loadCategoryArchiveAll(const sead::SafeString& categoryName) {
    alProjectInterface::getSystemKit()->getResourceSystem()->loadCategoryArchiveAll(categoryName);
}

void setCurrentCategoryName(const char* categoryName) {
    alProjectInterface::getSystemKit()->getResourceSystem()->setCurrentCategory(categoryName);
}

void resetCurrentCategoryName() {
    alProjectInterface::getSystemKit()->getResourceSystem()->resetCurrentCategoryName();
}

void setCurrentCategoryNameDefault() {
    if (getWorldResourceHeap())
        setCurrentCategoryName("ワールド常駐");
    else
        setCurrentCategoryName("シーン");
}

const u8* getBymlFromObjectResource(const sead::SafeString& objectName,
                                    const sead::SafeString& categoryName) {
    StringTmp<256> resourceName("ObjectData/%s", objectName.cstr());
    return findOrCreateResource(resourceName, nullptr)->getByml(categoryName);
}

const u8* tryGetBymlFromObjectResource(const sead::SafeString& objectName,
                                       const sead::SafeString& categoryName) {
    StringTmp<256> resourceName("ObjectData/%s", objectName.cstr());
    return findOrCreateResource(resourceName, nullptr)->tryGetByml(categoryName);
}

const u8* getBymlFromLayoutResource(const sead::SafeString& layoutName,
                                    const sead::SafeString& categoryName) {
    StringTmp<256> resourceName("LayoutData/%s", layoutName.cstr());
    return findOrCreateResource(resourceName, nullptr)->getByml(categoryName);
}

const u8* tryGetBymlFromLayoutResource(const sead::SafeString& layoutName,
                                       const sead::SafeString& categoryName) {
    StringTmp<256> resourceName("LayoutData/%s", layoutName.cstr());
    return findOrCreateResource(resourceName, nullptr)->tryGetByml(categoryName);
}

const u8* tryGetBymlFromArcName(const sead::SafeString& archiveName,
                                const sead::SafeString& categoryName) {
    return findOrCreateResource(archiveName, nullptr)->tryGetByml(categoryName);
}

const u8* getBymlFromArcName(const sead::SafeString& archiveName,
                             const sead::SafeString& categoryName) {
    return tryGetBymlFromArcName(archiveName, categoryName);
}

const u8* getByml(const Resource* resource, const sead::SafeString& bymlName) {
    return resource->tryGetByml(bymlName);
}

const u8* tryGetByml(const Resource* resource, const sead::SafeString& bymlName) {
    return resource->tryGetByml(bymlName);
}

void setAudioPlayerToResourceSystem(SeadAudioPlayer* audioPlayerA, SeadAudioPlayer* audioPlayerB) {
    alProjectInterface::getSystemKit()->getResourceSystem()->setAudioPlayer(audioPlayerA,
                                                                            audioPlayerB);
}

}  // namespace al
