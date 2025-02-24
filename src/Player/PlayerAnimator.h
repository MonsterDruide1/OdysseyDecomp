#pragma once

#include <prim/seadSafeString.h>

#include "Player/PlayerAnimFrameCtrl.h"

namespace al {
class LiveActor;
}
class PlayerModelHolder;

class PlayerAnimator {
public:
    void startAnim(const sead::SafeString& animName);
    void startSubAnim(const sead::SafeString& animName);
    void startSubAnimOnlyAir(const sead::SafeString& animName);
    void startUpperBodyAnimAndHeadVisKeep(const sead::SafeString& animName);
    void startAnimDead();  // chooses one of the 5 death animations and starts that animation
    void endSubAnim();

    void updateAnimFrame();
    void clearUpperBodyAnim();

    bool isAnim(const sead::SafeString& animName) const;
    bool isAnimEnd() const;
    bool isSubAnim(const sead::SafeString& subAnimName) const;
    bool isSubAnimEnd() const;
    bool isUpperBodyAnimAttached() const;

    f32 getAnimFrame() const;
    f32 getAnimFrameMax() const;
    f32 getAnimFrameRate() const;
    f32 getSubAnimFrame() const;
    f32 getSubAnimFrameMax() const;
    f32 getBlendWeight(s32 index);

    void setAnimRate(f32);
    void setAnimRateCommon(f32);
    void setAnimFrame(f32);
    void setAnimFrameCommon(f32);
    void setSubAnimFrame(f32);
    void setSubAnimRate(f32);
    void setBlendWeight(f32, f32, f32, f32, f32, f32);
    void setModelAlpha(f32);
    void setPartsAnimRate(f32, const char*);
    void setPartsAnimFrame(f32, const char*);

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
