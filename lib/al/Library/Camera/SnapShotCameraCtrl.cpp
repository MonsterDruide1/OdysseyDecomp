#include "Library/Camera/SnapShotCameraCtrl.h"

#include "Library/Camera/CameraPoserFunction.h"
#include "Library/Camera/ICameraInput.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveExecutor.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Se/SeFunction.h"
#include "Library/Yaml/ByamlUtil.h"

#include "Library/Nerve/NerveSetupUtil.h"
#include "math/seadVectorFwd.h"

namespace {
using namespace al;
NERVE_IMPL(SnapShotCameraCtrl, Wait);
NERVE_IMPL(SnapShotCameraCtrl, Reset);

NERVES_MAKE_STRUCT(SnapShotCameraCtrl, Wait, Reset);
}  // namespace

namespace al {

// NON_MATCHING: also this is generating C2 when the original is C1
SnapShotCameraCtrl::SnapShotCameraCtrl(const SnapShotCameraSceneInfo* sceneInfo)
    : NerveExecutor("スナップショットモード中のカメラ制御"), mCameraSceneInfo(sceneInfo) {
    initNerve(&NrvSnapShotCameraCtrl.Wait, 0);

    CameraParam* param = new CameraParam();
    param->mHasMin = false;
    param->mHasMax = false;
    param->mMinFovyDegree = 40.0f;
    param->mMaxFovyDegree = 85.0f;
    mParam = param;
}

void SnapShotCameraCtrl::start(f32 fovy) {
    field_38 = sead::Vector3f(0.0f, 0.0f, 0.0f);
    mLookAtOffset = sead::Vector3f(0.0f, 0.0f, 0.0f);
    mFovyDegree = fovy;
    field_4C = fovy;
    field_50 = fovy;
    mRollDegree = 0.0f;
    mRollTarget = 0.0f;
}

void SnapShotCameraCtrl::load(const ByamlIter& iter) {
    CameraParam* param = mParam;
    ByamlIter paramIter;
    if (!tryGetByamlIterByKey(&paramIter, iter, "SnapShotParam"))
        return;
    if (tryGetByamlF32(&param->mMinFovyDegree, paramIter, "MinFovyDegree"))
        param->mHasMin = true;
    if (tryGetByamlF32(&param->mMaxFovyDegree, paramIter, "MaxFovyDegree"))
        param->mHasMax = true;
}

// NON_MATCHING
void SnapShotCameraCtrl::startReset(s32 unk) {
    field_64 = unk >= 0 ? unk : 15;
    setNerve(this, &NrvSnapShotCameraCtrl.Reset);
}

f32 minRoll = -90.0f;
f32 maxRoll = 90.0f;

void SnapShotCameraCtrl::update(const sead::LookAtCamera& camera, const IUseCollision* collision,
                                const ICameraInput* input) {
    updateNerve();
    if (!al::isNerve(this, &NrvSnapShotCameraCtrl.Wait))
        return;

    if (mIsValidZoomFovy) {
        f32 prevFovyDegree = mFovyDegree;
        f32 v11 = field_50;
        if (input->isHoldSnapShotZoomIn())
            v11 = field_50 - 3.0f;
        else if (input->isHoldSnapShotZoomOut())
            v11 = field_50 + 3.0f;

        f32 min = mParam->mHasMin ? mParam->mMinFovyDegree : 5.0f;
        f32 max = mParam->mHasMax              ? mParam->mMaxFovyDegree :
                  mMaxZoomOutFovyDegree > 0.0f ? mMaxZoomOutFovyDegree :
                                                 field_4C;

        f32 clamped = sead::Mathf::clamp(v11, min, max);
        field_50 = al::lerpValue(field_50, clamped, 0.3f);
        mFovyDegree = al::lerpValue(mFovyDegree, field_50, 0.3f);

        if (getAudioKeeper() && sead::Mathf::abs(mFovyDegree - prevFovyDegree) > 0.5f)
            al::holdSeWithParam(this, "Zoom", mFovyDegree, "");
    }

    if (mIsValidRoll) {
        f32 prevRollDegree = mRollDegree;
        f32 prevRollTarget = mRollTarget;
        f32 rollTarget = prevRollTarget;
        if (input->isHoldSnapShotRollLeft())
            rollTarget = mRollTarget - 3.0f;
        else if (input->isHoldSnapShotRollRight())
            rollTarget = mRollTarget + 3.0f;

        f32 v26 = sead::Mathf::clamp(rollTarget, minRoll, maxRoll);
        mRollTarget = al::lerpValue(mRollTarget, v26, 0.2f);
        mRollDegree = al::lerpValue(mRollDegree, mRollTarget, 0.15f);
        if (getAudioKeeper() && sead::Mathf::abs(mRollDegree - prevRollDegree) > 0.2f)
            al::holdSeWithParam(this, "Roll", mRollDegree, "");
    }

    if (mIsValidLookAtOffset) {
        sead::Vector3f offset = {0.0f, 0.0f, 0.0f};
        sead::Vector2f moveStick = sead::Vector2f(0.0f, 0.0f);
        if (input->tryCalcSnapShotMoveStick(&moveStick) && !al::isNearZero(moveStick, 0.001f)) {
            sead::Vector3f field38_before = field_38;
            sead::Vector3f forward = camera.getAt() - camera.getPos();
            al::normalize(&forward);
            sead::Vector3f up = camera.getUp();
            al::rotateVectorDegree(&up, up, forward, mRollDegree);
            al::normalize(&up);
            if (!al::isNearZero(moveStick.x, 0.001f)) {
                sead::Vector3f side;
                side.setCross(forward, up);
                offset = moveStick.x * side * 50.0f + sead::Vector3f{0.0f, 0.0f, 0.0f};
            }
            if (!al::isNearZero(moveStick.y, 0.001f))
                offset += moveStick.y * up * 50.0f;
            if (!al::isNearZero(offset, 0.001f)) {
                sead::Vector3f clampedOffset = mLookAtOffset + offset;
                al::clampV3f(&clampedOffset, sead::Vector3f(-500.0f, -500.0f, -500.0f),
                             sead::Vector3f(500.0f, 500.0f, 500.0f));
                al::lerpVec(&field_38, field_38, clampedOffset, 0.3f);
            }
            if (mCameraSceneInfo->field_8) {
                if (field_38.y < 0.0f) {
                    if (field_38.y + camera.getAt().y < mCameraSceneInfo->field_C) {
                        field_38.y = sead::Mathf::min(camera.getAt().y, mCameraSceneInfo->field_C) -
                                     camera.getAt().y;
                    }
                }
            }
            sead::Vector3f camAt = camera.getAt();
            sead::Vector3f otherAt = camAt + field38_before;
            sead::Vector3f newAt = camAt + field_38;
            alCameraPoserFunction::checkCameraCollisionMoveSphere(&newAt, collision, otherAt, newAt,
                                                                  75.0f);
            field_38 = newAt - camera.getAt();
        }
        al::lerpVec(&mLookAtOffset, mLookAtOffset, field_38, 0.3f);
    }
    if (unk5 && input->isTriggerReset())
        startReset(-1);
}

void SnapShotCameraCtrl::makeLookAtCameraPost(sead::LookAtCamera* camera) const {
    if (mIsValidLookAtOffset) {
        camera->getAt() = camera->getAt() + mLookAtOffset;
        camera->getPos() = camera->getPos() + mLookAtOffset;
    }
}

void SnapShotCameraCtrl::makeLookAtCameraLast(sead::LookAtCamera* camera) const {
    if (mIsValidRoll) {
        sead::Vector3f v13 = camera->getAt() - camera->getPos();
        al::normalize(&v13);

        sead::Vector3f up = camera->getUp();
        al::rotateVectorDegree(&up, up, v13, this->mRollDegree);
        al::normalize(&up);

        camera->getUp() = up;
        camera->getUp().normalize();
    }
}

void SnapShotCameraCtrl::exeWait() {}

// NON_MATCHING
void SnapShotCameraCtrl::exeReset() {
    if (al::isFirstStep(this)) {
        field_50 = mFovyDegree;
        mRollTarget = mRollDegree;
        field_38 = mLookAtOffset;
    }

    mFovyDegree = al::calcNerveValue(this, field_64, mFovyDegree, field_4C);
    f32 rate = al::calcNerveRate(this, field_64);
    rate = 1.0f - rate;
    mRollDegree = mRollTarget * rate;
    mLookAtOffset = rate * field_38;

    if (al::isGreaterEqualStep(this, field_64)) {
        field_64 = -1;
        start(field_4C);
        setNerve(this, &NrvSnapShotCameraCtrl.Wait);
    }
}

}  // namespace al
