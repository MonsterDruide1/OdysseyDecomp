#pragma once

#include <math/seadMatrix.h>
#include <prim/seadSafeString.h>

namespace al {
class ActorDitherAnimator;
class LiveActor;
}  // namespace al

class PlayerModelHolder;
class PlayerAnimFrameCtrl;

class PlayerAnimator {
public:
    PlayerAnimator(const PlayerModelHolder*, al::ActorDitherAnimator*);
    void startAnim(const sead::SafeString&);
    void updateAnimFrame();
    void updateModel();
    void copyAnim();
    void startAnimCommon(const sead::SafeString&);
    void setAnimRate(f32);
    void startAnimSpinAttack(const sead::SafeString&);
    void setAnimRateCommon(f32);
    void setAnimFrame(f32);
    void setAnimFrameCommon(f32);
    bool isAnimEnd() const;
    bool isAnim(const sead::SafeString&) const;
    bool isCurrentAnimOneTime() const;
    f32 getAnimFrame() const;
    f32 getAnimFrameMax() const;
    f32 getAnimFrameRate() const;
    void clearInterpolation();
    void startSubAnim(const sead::SafeString&);
    void startSubAnimOnlyAir(const sead::SafeString&);
    void endSubAnim();
    void applyBlendWeight();
    void setSubAnimFrame(f32);
    void setSubAnimRate(f32);
    bool isSubAnimEnd() const;
    bool isSubAnim(const sead::SafeString&) const;
    f32 getSubAnimFrame() const;
    f32 getSubAnimFrameMax() const;
    bool isUpperBodyAnimAttached() const;
    bool isUpperBodyAnimEnd() const;
    bool isUpperBodyAnim(const sead::SafeString&) const;
    void startUpperBodyAnim(const sead::SafeString&);
    void startPartsPartialAnim(const sead::SafeString&);
    void startUpperBodyAnimSubParts(const sead::SafeString&);
    void startUpperBodyAnimAndHeadVisKeep(const sead::SafeString&);
    void clearUpperBodyAnim();
    void setBlendWeight(f32, f32, f32, f32, f32, f32);
    f32 getBlendWeight(s32);
    void startAnimDead();
    void startPress();
    void forceCapOn();
    void forceCapOff();
    f32 getModelAlpha() const;
    void updateModelAlpha();
    void setModelAlpha(f32);
    void resetModelAlpha();
    void endDemoInvalidateModelAlpha();
    void startSnapShotMode();
    void endSnapShotMode();
    void startEyeControlAnim(bool);
    void endEyeControlAnim(s32);
    void clearEndEyeControlAnimDelay();
    void updateEyeControlAnim();
    void startRightHandAnim(const char*);
    void overwrideYoshiEatVis();
    void restartYoshiActionVis();
    f32 getMario3DWaitFrameMax() const;
    f32 getRunStartAnimFrameMax() const;
    f32 getRunStartAnimBlendRate() const;
    void recordRunStartAnimRate(f32);
    void calcModelJointRootMtx(sead::Matrix34f*) const;
    void startPartsAnim(const sead::SafeString&);
    void setPartsAnimRate(f32, const char*);
    void setPartsAnimFrame(f32, const char*);
    void copyAnimLocal();

    bool isSubAnimPlaying() const { return mIsSubAnimPlaying; }

private:
    PlayerModelHolder* mModelHolder;
    al::LiveActor* mPlayerDeco;
    void* _10;
    PlayerAnimFrameCtrl* mAnimFrameCtrl;
    sead::FixedSafeString<64> mCurAnim;
    sead::FixedSafeString<64> mCurSubAnim;
    sead::FixedSafeString<64> mCurUpperBodyAnim;
    sead::FixedSafeString<64> _128;
    char padding_180[0x1A2 - 0x180];
    bool mIsSubAnimPlaying;
};

static_assert(sizeof(PlayerAnimator) == 0x1a8);
