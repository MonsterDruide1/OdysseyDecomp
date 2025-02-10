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
    field_64 = unk < 0 ? 15 : unk;
    setNerve(this, &NrvSnapShotCameraCtrl.Reset);
}

void SnapShotCameraCtrl::update(const sead::LookAtCamera& camera, const IUseCollision* collision,
                                const ICameraInput* input) {
    updateNerve();
    if (!al::isNerve(this, &NrvSnapShotCameraCtrl.Wait))
        return;

    if (mIsValidZoomFovy) {
        f32 prevFovyDegree = mFovyDegree;
        f32 v11 = field_50;
        if (input->isHoldSnapShotZoomIn())
            v11 -= 3.0f;
        else if (input->isHoldSnapShotZoomOut())
            v11 += 3.0f;

        f32 min = mParam->mHasMin ? mParam->mMinFovyDegree : 5.0f;
        f32 max = mParam->mHasMax               ? mParam->mMaxFovyDegree :
                  mMaxZoomOutFovyDegree >= 0.0f ? mMaxZoomOutFovyDegree :
                                                  field_4C;

        field_50 = al::lerpValue(v11, sead::Mathf::clamp(v11, min, max), 0.3f);
        mFovyDegree = al::lerpValue(mFovyDegree, field_50, 0.3f);

        if (getAudioKeeper()) {
            f32 v21 = mFovyDegree - prevFovyDegree;
            if (mFovyDegree - prevFovyDegree <= 0.0f)
                v21 = -(mFovyDegree - prevFovyDegree);
            if (v21 > 0.5f)
                al::holdSeWithParam(this, "Zoom", mFovyDegree, nullptr);
        }
    }

    if (mIsValidRoll) {
        f32 prevRollDegree = mRollDegree;
        f32 prevRollTarget = mRollTarget;
        f32 add;
        if (input->isHoldSnapShotRollLeft())
            add = -3.0f;
        else if (input->isHoldSnapShotRollRight())
            add = 3.0f;
        prevRollTarget = prevRollTarget + add;

        f32 v26 = -90.0f;
        if (prevRollTarget >= -90.0f) {
            v26 = prevRollTarget;
            if (prevRollTarget > 90.0f)
                v26 = 90.0f;
        }
        f32 v27 = al::lerpValue(mRollTarget, v26, 0.2f);
        mRollTarget = v27;
        f32 v29 = al::lerpValue(mRollDegree, v27, 0.15f);
        mRollDegree = v29;
        if (getAudioKeeper()) {
            f32 v21 = mRollDegree - prevRollDegree;
            if (mRollDegree - prevRollDegree <= 0.0f)
                v21 = -(mRollDegree - prevRollDegree);
            if (v21 > 0.2f)
                al::holdSeWithParam(this, "Roll", mRollDegree, nullptr);
        }
    }

    if (mIsValidLookAtOffset) {
        sead::Vector2f moveStick = sead::Vector2f(0.0f, 0.0f);
        if (input->tryCalcSnapShotMoveStick(&moveStick) && !al::isNearZero(moveStick, 0.001f)) {
            sead::Vector3f forward = camera.getAt() - camera.getPos();
            al::normalize(&forward);
            sead::Vector3f up = camera.getUp();
            al::rotateVectorDegree(&up, up, forward, mRollDegree);
            al::normalize(&up);
            sead::Vector3f v38;
            sead::Vector3f offset;
            if (al::isNearZero(moveStick.x, 0.001f)) {
                v38 = sead::Vector3f(0.0f, 0.0f, 0.0f);
            } else {
                v38.z =
                    (float)((float)((float)((float)(forward.y * up.z) - (float)(forward.z * up.y)) *
                                    moveStick.x) *
                            50.0f) +
                    0.0f;
                v38.y = (float)((float)(moveStick.x * (float)((float)(forward.z * up.x) -
                                                              (float)(up.z * forward.x))) *
                                50.0f) +
                        0.0f;
                v38.x = (float)((float)(moveStick.x * (float)((float)(up.y * forward.x) -
                                                              (float)(forward.y * up.x))) *
                                50.0f) +
                        0.0f;
                offset = v38;
            }
            if (!al::isNearZero(moveStick.y, 0.001f))
                offset = moveStick.y * up * 50.0f + v38;
            if (!al::isNearZero(offset, 0.001f)) {
                sead::Vector3f clampedOffset = mLookAtOffset + offset;
                al::clampV3f(&clampedOffset, sead::Vector3f(-500.0f, -500.0f, -500.0f),
                             sead::Vector3f(500.0f, 500.0f, 500.0f));
                al::lerpVec(&field_38, field_38, clampedOffset, 0.3f);
            }
            if (mCameraSceneInfo->field_8) {
                if (field_38.y < 0.0f) {
                    f32 v46 = camera.getAt().y;
                    f32 v47 = mCameraSceneInfo->field_C;
                    if (field_38.y + camera.getAt().y < v47) {
                        f32 v48;
                        if (v47 >= v46)
                            v48 = camera.getAt().y;
                        else
                            v48 = mCameraSceneInfo->field_C;
                        field_38.y = v48 - v46;
                    }
                }
            }
            sead::Vector3f newAt = camera.getAt() + field_38;
            alCameraPoserFunction::checkCameraCollisionMoveSphere(&newAt, collision, newAt, newAt,
                                                                  75.0f);
            field_38 = newAt - camera.getAt();
        }
        al::lerpVec(&mLookAtOffset, mLookAtOffset, field_38, 0.3f);
    }
    if (unk5 && input->isTriggerReset()) {  // startReset()?
        field_64 = 15;
        al::setNerve(this, &NrvSnapShotCameraCtrl.Reset);
    }
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
