#include "Library/Camera/CameraShaker.h"

#include "Library/Base/StringUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
using namespace al;

NERVE_IMPL(CameraShaker, Shake);
NERVE_IMPL(CameraShaker, Wait);
NERVE_IMPL(CameraShaker, ShakeLoop);

NERVES_MAKE_NOSTRUCT(CameraShaker, Shake);
NERVES_MAKE_STRUCT(CameraShaker, Wait, ShakeLoop);
}  // namespace

namespace al {

const CameraShaker::ShakeInfo WeakShakeLoop = {"弱", -1, 7.5f, 0.0007f,
                                               CameraShaker::ShakeDirection::Both};

const CameraShaker::ShakeInfo ShakeInfos[11] = {
    {"微弱", 15, 2.5f, 0.0015f, CameraShaker::ShakeDirection::Both},
    {"微弱[短]", 10, 2.0f, 0.0008f, CameraShaker::ShakeDirection::Both},
    {"弱", 25, 2.5f, 0.0025f, CameraShaker::ShakeDirection::Both},
    {"中", 25, 2.5f, 0.004f, CameraShaker::ShakeDirection::Both},
    {"強", 30, 3.0f, 0.008f, CameraShaker::ShakeDirection::Both},
    {"最強", 45, 3.5f, 0.015f, CameraShaker::ShakeDirection::Both},
    {"超最強", 45, 3.5f, 0.05f, CameraShaker::ShakeDirection::Both},
    {"長い微弱", 60, 6.0f, 0.0025f, CameraShaker::ShakeDirection::Both},
    {"長い弱", 60, 6.0f, 0.004f, CameraShaker::ShakeDirection::Both},
    {"船内振動", 100, 6.0f, 0.0005f, CameraShaker::ShakeDirection::Both},
    {"弱[縦]", 25, 2.5f, 0.004f, CameraShaker::ShakeDirection::Vertical},
};

CameraShaker::CameraShaker() : NerveExecutor("カメラ振動") {
    initNerve(&NrvCameraShaker.Wait, 0);
    mEditedShake = {"NULL", 0, 0.0f, 0.0f, ShakeDirection::Both};
}

void CameraShaker::update(const char* shakeLoop) {
    if (shakeLoop) {
        mShakeLoop = isEqualString(shakeLoop, "弱") ? &WeakShakeLoop : nullptr;
        if (isNerve(this, &NrvCameraShaker.Wait))
            setNerve(this, &NrvCameraShaker.ShakeLoop);
    } else {
        mShakeLoop = nullptr;
        if (isNerve(this, &NrvCameraShaker.ShakeLoop))
            setNerve(this, &NrvCameraShaker.Wait);
    }

    updateNerve();
}

void CameraShaker::startShakeByAction(const char* name, const char* unused1, const char* unused2,
                                      s32 steps) {
    startShakeByName(name, steps);
}

void CameraShaker::startShakeByName(const char* name, s32 steps) {
    s32 index = -1;
    if (isEqualString(name, "微弱"))
        index = 0;
    else if (isEqualString(name, "微弱[短]"))
        index = 1;
    else if (isEqualString(name, "弱"))
        index = 2;
    else if (isEqualString(name, "中"))
        index = 3;
    else if (isEqualString(name, "強"))
        index = 4;
    else if (isEqualString(name, "最強"))
        index = 5;
    else if (isEqualString(name, "超最強"))
        index = 6;
    else if (isEqualString(name, "長い微弱"))
        index = 7;
    else if (isEqualString(name, "長い弱"))
        index = 8;
    else if (isEqualString(name, "船内振動"))
        index = 9;
    else if (isEqualString(name, "弱[縦]"))
        index = 10;

    startShakeByIndex(index, steps);
}

void CameraShaker::startShakeByHitReaction(const char* name, const char* unused1,
                                           const char* unused2, s32 steps) {
    startShakeByName(name, steps);
}

void CameraShaker::exeWait() {
    if (isFirstStep(this)) {
        mActiveShake = nullptr;
        mShakeLoop = nullptr;
    }
    mOffset = {0.0f, 0.0f};
}

void CameraShaker::exeShake() {
    if (isGreaterEqualStep(this, mActiveShake->mSteps)) {
        if (mShakeLoop) {
            setNerve(this, &NrvCameraShaker.ShakeLoop);
        } else {
            mOffset = {0.0f, 0.0f};
            mActiveShake = nullptr;
            setNerve(this, &NrvCameraShaker.Wait);
        }
        return;
    }

    f32 shakeSpeed = (mActiveShake->mSpeed * 360.0f) / mActiveShake->mSteps;
    f32 currentShakeStrength =
        sead::Mathf::cos(sead::Mathf::deg2rad(shakeSpeed * getNerveStep(this)));
    f32 shakeOffset =
        currentShakeStrength * (mActiveShake->mStrength *
                                (mActiveShake->mSteps - getNerveStep(this)) / mActiveShake->mSteps);
    mOffset = {shakeOffset, shakeOffset};
    if (mActiveShake->mDirection == ShakeDirection::Vertical)
        mOffset.x = 0.0f;
}

void CameraShaker::exeShakeLoop() {
    s32 nerveStep = getNerveStep(this);
    f32 shakeStep = nerveStep <= 0 ? 0.0f : nerveStep / mShakeLoop->mSpeed * sead::Mathf::pi2();
    f32 shakeOffset = mShakeLoop->mStrength * sead::Mathf::cos(shakeStep);
    mOffset = {shakeOffset, shakeOffset};
    if (mShakeLoop->mDirection == ShakeDirection::Vertical)
        mOffset.x = 0.0f;
}

void CameraShaker::startShakeByIndex(s32 index, s32 steps) {
    const ShakeInfo& shake = ShakeInfos[index];
    if (mActiveShake && (*mActiveShake > shake))
        return;

    mActiveShake = &shake;

    if (steps >= 1) {
        ShakeInfo shake2 = shake;
        mEditedShake = {shake.mName, steps, shake.mSpeed, shake.mStrength, shake.mDirection};
        mActiveShake = &mEditedShake;
        mEditedShake.mSpeed = ((f32)steps / (f32)shake2.mSteps) * shake.mSpeed;
    }

    setNerve(this, &Shake);
}

}  // namespace al
