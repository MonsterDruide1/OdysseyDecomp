#pragma once

namespace al {
class AudioEffectDataBase;
class AudioSoundArchiveInfo;
class BgmDataBase;
class SeDataBase;

struct AudioSystemInfo {
    AudioSystemInfo();

    void* _0;
    AudioEffectDataBase* audioEffectDataBase;
    AudioSoundArchiveInfo* audioSoundArchiveInfo;
    SeDataBase* seDataBase;
    BgmDataBase* bgmDataBase;
};
}  // namespace al
