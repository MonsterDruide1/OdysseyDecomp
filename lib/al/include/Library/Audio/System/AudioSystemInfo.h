#pragma once

namespace al {
class AudioEffectDataBase;
class AudioSoundArchiveInfo;
class BgmDataBase;
class SeDataBase;

class AudioSystemInfo{
public:
    AudioSystemInfo();

    void* mUnknown0;
    AudioEffectDataBase* mAudioEffectDataBase;
    AudioSoundArchiveInfo** mAudioSoundArchiveInfo;
    SeDataBase* mSeDataBase;
    BgmDataBase* mBgmDataBase;

};
}