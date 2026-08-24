#pragma once

#include <container/seadPtrArray.h>

namespace al {

class ByamlIter;
template<typename T> class AudioInfoListWithParts;

struct AudioResourceLoadInfo {
    AudioResourceLoadInfo();

    void initialize(const char* name_, bool isBgm_) {
        name = name_;
        isBgm = isBgm_;
    }

    const char* name = nullptr;
    bool isBgm = false;
};

static_assert(sizeof(AudioResourceLoadInfo) == 0x10);

struct AudioLoadGroupList {
    sead::PtrArray<AudioResourceLoadInfo>* _0;
    void* _8;
    sead::PtrArray<AudioResourceLoadInfo>* resourceLoadInfos;
};
static_assert(sizeof(AudioLoadGroupList) == 0x18);

struct AudioResourceLoadGroupInfo {
    AudioResourceLoadGroupInfo();

    static AudioResourceLoadGroupInfo* createInfo(const ByamlIter& iter);
    static s32 compareInfo(const AudioResourceLoadGroupInfo* lhs,
                           const AudioResourceLoadGroupInfo* rhs);

    const char* name = nullptr;
    AudioInfoListWithParts<AudioResourceLoadInfo>* userManagementGroupLoadInfoList = nullptr;
    AudioInfoListWithParts<AudioResourceLoadInfo>* addonSoundArchiveLoadInfoList = nullptr;
};

static_assert(sizeof(AudioResourceLoadGroupInfo) == 0x18);

}  // namespace al
