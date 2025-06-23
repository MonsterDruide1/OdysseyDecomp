#include "Library/LiveActor/ActorResourceFunction.h"

#include <prim/seadSafeString.h>

#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Model/ModelKeeper.h"
#include "Library/Resource/Resource.h"
#include "Library/Resource/ResourceHolder.h"
#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/ParameterObj.h"

namespace al {

bool isExistModelResource(const LiveActor* actor) {
    return actor->getModelKeeper() != nullptr;
}

bool isExistAnimResource(const LiveActor* actor) {
    return tryGetAnimResource(actor) != nullptr;
}

Resource* tryGetAnimResource(const LiveActor* actor) {
    return actor->getModelKeeper()->getAnimResource();
}

bool isExistModelResourceYaml(const LiveActor* actor, const char* name, const char* suffix) {
    return isExistResourceYaml(getModelResource(actor), name, suffix);
}

Resource* getModelResource(const LiveActor* actor) {
    return actor->getModelKeeper()->getModelResource();
}

bool isExistAnimResourceYaml(const LiveActor* actor, const char* name, const char* suffix) {
    return isExistResourceYaml(getAnimResource(actor), name, suffix);
}

Resource* getAnimResource(const LiveActor* actor) {
    return actor->getModelKeeper()->getAnimResource();
}

bool isExistModelOrAnimResourceYaml(const LiveActor* actor, const char* name, const char* suffix) {
    if (isExistModelResourceYaml(actor, name, suffix))
        return true;
    return tryGetAnimResource(actor) && isExistResourceYaml(getAnimResource(actor), name, suffix);
}

const u8* getModelResourceYaml(const LiveActor* actor, const char* name, const char* suffix) {
    return findResourceYaml(getModelResource(actor), name, suffix);
}

const u8* getAnimResourceYaml(const LiveActor* actor, const char* name, const char* suffix) {
    return findResourceYaml(getAnimResource(actor), name, suffix);
}

const u8* getModelOrAnimResourceYaml(const LiveActor* actor, const char* name, const char* suffix) {
    if (isExistModelResourceYaml(actor, name, suffix))
        return getModelResourceYaml(actor, name, suffix);
    else
        return getAnimResourceYaml(actor, name, suffix);
}

const u8* getMapPartsResourceYaml(const ActorInitInfo& initInfo, const char* name) {
    sead::FixedSafeString<256> modelName, path;
    makeMapPartsModelName(&modelName, &path, *initInfo.placementInfo);
    return findOrCreateResource(path, nullptr)->getByml(name);
}

const u8* tryGetMapPartsResourceYaml(const ActorInitInfo& initInfo, const char* name) {
    sead::FixedSafeString<256> modelName, path;
    makeMapPartsModelName(&modelName, &path, *initInfo.placementInfo);
    return findOrCreateResource(path, nullptr)->tryGetByml(name);
}

bool tryMakeInitFileName(sead::BufferedSafeString* fileName, const Resource* resource,
                         const char* suffixIterKey, const char* suffixIterSuffix,
                         const char* suffixIterName) {
    const char* suffix = nullptr;
    ByamlIter iter;
    bool tryResult = tryGetSuffixIter(&iter, resource, suffixIterName, suffixIterSuffix);
    if (tryResult) {
        if (!iter.isExistKey(suffixIterKey))
            return false;
        iter.tryGetStringByKey(&suffix, suffixIterKey);
    }
    createFileNameBySuffix(fileName, suffixIterKey, suffix);
    return true;
}

bool tryGetSuffixIter(ByamlIter* iter, const Resource* resource, const char* name,
                      const char* suffix) {
    if (!suffix)
        return false;
    al::StringTmp<128> fileNameBySuffix;
    createFileNameBySuffix(&fileNameBySuffix, name, suffix);
    const u8* resBymlData = resource->tryGetByml(fileNameBySuffix);
    if (!resBymlData)
        return false;

    *iter = {resBymlData};
    return true;
}

bool tryGetInitFileIterAndName(ByamlIter* iter, sead::BufferedSafeString* fileName,
                               const Resource* resource, const char* suffixIterKey,
                               const char* suffixIterSuffix, const char* suffixIterName) {
    al::StringTmp<128> tmpFileName;
    if (!tryMakeInitFileName(&tmpFileName, resource, suffixIterKey, suffixIterSuffix,
                             suffixIterName))
        return false;
    const u8* resBymlData = resource->tryGetByml(tmpFileName.cstr());
    if (!resBymlData)
        return false;

    if (iter)
        *iter = {resBymlData};
    if (fileName)
        fileName->format(tmpFileName.cstr());
    return true;
}

bool tryGetActorInitFileIterAndName(ByamlIter* iter, sead::BufferedSafeString* fileName,
                                    const Resource* resource, const char* suffixIterKey,
                                    const char* suffixIterSuffix) {
    return tryGetInitFileIterAndName(iter, fileName, resource, suffixIterKey, suffixIterSuffix,
                                     "InitActor");
}

bool tryGetActorInitFileIter(ByamlIter* iter, const Resource* resource, const char* suffixIterKey,
                             const char* suffixIterSuffix) {
    return tryGetActorInitFileIterAndName(iter, nullptr, resource, suffixIterKey, suffixIterSuffix);
}

bool tryGetActorInitFileIterAndName(ByamlIter* iter, sead::BufferedSafeString* fileName,
                                    const LiveActor* actor, const char* suffixIterKey,
                                    const char* suffixIterSuffix) {
    return tryGetInitFileIterAndName(iter, fileName, getModelResource(actor), suffixIterKey,
                                     suffixIterSuffix, "InitActor");
}

bool tryGetActorInitFileIter(ByamlIter* iter, const LiveActor* actor, const char* suffixIterKey,
                             const char* suffixIterSuffix) {
    return tryGetActorInitFileIterAndName(iter, nullptr, actor, suffixIterKey, suffixIterSuffix);
}

bool tryGetActorInitFileName(sead::BufferedSafeString* fileName, const Resource* resource,
                             const char* suffixIterKey, const char* suffixIterSuffix) {
    return tryMakeInitFileName(fileName, resource, suffixIterKey, suffixIterSuffix, "InitActor");
}

bool tryGetActorInitFileName(sead::BufferedSafeString* fileName, const ActorResource* actorRes,
                             const char* suffixIterKey, const char* suffixIterSuffix) {
    return tryGetActorInitFileName(fileName, actorRes->getModelRes(), suffixIterKey,
                                   suffixIterSuffix);
}

bool tryGetActorInitFileName(sead::BufferedSafeString* fileName, const LiveActor* actor,
                             const char* suffixIterKey, const char* suffixIterSuffix) {
    return tryGetActorInitFileName(fileName, getModelResource(actor), suffixIterKey,
                                   suffixIterSuffix);
}

bool tryGetActorInitFileSuffixName(sead::BufferedSafeString* fileName, const Resource* resource,
                                   const char* suffixIterKey, const char* suffixIterSuffix) {
    const char* suffix = nullptr;
    ByamlIter iter;
    bool tryResult = tryGetSuffixIter(&iter, resource, "InitActor", suffixIterSuffix);
    if (tryResult) {
        if (!iter.isExistKey(suffixIterKey))
            return false;
        iter.tryGetStringByKey(&suffix, suffixIterKey);
    }

    if (fileName)
        fileName->copy(suffix == nullptr ? "" : suffix);
    return true;
}

bool tryGetActorInitFileSuffixName(sead::BufferedSafeString* fileName, const LiveActor* actor,
                                   const char* suffixIterKey, const char* suffixIterSuffix) {
    return tryGetActorInitFileSuffixName(fileName, getModelResource(actor), suffixIterKey,
                                         suffixIterSuffix);
}

const char* tryGetActorInitFileSuffixName(const LiveActor* actor, const char* suffixIterKey,
                                          const char* suffixIterSuffix) {
    return tryGetActorInitFileSuffixName(getModelResource(actor), suffixIterKey, suffixIterSuffix);
}

const char* tryGetActorInitFileSuffixName(const Resource* resource, const char* suffixIterKey,
                                          const char* suffixIterSuffix) {
    const char* suffix = nullptr;
    ByamlIter iter;
    bool tryResult = tryGetSuffixIter(&iter, resource, "InitActor", suffixIterSuffix);
    if (tryResult) {
        if (!iter.isExistKey(suffixIterKey))
            return nullptr;
        iter.tryGetStringByKey(&suffix, suffixIterKey);
    }
    return suffix;
}

void initParameterIoAsActorInfo(ParameterIo* parameterIo, const LiveActor* actor,
                                const char* suffixIterKey, const char* suffixIterSuffix) {}

void initParameterIoAndLoad(ParameterIo* parameterIo, const LiveActor* actor,
                            const char* suffixIterKey, const char* suffixIterSuffix) {
    ByamlIter iter;
    tryGetActorInitFileIter(&iter, actor, suffixIterKey, suffixIterSuffix);
    parameterIo->tryGetParam(iter);
}

}  // namespace al
