#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>
#include <math/seadVector.h>

#include "Library/HostIO/HioNode.h"

namespace al {
struct AudioSoundArchiveInfo;
class IAudioResourceInfoAccessor;
class SeadAudioPlayer;

class AudioResourcePlayer {
public:
    AudioResourcePlayer(const char* name, const char* fileName, AudioSoundArchiveInfo* archiveInfo,
                        IAudioResourceInfoAccessor* accessor);

    const char* getName() const { return mName; }

    const char* getFileName() const { return mFileName; }

    SeadAudioPlayer* getPlayer() const { return mPlayer; }

    s32 getAddonSize() const { return mAddonSize; }

private:
    const char* mName;
    const char* mFileName;
    SeadAudioPlayer* mPlayer = nullptr;
    s32 mAddonSize = 0;
};

static_assert(sizeof(AudioResourcePlayer) == 0x20);

class AudioResourcePlayerKeeper : public HioNode {
public:
    AudioResourcePlayerKeeper();
    virtual ~AudioResourcePlayerKeeper() {};

    void createAndAppendAudioResourcePlayer(const char* name, const char* fileName,
                                            AudioSoundArchiveInfo* archiveInfo,
                                            IAudioResourceInfoAccessor* accessor);
    // TODO: Rename parameters a and b
    void init(s32 size, bool a, bool b);
    AudioResourcePlayer* tryGetAudioResourcePlayer(const char* name) const;
    SeadAudioPlayer* tryGetSeadAudioPlayer(const char* name) const;
    void update();

private:
    sead::PtrArray<AudioResourcePlayer>* mResources;
    s32 _10 = 0;
    f32 _14 = 1.0f;
    s32 mSize = 0;
    bool mIsInitialized = false;
};

static_assert(sizeof(AudioResourcePlayerKeeper) == 0x20);
}  // namespace al
