#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
struct AudioSystemInfo;
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
    void requestPlaySe(const char*, f32, const char*, const SePlayParamList*, const MeInfo*, bool,
                       const char*, const sead::Vector3f*);
    void tryRequestPlaySe(const char*, f32, const char*, const SePlayParamList*, const MeInfo*,
                          bool, const char*, const sead::Vector3f*);
    void requestPlaySeIdLocal(SeRequestParam*, const char*);
    void requestPlayLoopSeSequence(const char*, const MeInfo*, s32);
    void stopSe(const char*, s32, bool, const char*);
    void tryFindPlayInfo(const char*) const;
    void setLifeTimeForHoldCall(const char*, u32, const char*);
    void stopAll(s32, const char*);
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
    void setSeSourceVolume(f32);
    void setSyncParamPtr(const f32*, const char*);
    void setSyncParamPtrInt(const s32*, const char*);
    void setEmitterPoseMtxPtr(const sead::Matrix34f*, const char*);
    void setEmitterPosePosPtr(const sead::Vector3f*, const char*);
    void loadSe(IAudioResourceLoader*);
};
}  // namespace al
