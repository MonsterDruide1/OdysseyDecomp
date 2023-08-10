#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class AudioSystemInfo;
class SeDirector;
class MeInfo;
class SeadAudioPlayer;
class SePlayParamList;
class SeRequestParam;
class IAudioResourceLoader;
class ModelKeeper;
class CameraDirector;

class SeKeeper {
public:
    static void create(const AudioSystemInfo*, SeDirector*, const char*, const sead::Vector3f*,
                       const sead::Matrix34f*, const ModelKeeper*, CameraDirector*);
    SeKeeper(const AudioSystemInfo*, SeDirector*, const char*, const sead::Vector3f*,
             const sead::Matrix34f*, const ModelKeeper*, CameraDirector*);

    void tryLoadAddonSoundArchive(SeadAudioPlayer*);
    void requestPlaySe(const char*, float, const char*, const SePlayParamList*, const MeInfo*, bool,
                       const char*, const sead::Vector3f*);
    void tryRequestPlaySe(const char*, float, const char*, const SePlayParamList*, const MeInfo*,
                          bool, const char*, const sead::Vector3f*);
    void requestPlaySeIdLocal(SeRequestParam*, const char*);
    void requestPlayLoopSeSequence(const char*, MeInfo const*, int);
    void stopSe(const char*, int, bool, const char*);
    void tryFindPlayInfo(const char*) const;
    void setLifeTimeForHoldCall(const char*, unsigned int, const char*);
    void stopAll(int, const char*);
    void separatePlayingSePosFromEmitter();
    void tryGetCurrentStreamSamplePos(const char*, const char*) const;
    void isPlaying(const char*);
    void checkIsPlayingSe(const char*, const char*);
    void validate();
    void invalidate();
    void startClipped();
    void endClipped();
    void appear();
    void kill();
    void resetPosition();
    void setIsInWater(bool);
    void getWaterState();
    void setIsMaterialWet(bool);
    void setIsMaterialPuddle(bool);
    void tryUpdateMaterial(const char*);
    void setSeSourceVolume(float);
    void setSyncParamPtr(float const*, const char*);
    void setSyncParamPtrInt(int const*, const char*);
    void setEmitterPoseMtxPtr(const sead::Matrix34f*, const char*);
    void setEmitterPosePosPtr(const sead::Vector3f*, const char*);
    void loadSe(IAudioResourceLoader*);
};
}  // namespace al
