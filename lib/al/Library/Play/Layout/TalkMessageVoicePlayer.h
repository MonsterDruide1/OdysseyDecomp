#pragma once

#include <basis/seadTypes.h>

namespace al {
class IUseAudioKeeper;
class IUseMessageSystem;

class TalkMessageVoicePlayer {
public:
    TalkMessageVoicePlayer();
    void start(const IUseMessageSystem*, const IUseAudioKeeper*, const char16*, s32);
    void stop();
    void update();
    f32 calcVoicePitch(s32) const;
    bool isPlaying() const;

private:
    void* filler[136];
};

static_assert(sizeof(TalkMessageVoicePlayer) == 0x440);

}  // namespace al
