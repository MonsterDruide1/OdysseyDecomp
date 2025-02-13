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

    void initAnimResource(const Resource*, const sead::Matrix34f*);
    void start(const CameraStartInfo&);
    void update();
    void setAnim(const char* animName, s32, s32, s32);
    bool isExistAnim(const char* animName);
    bool isAnimPlaying(const char* animName);
    bool isAnimEnd();
    u32 calcStepMax(const char* animName);

private:
    Resource* mRes;
    nn::g3d::ResFile* mResFile;
    nn::g3d::CameraAnimResult* mAnimResult;
    f32 _158;
    f32 _15C;
    f32 _160;
    f32 _164;
    f32 _168;
    f32 _16C;
    f32 _170;
    f32 _174;
    f32 _178;
    f32 _17C;
    const char* mPlayingAnimName;
    f32 _188;
    f32 _18C;
    f32 _190;
    f32 _194;
    f32 _198;
    f32 _19C;
    const sead::Matrix34f* _1A0;
    f32 _1A8;
    bool mIsHoldZoom;
    bool _1AD;
};
}  // namespace al
