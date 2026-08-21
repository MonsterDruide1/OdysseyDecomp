#pragma once
#include "container/seadPtrArray.h"

namespace al {

struct AudioResourceLoadInfo {
    AudioResourceLoadInfo();

    void setName(const char* named, bool kek) {
        name = named;
        isBgm = kek;
    }

    const char* name = nullptr;
    bool isBgm = false;

    static s32 compareInfo(const AudioResourceLoadInfo* lhs, const AudioResourceLoadInfo* rhs);
};

struct AudioLoadGroupList {
    sead::PtrArray<AudioResourceLoadInfo>* unk1;
    void* unk2;
    sead::PtrArray<AudioResourceLoadInfo>* resourceLoadInfos;
};

struct AudioResourceLoadGroupInfo {
    AudioResourceLoadGroupInfo();

    static AudioResourceLoadGroupInfo* createInfo(const ByamlIter& iter);
    static s32 compareInfo(const AudioResourceLoadGroupInfo* lhs,
                           const AudioResourceLoadGroupInfo* rhs);

    const char* name = nullptr;
    AudioInfoListWithParts<AudioResourceLoadInfo>* userManagementGroupLoadInfoList = nullptr;
    AudioInfoListWithParts<AudioResourceLoadInfo>* addonSoundArchiveLoadInfoList = nullptr;
};

}  // namespace al
