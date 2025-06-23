#pragma once

#include <prim/seadSafeString.h>

namespace al {
class ByamlIter;
class LiveActor;
class ActorResource;
struct ActorInitInfo;
class ParameterIo;
class Resource;

bool isExistModelResource(const LiveActor* actor);
bool isExistAnimResource(const LiveActor* actor);
Resource* tryGetAnimResource(const LiveActor* actor);
bool isExistModelResourceYaml(const LiveActor* actor, const char* name, const char* suffix);
Resource* getModelResource(const LiveActor* actor);
bool isExistAnimResourceYaml(const LiveActor* actor, const char* name, const char* suffix);
Resource* getAnimResource(const LiveActor* actor);
bool isExistModelOrAnimResourceYaml(const LiveActor* actor, const char* name, const char* suffix);
const u8* getModelResourceYaml(const LiveActor* actor, const char* name, const char* suffix);
const u8* getAnimResourceYaml(const LiveActor* actor, const char* name, const char* suffix);
const u8* getModelOrAnimResourceYaml(const LiveActor* actor, const char* name, const char* suffix);
const u8* getMapPartsResourceYaml(const ActorInitInfo& initInfo, const char* name);
const u8* tryGetMapPartsResourceYaml(const ActorInitInfo& initInfo, const char* name);
bool tryMakeInitFileName(sead::BufferedSafeString* fileName, const Resource* resource,
                         const char* suffixIterKey, const char* suffixIterSuffix,
                         const char* suffixIterName);
bool tryGetSuffixIter(ByamlIter* iter, const Resource* resource, const char* name,
                      const char* suffix);
bool tryGetInitFileIterAndName(ByamlIter* iter, sead::BufferedSafeString* fileName,
                               const Resource* resource, const char* suffixIterKey,
                               const char* suffixIterSuffix, const char* suffixIterName);
bool tryGetActorInitFileIterAndName(ByamlIter* iter, sead::BufferedSafeString* fileName,
                                    const Resource* resource, const char* suffixIterKey,
                                    const char* suffixIterSuffix);
bool tryGetActorInitFileIter(ByamlIter* iter, const Resource* resource, const char* suffixIterKey,
                             const char* suffixIterSuffix);
bool tryGetActorInitFileIterAndName(ByamlIter* iter, sead::BufferedSafeString* fileName,
                                    const LiveActor* actor, const char* suffixIterKey,
                                    const char* suffixIterSuffix);
bool tryGetActorInitFileIter(ByamlIter* iter, const LiveActor* actor, const char* suffixIterKey,
                             const char* suffixIterSuffix);
bool tryGetActorInitFileName(sead::BufferedSafeString* fileName, const Resource* resource,
                             const char* suffixIterKey, const char* suffixIterSuffix);
bool tryGetActorInitFileName(sead::BufferedSafeString* fileName, const ActorResource* actorRes,
                             const char* suffixIterKey, const char* suffixIterSuffix);
bool tryGetActorInitFileName(sead::BufferedSafeString* fileName, const LiveActor* actor,
                             const char* suffixIterKey, const char* suffixIterSuffix);
bool tryGetActorInitFileSuffixName(sead::BufferedSafeString* fileName, const Resource* resource,
                                   const char* suffixIterKey, const char* suffixIterSuffix);
bool tryGetActorInitFileSuffixName(sead::BufferedSafeString* fileName, const LiveActor* actor,
                                   const char* suffixIterKey, const char* suffixIterSuffix);
const char* tryGetActorInitFileSuffixName(const LiveActor* actor, const char* suffixIterKey,
                                          const char* suffixIterSuffix);
const char* tryGetActorInitFileSuffixName(const Resource* resource, const char* suffixIterKey,
                                          const char* suffixIterSuffix);
void initParameterIoAsActorInfo(ParameterIo* parameterIo, const LiveActor* actor,
                                const char* suffixIterKey, const char* suffixIterSuffix);
void initParameterIoAndLoad(ParameterIo* parameterIo, const LiveActor* actor,
                            const char* suffixIterKey, const char* suffixIterSuffix);

}  // namespace al
