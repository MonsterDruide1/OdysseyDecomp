#pragma once

#include <math/seadVector.h>

namespace al {
class PlayerHolder;
class CameraDirector;
class WaveVibrationHolder;
struct PadRumbleParam;
class PadRumbleKeeper;
class LiveActor;

class PadRumbleDirector {
public:
    PadRumbleDirector(const PlayerHolder*, const CameraDirector*);

    void setWaveVibrationHolder(WaveVibrationHolder*);
    void update();
    void updateInfoListAll();
    void startRumble(const char*, const sead::Vector3f&, const PadRumbleParam&, s32);
    void* findDeadInfoOneTime();  // unknown return type
    void startRumbleNo3D(const char*, const PadRumbleParam&, s32);
    void stopPadRumbleOneTime(const char*, s32);
    void startRumbleLoop(const char*, const sead::Vector3f*, const PadRumbleParam&, s32);
    void* findDeadInfo();  // unknown return type
    void startRumbleLoopNo3D(const char*, const sead::Vector3f*, const PadRumbleParam&, s32);
    void stopRumbleLoop(const char*, const sead::Vector3f*, s32);
    void* findInfo(const char*, const sead::Vector3f*, s32);  // unknown return type
    bool checkIsAliveRumbleLoop(const char*, const sead::Vector3f*, s32);
    void stopAllRumble();
    void clearAllInfoList();
    void pause();
    void endPause();
    void changeRumbleLoopVolume(const char*, const sead::Vector3f*, f32, f32, s32);
    void updateInfoListLoop();
    void changeRumbleLoopPitch(const char*, const sead::Vector3f*, f32, f32, s32);
    void startRumbleWithVolume(const char*, f32, f32, s32);
    void startRumbleDirectValue(f32, f32, f32, f32, f32, f32, s32);
    void stopRumbleDirectValue(s32);
    void updateInfoListOneTime();
    void testStartPadRumbleWithVolumeNoActor(const char*, f32, f32);

private:
    void* filler[14];
};

PadRumbleKeeper* createPadRumbleKeeper(const LiveActor*, s32);

}  // namespace al
