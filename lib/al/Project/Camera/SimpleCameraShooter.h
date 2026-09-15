#pragma once

#include <math/seadVector.h>

#include "Library/Collision/CollisionPartsKeeperUtil.h"

namespace sead {
class LookAtCamera;
class PerspectiveProjection;
}  // namespace sead

namespace al {
class IUseCollision;

class SimpleCameraShooter {
public:
    SimpleCameraShooter(const char* _name);

    f32 calcAspectRatioByScreenSize(s32 width, s32 height);
    void calcCameraPos(sead::Vector3f* cameraPos, const IUseCollision* collision) const;
    void calcFrontDir(sead::Vector3f* frontDir) const;
    f32 checkCollision(f32 distance, const IUseCollision* collision) const;
    void applyCameraWithCollision(sead::LookAtCamera* camera, const IUseCollision* collision) const;
    void applyProjection(sead::PerspectiveProjection* projection) const;
    void lookAt(const sead::Vector3f& position);
    void applyLimitation();
    void rotateYaw(f32 degree);
    void rotatePitch(f32 degree);

private:
    sead::Vector3f mPosition = sead::Vector3f::zero;
    sead::Vector3f mSideDir = sead::Vector3f::ex;
    sead::Vector3f mUpDir = sead::Vector3f::ey;
    sead::Vector3f mBackDir = sead::Vector3f::ez;
    f32 mDistance = 100.0f;
    f32 mFovyDegree = 45.0f;
    f32 mNear = 100.0f;
    f32 mFar = 1000000.0f;
    f32 mAspectRatio = 1.0f;
    u32 _44;
    alCollisionUtil::SphereMoveHitInfo* mHitInfos = new alCollisionUtil::SphereMoveHitInfo[8];
};

static_assert(sizeof(SimpleCameraShooter) == 0x50);

}  // namespace al
