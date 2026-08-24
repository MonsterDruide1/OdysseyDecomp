#pragma once

#include <heap/seadHeap.h>

namespace al {
class AudioEffectDataBase;
class AudioSoundArchiveInfo;
class BgmDataBase;
class CollisionCodeList;
class SeDataBase;

struct AudioSystemInitInfo {
    void setHeapSize(sead::Heap* heap) {
        if (!heap) {
            systemHeapSize = 0;
            return;
        }
        systemHeapSize = heap->getFreeSize();
    }

    void setMaterialCode(CollisionCodeList* codeList, CollisionCodeList* codePrefixList) {
        materialCodeList = codeList;
        materialCodePrefixList = codePrefixList;
    }

    const char* seDataName = nullptr;
    const char* seBgmName = nullptr;
    bool _10 = true;
    bool _11 = true;
    bool isBgmOnSameMixIndex = false;
    f32 masterVolume = 1.0f;
    f32 dockedVolume = 1.0f;
    f32 undockedVolume = 1.0f;
    s32 systemHeapSize = -1;
    s32 _24 = 0;
    bool useAudioMaximizer = false;
    bool changeInputBgmChannelVolume = false;
    f32 monoVolume = 1.0f;
    f32 stereoVolume = 1.0f;
    CollisionCodeList* materialCodeList = nullptr;
    CollisionCodeList* materialCodePrefixList = nullptr;
    s32 cacheSizePerSound = 0;
};

static_assert(sizeof(AudioSystemInitInfo) == 0x50);

struct AudioSystemInfo {
    AudioSystemInfo();

    void* _0;
    AudioEffectDataBase* audioEffectDataBase;
    AudioSoundArchiveInfo* audioSoundArchiveInfo;
    SeDataBase* seDataBase;
    BgmDataBase* bgmDataBase;
};
}  // namespace al
