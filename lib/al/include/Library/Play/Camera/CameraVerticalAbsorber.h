#pragma once

#include <gfx/seadCamera.h>
#include <gfx/seadProjection.h>

#include "Library/Camera/CameraPoserFunction.h"
#include "Library/Nerve/Nerve.h"
#include "Library/Nerve/NerveExecutor.h"
#include "Library/Nerve/NerveKeeper.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Yaml/ByamlIter.h"

namespace al {
class CameraPoser;
class CameraStartInfo;

class CameraVerticalAbsorber : public NerveExecutor {
public:
    CameraVerticalAbsorber(const CameraPoser* cameraParent, bool isCameraPosAbsorb);
    ~CameraVerticalAbsorber();
    void exeAbsorb();
    void exeFollow();
    void exeFollowAbsolute();
    void exeFollowClimbPole();
    void exeFollowGround();
    void invalidate();
    bool isAbsorbing() const;
    bool isValid() const;
    void liverateAbsorb();
    void load(const ByamlIter&);
    void makeLookAtCamera(sead::LookAtCamera*) const;
    void start(const sead::Vector3f&, const CameraStartInfo&);
    void tryResetAbsorbVecIfInCollision(const sead::Vector3f&);
    void update();

private:
    const CameraPoser* mCameraPoser;
    sead::LookAtCamera mLookAtCamera;
    sead::PerspectiveProjection mProjection;
    sead::Vector3f mTargetInterp;
    float mLerp1;
    float mAbsorbScreenPosUp;
    float mAbsorbScreenPosDown;
    bool mAdvanceAbsorbUp;
    float mAdvanceAbsorbScreenPosUp;
    bool isExistCollisionUnderTarget;
    sead::Vector3f mUnderTargetCollisionPos;
    sead::Vector3f mUnderTargetCollisionNormal;
    float mLerp2;
    float mKeepInFrameOffsetUp;
    float mKeepInFrameOffsetDown;
    float mHighJumpJudgeSpeedV;
    sead::Vector3f mPrevTargetTrans;
    sead::Vector3f mTargetFront;
    sead::Vector3f mPrevTargetFront;
    bool isNoCameraPosAbsorb;
    bool isInvalidated;
    bool unusedBool;
    bool isStopUpdate;
    bool isKeepInFrame;
};
static_assert(sizeof(CameraVerticalAbsorber) == 0x1B0);

}  // namespace al
