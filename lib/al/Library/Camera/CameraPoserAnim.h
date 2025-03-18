#pragma once

#include <math/seadMatrix.h>

#include "Library/Camera/CameraPoser.h"

namespace nn::g3d {
class ResFile;
class CameraAnimResult;
}  // namespace nn::g3d

namespace al {

class CameraStartInfo;
class CameraPoser;
class Resource;

class CameraPoserAnim : public CameraPoser {
public:
    CameraPoserAnim();

    void start(const CameraStartInfo& startInfo) override;
    void update() override;
    void initAnimResource(const Resource* resource, const sead::Matrix34f* mtx);
    void setAnim(const char* animName, s32, s32, s32);
    bool isExistAnim(const char* animName) const;
    bool isAnimPlaying(const char* animName) const;
    bool isAnimEnd() const;
    u32 calcStepMax(const char* animName) const;

private:
    Resource* mRes;
    nn::g3d::ResFile* mResFile;
    nn::g3d::CameraAnimResult* mAnimResult;
    f32 _158;
    f32 _15c;
    f32 _160;
    f32 _164;
    f32 _168;
    f32 _16c;
    f32 _170;
    f32 _174;
    f32 _178;
    f32 _17c;
    const char* mPlayingAnimName;
    f32 _188;
    f32 _18c;
    f32 _190;
    f32 _194;
    f32 _198;
    f32 _19c;
    const sead::Matrix34f* _1a0;
    f32 _1a8;
    bool mIsHoldZoom;
    bool _1ad;
};

static_assert(sizeof(CameraPoserAnim) == 0x1B0);

}  // namespace al
