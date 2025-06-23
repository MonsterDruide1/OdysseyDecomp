#pragma once

#include <prim/seadSafeString.h>

namespace al {
class ByamlIter;
class LiveActor;
class ActorResource;
struct ActorInitInfo;
class ParameterIo;
class Resource;

bool isExistModelResource(const LiveActor*);
bool isExistAnimResource(const LiveActor*);
Resource* tryGetAnimResource(const LiveActor*);
bool isExistModelResourceYaml(const LiveActor*, const char*, const char*);
Resource* getModelResource(const LiveActor*);
bool isExistAnimResourceYaml(const LiveActor*, const char*, const char*);
Resource* getAnimResource(const LiveActor*);
bool isExistModelOrAnimResourceYaml(const LiveActor*, const char*, const char*);
const u8* getModelResourceYaml(const LiveActor*, const char*, const char*);
const u8* getAnimResourceYaml(const LiveActor*, const char*, const char*);
const u8* getModelOrAnimResourceYaml(const LiveActor*, const char*, const char*);
const u8* getMapPartsResourceYaml(const ActorInitInfo&, const char*);
const u8* tryGetMapPartsResourceYaml(const ActorInitInfo&, const char*);
bool tryMakeInitFileName(sead::BufferedSafeString*, const Resource*, const char*, const char*,
                         const char*);
bool tryGetSuffixIter(ByamlIter*, const Resource*, const char*, const char*);
bool tryGetInitFileIterAndName(ByamlIter*, sead::BufferedSafeString*, const Resource*, const char*,
                               const char*, const char*);
bool tryGetActorInitFileIterAndName(ByamlIter*, sead::BufferedSafeString*, const Resource*,
                                    const char*, const char*);
bool tryGetActorInitFileIter(ByamlIter*, const Resource*, const char*, const char*);
bool tryGetActorInitFileIterAndName(ByamlIter*, sead::BufferedSafeString*, const LiveActor*,
                                    const char*, const char*);
bool tryGetActorInitFileIter(ByamlIter*, const LiveActor*, const char*, const char*);
bool tryGetActorInitFileName(sead::BufferedSafeString*, const Resource*, const char*, const char*);
bool tryGetActorInitFileName(sead::BufferedSafeString*, const ActorResource*, const char*,
                             const char*);
bool tryGetActorInitFileName(sead::BufferedSafeString*, const LiveActor*, const char*, const char*);
bool tryGetActorInitFileSuffixName(sead::BufferedSafeString*, const Resource*, const char*,
                                   const char*);
bool tryGetActorInitFileSuffixName(sead::BufferedSafeString*, const LiveActor*, const char*,
                                   const char*);
const char* tryGetActorInitFileSuffixName(const LiveActor*, const char*, const char*);
const char* tryGetActorInitFileSuffixName(const Resource*, const char*, const char*);
void initParameterIoAsActorInfo(ParameterIo*, const LiveActor*, const char*, const char*);
void initParameterIoAndLoad(ParameterIo*, const LiveActor*, const char*, const char*);
}  // namespace al
