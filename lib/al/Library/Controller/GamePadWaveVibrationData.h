#pragma once

#include "Library/Audio/IAudioFrameProcess.h"
#include "Library/Controller/GamePadSystem.h"
#include "Library/HostIO/HioNode.h"

namespace nn::hid {

class VibrationMixer;

}  // namespace nn::hid

namespace al {

class VibrationPlayer;
class VibrationNodeConnection;

struct WaveVibrationData {
    bool operator<(const WaveVibrationData&);

    char filler[0x18];
};

static_assert(sizeof(WaveVibrationData) == 0x18);

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

private:
    VibrationPlayer* mVibrationPlayer;
    VibrationNodeConnection* _8;
    VibrationNodeConnection* _10;
    s32 _18;
    WaveVibrationData* mData;
    bool _28;
    bool _29;
};

static_assert(sizeof(WaveVibrationPlayer) == 0x30);

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

static_assert(sizeof(WaveVibrationHolder) == 0xe0);

}  // namespace al
