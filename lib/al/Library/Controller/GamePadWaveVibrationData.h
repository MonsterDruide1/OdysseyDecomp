#pragma once

#include <nn/hid.h>

#include "Library/Audio/IAudioFrameProcess.h"

namespace nn::hid {

class VibrationMixer;

} // namespace nn::hid

namespace al {

struct WaveVibrationData {
    bool operator <(const WaveVibrationData&);
};

class WaveVibrationPlayer {
public:
    WaveVibrationPlayer(nn::hid::VibrationMixer*, nn::hid::VibrationMixer*);

    void update();
    void startOneTime(const WaveVibrationData*, f32, f32, f32, f32, bool);
    void start(const WaveVibrationData*, f32, f32, f32, f32, bool);
    void startLoop(const WaveVibrationData*, f32, f32, f32, f32, bool);
    void stop();
    void pause();
    void endPause();
    void changeVolume(f32, f32);
    bool isPlaying() const;
    void changePitch(f32, f32);
    void changeVolumeAndPitch(f32, f32, f32, f32);
};

class WaveVibrationHolder : public HioNode, public aal::IAudioFrameProcess {
public:
    WaveVibrationHolder(const GamePadSystem*);

    void loadData();
    void update();
    WaveVibrationData* findWaveVibrationData(const char*) const;
    WaveVibrationPlayer* findPlayableVibrationPlayer(s32) const;
    s32 findId(s32) const;
    void stopAll();
    s32 getUsePadNum() const;
    void pause();
    void endPause();
    void startVibrationDirectValue(s32, f32, f32, f32, f32, f32, f32);
    void stopVibrationDirectValue(s32);
    
private:
    char filler[0xd8];
};

}  // namespace al
