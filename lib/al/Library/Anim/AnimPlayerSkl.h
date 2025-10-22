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

    bool calcNeedUpdateAnimNext() override;
    void updateLast() override;

    bool startSklAnim(const char*, const char*, const char*, const char*, const char*, const char*,
                      const char*);
    void clearSklAnimBlend();
    void setSklAnim(const char*, s32);
    bool calcSklAnim();
    void initInterp(const char*);
    void setSklAnimBlendWeight(s32, f32);
    f32 getSklAnimBlendWeight(s32) const;
    s32 getSklAnimBlendNum() const;
    f32 getSklAnimFrame(s32) const;
    nn::g3d::SkeletalAnimObj* getAnimObj(s32) const;
    void setSklAnimFrame(s32, f32);
    nn::g3d::SkeletalAnimObj* getAnimObj(s32);
    f32 getSklAnimFrameMax(s32) const;
    f32 getSklAnimFrameMax(const char*) const;
    f32 getSklAnimFrameRate(s32) const;
    void setSklAnimFrameRate(s32, f32);
    bool isSklAnimExist(const char*) const;
    bool isSklAnimEnd(s32) const;
    bool isSklAnimOneTime(s32) const;
    bool isSklAnimOneTime(const char*) const;
    bool isSklAnimPlaying(s32) const;
    const char* getPlayingSklAnimName(s32) const;
    void update();
    void reset();
    void prepareAnimInterpDirect(s32);
    void initPartialAnim(s32, s32, s32);
    s32 getPartialAnimSlotNum() const;
    s32 getJoitsAmountFromJoint(const char*) const;
    void addPartialAnimJoint(s32, const char*, const char*);
    void addPartialAnimJointRecursive(s32, const char*);
    void startPartialAnim(const char*, s32, s32, const SklAnimRetargettingInfo*);
    void clearPartialAnim(s32);
    void getPartialAnimInfo(s32);  // TODO: unknown return type
    bool isPartialAnimEnd(s32) const;
    nn::g3d::SkeletalAnimObj* getPartialAnimObj(s32) const;
    bool isPartialAnimOneTime(s32) const;
    bool isPartialAnimAttached(s32) const;
    void getPartialAnimInfo(s32) const;  // TODO: unknown return type
    const char* getPlayingPartialSklAnimName(s32) const;
    f32 getPartialAnimFrame(s32) const;
    void setPartialAnimFrame(s32, f32);
    nn::g3d::SkeletalAnimObj* getPartialAnimObj(s32);
    f32 getPartialAnimFrameRate(s32) const;
    void setPartialAnimFrameRate(s32, f32);

    void setRetargettingInfo(SklAnimRetargettingInfo* info) { mRetargettingInfo = info; }

private:
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

static_assert(sizeof(AnimPlayerSkl) == 0x90);

}  // namespace al
