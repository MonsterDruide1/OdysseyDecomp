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

    const char* mReactionName;
    EffectHitReactionInfo* mEffectHitReactionInfo;
    SeHitReactionInfo* mSeHitReactionInfo;
    const char* mPadRumbleType;
    f32 mPadRumbleDistance;
    const char* mPadRumbleName;
    const char* mPadWaveRumbleName;
    f32 mPadWaveRumbleNearDistance;
    f32 mPadWaveRumbleFarDistance;
    f32 mPadWaveRumbleVolumeBalance;
    bool mPadWaveRumbleIsUseDemo;
    s32 mCameraShakeViewIndex;
    const char* mCameraShakeType;
    f32 mCameraShakeDistance;
    const char* mCameraShakeName;
    s32 mCameraShakeStep;
    s32 mStopSceneFrame;
    bool mIsStopSceneForHitEffect;
    s32 mRadialBlurFrame;
    f32 mRadialBlurRadiusBegin;
    f32 mRadialBlurRadiusEnd;
    f32 mRadialBlurStrengthBegin;
    f32 mRadialBlurStrengthEnd;
    f32 mRadialBlurAlphaBegin;
    f32 mRadialBlurAlphaEnd;
    bool mRadialBlurScreenSpace;
    sead::Vector2f mRadialBlurScreenPos;
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
    PadRumbleDirector* mPadRumbleDirector;
    SceneStopCtrl* mSceneStopCtrl;
    RadialBlurDirector* mRadialBlurDirector;
    const PlayerHolder* mPlayerHolder;
    s32 mNumReactions;
    HitReactionInfo* mReactions;
    PadRumbleKeeper* mPadRumbleKeeper;
};

}  // namespace al
