#pragma once

#include <basis/seadTypes.h>

#include "Project/Anim/AnimPlayerBase.h"

namespace nn::g3d {
class SkeletalAnimObj;
class SkeletalAnimBlender;
}  // namespace nn::g3d

namespace al {
class AnimInfoTable;
struct AnimPlayerInitInfo;
class ModelAnimInterp;
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

    void setRetargettingInfo(SklAnimRetargettingInfo* info) { mRetargettingInfo = info; }

private:
    AnimInfoTable* mAnimInfoTable;
    u32 _10;
    void* _18;
    u32 mAnimObjCount0;
    nn::g3d::SkeletalAnimObj* mSkelAnimObjStack0;
    u32 mAnimObjCount1;
    nn::g3d::SkeletalAnimObj* mSkelAnimObjStack1;
    ModelAnimInterp* mModelAnimInterp;
    nn::g3d::SkeletalAnimBlender* mSkelAnimBlender;
    u32 mBlendWeightCount;
    void* mBlendWeightStack;
    u32 mPartialAnimObjCount;
    nn::g3d::SkeletalAnimObj* mPartialAnimObjStack;
    void* _70;
    u32 _78;
    SklAnimRetargettingInfo* mRetargettingInfo;
    bool _88;
    bool _89;
};
}  // namespace al
