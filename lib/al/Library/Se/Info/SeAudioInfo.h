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

    const char* mName;
    bool mIsPartsUser = false;
    sead::PtrArray<const char*> mPartsUserNameList;
    AudioInfoListWithParts* mEmitterInfoList;
    AudioInfoListWithParts* mActionInfoList;
    AudioInfoListWithParts* mPlayInfoList;
    AudioInfoListWithParts* mHitReactionInfoList;
};
}  // namespace al
