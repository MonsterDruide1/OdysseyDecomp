#pragma once

#include <gfx/seadCamera.h>
#include <gfx/seadProjection.h>

#include "Library/Nerve/NerveExecutor.h"

namespace al {
class ByamlIter;
class CameraPoser;
struct CameraStartInfo;

class CameraVerticalAbsorber : public NerveExecutor {
public:
    CameraVerticalAbsorber(const CameraPoser* cameraParent, bool isCameraPosAbsorb);

    void load(const ByamlIter& iter);
    void start(const sead::Vector3f& pos, const CameraStartInfo& info);
    bool isValid() const;
    void update();
    void makeLookAtCamera(sead::LookAtCamera* lookAtCamera) const;
    void liberateAbsorb();
    bool isAbsorbing() const;
    void invalidate();
    void tryResetAbsorbVecIfInCollision(const sead::Vector3f& pos);

    void exeAbsorb();
    void exeFollow();
    void exeFollowGround();
    void exeFollowClimbPole();
    void exeFollowAbsolute();

private:
    const CameraPoser* mCameraPoser;
    sead::LookAtCamera mLookAtCamera;
    sead::PerspectiveProjection mProjection;
    sead::Vector3f mTargetInterp = {0.0f, 0.0f, 0.0f};
    f32 mLerp1 = 0.0f;
    f32 mAbsorbScreenPosUp = -200.0f;
    f32 mAbsorbScreenPosDown = 480.0f;
    bool mIsAdvanceAbsorbUp = false;
    f32 mAdvanceAbsorbScreenPosUp = 0.0f;
    bool mIsExistCollisionUnderTarget = false;
    sead::Vector3f mUnderTargetCollisionPos = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mUnderTargetCollisionNormal = {0.0f, 0.0f, 0.0f};
    f32 mLerp2 = 0.0f;
    f32 mKeepInFrameOffsetUp = 0.0f;
    f32 mKeepInFrameOffsetDown = 0.0f;
    f32 mHighJumpJudgeSpeedV = 35.0f;
    sead::Vector3f mPrevTargetTrans = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mTargetFront = sead::Vector3f::ez;
    sead::Vector3f mPrevTargetFront = sead::Vector3f::ez;
    bool mIsNoCameraPosAbsorb = false;
    bool mIsInvalidated = false;
    bool _1aa = false;
    bool mIsStopUpdate = false;
    bool mIsKeepInFrame = false;
};

static_assert(sizeof(CameraVerticalAbsorber) == 0x1B0);

}  // namespace al
