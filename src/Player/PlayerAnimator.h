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
    bool isSubAnim(sead::SafeString const& subAnimName) const;
    bool isSubAnimEnd() const;
    bool isUpperBodyAnimAttached() const;

    f32 getAnimFrame() const;
    f32 getAnimFrameMax() const;
    f32 getAnimFrameRate() const;
    f32 getSubAnimFrame() const;
    f32 getSubAnimFrameMax() const;
    f32 getBlendWeight(int index);

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

private:
    PlayerModelHolder* mModelHolder;
    al::LiveActor* mPlayerDeco;
    void* unkPtr;
    PlayerAnimFrameCtrl* mAnimFrameCtrl;
    sead::SafeString curAnim;
    unsigned char padding_78[0x78 - 0x30];
    sead::SafeString curSubAnim;
};
