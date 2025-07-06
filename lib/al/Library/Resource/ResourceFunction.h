#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>

namespace sead {
class Heap;
}

namespace al {
class ActorResource;
class ActorResourceHolder;
class Resource;
class SeadAudioPlayer;

void addResourceCategory(const sead::SafeString& resourceName, s32 category, sead::Heap* heap);
bool isEmptyCategoryResource(const sead::SafeString& resourceName);
void createCategoryResourceAll(const sead::SafeString& resourceName);
void removeResourceCategory(const sead::SafeString& resourceName);
const char* getResourceName(const Resource* resource);
const char* getResourcePath(const Resource* resource);
bool isExistResGraphicsFile(const Resource* resource);
Resource* findResource(const sead::SafeString&);
Resource* findOrCreateResource(const sead::SafeString& path, const char* ext);
Resource* findOrCreateResourceCategory(const sead::SafeString& path,
                                       const sead::SafeString& category, const char* ext);
Resource* findOrCreateResourceEventData(const char* eventDataName, const char* resourceName);
Resource* findOrCreateResourceSystemData(const char* systemDataName, const char* resourceName);
ActorResource* findOrCreateActorResource(ActorResourceHolder* resourceHolder,
                                         const char* actorResourceName, const char* suffix);
ActorResource* findOrCreateActorResourceWithAnimResource(ActorResourceHolder* resourceHolder,
                                                         const char* actorResourceName,
                                                         const char* animArc, const char* suffix,
                                                         bool isMergeAnim);
bool isExistResourceYaml(const Resource* resource, const char* resourceName, const char* suffix);
bool isExistResourceYaml(const ActorResource* actorResource, const char* resourceName,
                         const char* suffix);
const u8* findResourceYaml(const Resource* resource, const char* resourceName, const char* suffix);
const u8* findResourceYaml(const ActorResource* actorResource, const char* resourceName,
                           const char* suffix);
const void* tryFindStageParameterFileDesign(const sead::SafeString& stageName,
                                            const sead::SafeString& fileName, s32);
void loadCategoryArchiveAll(const sead::SafeString& categoryName);
void setCurrentCategoryName(const char* categoryName);
void resetCurrentCategoryName();
void setCurrentCategoryNameDefault();
const u8* getBymlFromObjectResource(const sead::SafeString& objectName,
                                    const sead::SafeString& categoryName);
const u8* tryGetBymlFromObjectResource(const sead::SafeString& objectName,
                                       const sead::SafeString& categoryName);
const u8* getBymlFromLayoutResource(const sead::SafeString& layoutName,
                                    const sead::SafeString& categoryName);
const u8* tryGetBymlFromLayoutResource(const sead::SafeString& layoutName,
                                       const sead::SafeString& categoryName);
const u8* tryGetBymlFromArcName(const sead::SafeString& archiveName,
                                const sead::SafeString& categoryName);
const u8* getBymlFromArcName(const sead::SafeString& archiveName,
                             const sead::SafeString& categoryName);
const u8* getByml(const Resource* resource, const sead::SafeString& bymlName);
const u8* tryGetByml(const Resource* resource, const sead::SafeString& bymlName);
void setAudioPlayerToResourceSystem(SeadAudioPlayer* audioPlayerA, SeadAudioPlayer* audioPlayerB);

}  // namespace al
