#pragma once

#include <container/seadPtrArray.h>
#include <prim/seadSafeString.h>

namespace al {
class AudioInfoListWithParts;
class ByamlIter;

struct SeUserInfo {
    static SeUserInfo* createInfo(const ByamlIter&, const sead::SafeString&);

    SeUserInfo();
    ~SeUserInfo();

    bool isIncludingSePartsUser(const SeUserInfo*);

    static s32 compareInfo(const SeUserInfo*, const SeUserInfo*);

    const char* name;
    bool isPartsUser = false;
    sead::PtrArray<const char*> partsUserNameList;
    AudioInfoListWithParts* emitterInfoList;
    AudioInfoListWithParts* actionInfoList;
    AudioInfoListWithParts* playInfoList;
    AudioInfoListWithParts* hitReactionInfoList;
};
}  // namespace al
