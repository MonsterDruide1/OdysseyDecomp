#pragma once

#include <math/seadVector.h>

namespace al {
class PlayerHolder;
class CameraDirector;
class WaveVibrationHolder;
class PadRumbleParam;

class PadRumbleDirector {
public:
    PadRumbleDirector(const PlayerHolder*, const CameraDirector*);

    void setWaveVibrationHolder(WaveVibrationHolder*);
    void update();
    void updateInfoListAll();
    void startRumble(const char*, const sead::Vector3f&, const PadRumbleParam&, int);
    void* findDeadInfoOneTime();  // unknown return type
    void startRumbleNo3D(const char*, const PadRumbleParam&, int);
    void stopPadRumbleOneTime(const char*, int);
    void startRumbleLoop(const char*, const sead::Vector3f*, const PadRumbleParam&, int);
    void* findDeadInfo();  // unknown return type
    void startRumbleLoopNo3D(const char*, const sead::Vector3f*, const PadRumbleParam&, int);
    void stopRumbleLoop(const char*, const sead::Vector3f*, int);
    void* findInfo(const char*, const sead::Vector3f*, int);  // unknown return type
    bool checkIsAliveRumbleLoop(const char*, const sead::Vector3f*, int);
    void stopAllRumble();
    void clearAllInfoList();
    void pause();
    void endPause();
    void changeRumbleLoopVolume(const char*, const sead::Vector3f*, float, float, int);
    void updateInfoListLoop();
    void changeRumbleLoopPitch(const char*, const sead::Vector3f*, float, float, int);
    void startRumbleWithVolume(const char*, float, float, int);
    void startRumbleDirectValue(float, float, float, float, float, float, int);
    void stopRumbleDirectValue(int);
    void updateInfoListOneTime();
    void testStartPadRumbleWithVolumeNoActor(const char*, float, float);

private:
    void* filler[14];
};

}  // namespace al
