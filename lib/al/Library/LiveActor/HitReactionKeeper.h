#pragma once

#include <math/seadVector.h>

namespace al {
class IUseEffectKeeper;
class IUseAudioKeeper;
class IUseCamera;
class PadRumbleDirector;
class SceneStopCtrl;
class RadialBlurDirector;
class PlayerHolder;
class PadRumbleKeeper;
class Resource;
class HitSensor;
class EffectHitReactionInfo;
class SeHitReactionInfo;

struct HitReactionInfo {
    HitReactionInfo();

    const char* reactionName;
    EffectHitReactionInfo* effectInfo;
    SeHitReactionInfo* seInfo;
    const char* padRumbleType;
    f32 padRumbleDistance;
    const char* padRumbleName;
    const char* padWaveRumbleName;
    f32 padWaveRumbleNearDistance;
    f32 padWaveRumbleFarDistance;
    f32 padWaveRumbleVolumeBalance;
    bool padWaveRumbleIsUseDemo;
    s32 cameraShakeViewIndex;
    const char* cameraShakeType;
    f32 cameraShakeDistance;
    const char* cameraShakeName;
    s32 cameraShakeStep;
    s32 stopSceneFrame;
    bool isStopSceneForHitEffect;
    s32 radialBlurFrame;
    f32 radialBlurRadiusBegin;
    f32 radialBlurRadiusEnd;
    f32 radialBlurStrengthBegin;
    f32 radialBlurStrengthEnd;
    f32 radialBlurAlphaBegin;
    f32 radialBlurAlphaEnd;
    bool radialBlurScreenSpace;
    sead::Vector2f radialBlurScreenPos;
};

class HitReactionKeeper {
public:
    static HitReactionKeeper* tryCreate(const char*, const sead::Vector3f*, IUseEffectKeeper*,
                                        const IUseAudioKeeper*, const IUseCamera*,
                                        PadRumbleDirector*, SceneStopCtrl*, RadialBlurDirector*,
                                        const PlayerHolder*, const Resource*, const char*);
    HitReactionKeeper(const char*, const sead::Vector3f*, IUseEffectKeeper*, const IUseAudioKeeper*,
                      const IUseCamera*, PadRumbleDirector*, SceneStopCtrl*, RadialBlurDirector*,
                      const PlayerHolder*, const Resource*, const char*);

    void start(const char*, const sead::Vector3f*, const HitSensor*, const HitSensor*);
    HitReactionInfo* tryFindReactionInfo(const char*);
    const HitReactionInfo& tryFindReactionInfo(const char*) const;

private:
    const sead::Vector3f* mTrans;
    IUseEffectKeeper* mEffectKeeper;
    const IUseAudioKeeper* mAudioKeeper;
    const IUseCamera* mCamera;
    PadRumbleDirector* mRumbleDirector;
    SceneStopCtrl* mSceneStopCtrl;
    RadialBlurDirector* mRadialBlurDirector;
    const PlayerHolder* mPlayerHolder;
    s32 mNumReactions;
    HitReactionInfo* mReactions;
    PadRumbleKeeper* mRumbleKeeper;
};

}  // namespace al
