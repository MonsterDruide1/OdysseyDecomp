#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>

namespace al {
template <typename T>
class AudioInfoListWithParts;
struct AudioSystemInfo;
class SeEmitterInfo;
class SeInterpolateTypeId;
class SePlayInfo;
class SeResourceSpecificInfo;
class SeResourceRtpcInfo;
class SeRtpcSetting;
class SeUserInfo;
}  // namespace al

namespace alSeDbFunction {
s32 calcIsOneTimeInUserInfo(const al::SeUserInfo*);
al::SeInterpolateTypeId convertInterpolateTypeToId(const char*);
const char* convertInterpolateTypeIdToName(al::SeInterpolateTypeId);
void applyResMainInfoSettingInPlayInfo(const al::SePlayInfo*, const char*);
f32 calcRtpcParam(const al::SeRtpcSetting*, f32);
f32 calcRtpcVolumeMod(const al::SeResourceRtpcInfo*, f32, bool);
al::SeEmitterInfo* createDefaultEmitterInfo(const char*);
sead::PtrArray<al::SeEmitterInfo>* createDefaultEmitterInfoList();
bool setPartsUserList(const al::SeUserInfo*, const al::AudioInfoListWithParts<al::SeUserInfo>*);
void setDefaultEmitter(const al::SeUserInfo*, bool, bool);
void setResourceSpecInfoToResourceInfo(
    const al::SeUserInfo*, const al::AudioInfoListWithParts<al::SeResourceSpecificInfo>*, bool);
const char* tryFindSceneSeUserName(const al::AudioSystemInfo*, const char*, s32);
s32 findRtpcInputTypeByName(const char*);
s32 findSeFluctuationTypeByName(const char*);
const char* createNameAreaAndCopy(const char*);
}  // namespace alSeDbFunction
