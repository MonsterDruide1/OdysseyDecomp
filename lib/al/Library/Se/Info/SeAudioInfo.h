#pragma once

#include <container/seadPtrArray.h>
#include <prim/seadSafeString.h>

namespace al {
template <typename T>
class AudioInfoListWithParts;
class ByamlIter;
class SeActionInfo;
class SeEmitterInfo;
class SeHitReactionInfo;
class SePlayInfo;

struct SeUserInfo {
    static SeUserInfo* createInfo(const ByamlIter&, const sead::SafeString&);

    SeUserInfo();
    ~SeUserInfo();

    bool isIncludingSePartsUser(const SeUserInfo*);

    static s32 compareInfo(const SeUserInfo*, const SeUserInfo*);

    const char* name;
    bool isPartsUser = false;
    sead::PtrArray<const char*> partsUserNameList;
    AudioInfoListWithParts<SeEmitterInfo>* emitterInfoList;
    AudioInfoListWithParts<SeActionInfo>* actionInfoList;
    AudioInfoListWithParts<SePlayInfo>* playInfoList;
    AudioInfoListWithParts<SeHitReactionInfo>* hitReactionInfoList;
};
}  // namespace al
