#include "Project/Camera/SimpleCameraShooter.h"

#include <gfx/seadCamera.h>
#include <gfx/seadProjection.h>

#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Math/MathUtil.h"

namespace al {

SimpleCameraShooter::SimpleCameraShooter(const char* _name) {
    mAspectRatio = 4.0f / 3.0f;
}

f32 SimpleCameraShooter::calcAspectRatioByScreenSize(s32 width, s32 height) {
    mAspectRatio = (f32)width / (f32)height;
    return mAspectRatio;
}

void SimpleCameraShooter::calcCameraPos(sead::Vector3f* cameraPos,
                                        const IUseCollision* collision) const {
    sead::Vector3f backDir = mBackDir;
    f32 distance = mDistance;
    if (collision) {
        if (alCollisionUtil::checkStrikeSphereMove(collision, mHitInfos, 8, mPosition, 30.0f,
                                                   distance * mBackDir, nullptr, nullptr))
            distance *= mHitInfos[0]._0;
    }

    cameraPos->set(mPosition);
    *cameraPos += backDir * distance;
}

void SimpleCameraShooter::calcFrontDir(sead::Vector3f* frontDir) const {
    *frontDir = -mBackDir;
}

f32 SimpleCameraShooter::checkCollision(f32 distance, const IUseCollision* collision) const {
    sead::Vector3f backDir = mBackDir;
    if (alCollisionUtil::checkStrikeSphereMove(collision, mHitInfos, 8, mPosition, 30.0f,
                                               backDir * distance, nullptr, nullptr))
        distance *= mHitInfos[0]._0;
    return distance;
}

void SimpleCameraShooter::applyCameraWithCollision(sead::LookAtCamera* camera,
                                                   const IUseCollision* collision) const {
    sead::Vector3f backDir = mBackDir;
    f32 distance = mDistance;
    if (collision)
        distance = checkCollision(distance, collision);

    sead::Vector3f cameraPos = mPosition;
    cameraPos = backDir * distance + cameraPos;
    camera->setPos(cameraPos);
    camera->setAt(mPosition - backDir);
    camera->setUp(mUpDir);
    camera->normalizeUp();
    camera->updateViewMatrix();
}

void SimpleCameraShooter::applyProjection(sead::PerspectiveProjection* projection) const {
    projection->set(mNear, mFar, sead::Mathf::deg2rad(mFovyDegree), mAspectRatio);
}

void SimpleCameraShooter::lookAt(const sead::Vector3f& position) {
    sead::Vector3f direction = position - mPosition;
    if (tryNormalizeOrZero(&direction)) {
        mBackDir = -direction;
        applyLimitation();
    }
}

void SimpleCameraShooter::applyLimitation() {
    sead::Vector3f upperLimit = sead::Vector3f::ey;
    sead::Vector3f lowerLimit = sead::Vector3f::ey;
    rotateVectorDegree(&upperLimit, upperLimit, mSideDir, 110.0f);
    rotateVectorDegree(&lowerLimit, lowerLimit, mSideDir, 70.0f);
    tryNormalizeOrZero(&upperLimit);
    tryNormalizeOrZero(&lowerLimit);

    const sead::Vector3f* limit = nullptr;
    f32 dot = upperLimit.dot(mBackDir);
    if (dot < 0.0f) {
        limit = &upperLimit;
    } else {
        dot = lowerLimit.dot(mBackDir);
        if (dot < 0.0f)
            limit = &lowerLimit;
    }

    if (limit) {
        sead::Vector3f offset = *limit;
        offset.multScalar(-dot);
        mBackDir += offset;
    }

    tryNormalizeOrZero(&mBackDir);
    if (isParallelDirection(mBackDir, sead::Vector3f::ey, 0.01f)) {
        mUpDir = mBackDir.cross(mSideDir);
        mSideDir = mUpDir.cross(mBackDir);
    } else {
        mSideDir = sead::Vector3f::ey.cross(mBackDir);
        mUpDir = mBackDir.cross(mSideDir);
    }
    tryNormalizeOrZero(&mSideDir);
    tryNormalizeOrZero(&mUpDir);
}

void SimpleCameraShooter::rotateYaw(f32 degree) {
    rotateVectorDegree(&mBackDir, mBackDir, mUpDir, sead::Mathf::deg2rad(degree));
    applyLimitation();
}

void SimpleCameraShooter::rotatePitch(f32 degree) {
    rotateVectorDegree(&mBackDir, mBackDir, mSideDir, sead::Mathf::deg2rad(degree));
    applyLimitation();
}

}  // namespace al
