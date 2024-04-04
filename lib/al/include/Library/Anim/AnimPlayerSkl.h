#pragma once

#include <basis/seadTypes.h>

#include "Project/Anim/AnimPlayerBase.h"

namespace al {
struct AnimPlayerInitInfo;
struct SklAnimRetargettingInfo;

class AnimPlayerSkl : public AnimPlayerBase {
public:
    static AnimPlayerSkl* tryCreate(const AnimPlayerInitInfo*, s32);

    AnimPlayerSkl(const AnimPlayerInitInfo*, s32);

    bool startSklAnim(const char*, const char*, const char*, const char*, const char*, const char*,
                      const char*);
    void startPartialAnim(const char*, s32, s32, const SklAnimRetargettingInfo*);

    void updateLast() override;
    void update();
    void reset();
    void clearSklAnimBlend();
    bool calcSklAnim();

    bool initInterp(const char*);
    void initPartialAnim(s32, s32, s32);
    void prepareAnimInterpDirect(s32);
    bool calcNeedUpdateAnimNext();
    void clearPartialAnim(s32);

    void setSklAnim(const char*, s32);
    void setSklAnimBlendWeight(s32, f32);
    void setSklAnimFrame(s32, f32);
    void setSklAnimFrameRate(s32, f32);
    void setPartialAnimFrame(s32, f32);
    void setPartialAnimFrameRate(s32, f32);
    void addPartialAnimJoint(s32, const char*, const char*);
    void addPartialAnimJointRecursive(s32, const char*);

    const char* getPlayingSklAnimName(s32);
    const char* getPlayingPartialSklAnimName(s32);
    f32 getSklAnimBlendWeight(s32);
    s32 getSklAnimBlendNum();
    f32 getSklAnimFrame(s32);
    f32 getSklAnimFrameMax(s32);
    f32 getSklAnimFrameMax(const char*);
    f32 getSklAnimFrameRate(s32);
    f32 getSklAnimFrameRate(s32, f32);
    s32 getPartialAnimSlotNum();
    s32 getJoitsAmountFromJos32(const char*);
    f32 getPartialAnimFrame(s32);
    f32 getPartialAnimFrameRate(s32);

    bool isSklAnimExist(const char*);
    bool isSklAnimEnd(s32);
    bool isSklAnimOneTime(s32);
    bool isSklAnimOneTime(const char*);
    bool isSklAnimPlaying(s32);
    bool isPartialAnimEnd(s32);
    bool isPartialAnimOneTime(s32);
    bool isPartialAnimAttached(s32);
};
}  // namespace al
