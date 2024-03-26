#pragma once

namespace al {
class AudioEffectDataBase;
class AudioSoundArchiveInfo;
class BgmDataBase;
class SeDataBase;

class AudioSystemInfo {
public:
    AudioSystemInfo();

    SeDataBase* getSeDataBase() const { return mSeDataBase; }
    BgmDataBase* getBgmDataBase() const { return mBgmDataBase; }

private:
    void* _0;
    AudioEffectDataBase* mAudioEffectDataBase;
    AudioSoundArchiveInfo* mAudioSoundArchiveInfo;
    SeDataBase* mSeDataBase;
    BgmDataBase* mBgmDataBase;
};
}  // namespace al
