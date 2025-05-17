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

    void set_420(bool value) { _420 = value; }

private:
    unsigned char padding1[0x420];
    bool _420;
    unsigned char padding2[0x1f];
};

static_assert(sizeof(TalkMessageVoicePlayer) == 0x440);

}  // namespace al
