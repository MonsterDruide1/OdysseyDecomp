#include "Area/In2DAreaMoveControl.h"

#include "Library/Math/MathUtil.h"

#include "Util/AreaUtil.h"

In2DAreaMoveControl::In2DAreaMoveControl() = default;

void In2DAreaMoveControl::update(const sead::FixedPtrArray<al::AreaObj, 8>& areas) {
    s32 size = areas.size();
    mAreas.clear();
    if (size > 0) {
        mLastAreas.clear();
        for (s32 i = 0; i < size; i++) {
            al::AreaObj* area = areas[i];
            mAreas.pushBack(area);
            mLastAreas.pushBack(area);
        }
    }
}

void In2DAreaMoveControl::calcGravityDir(sead::Vector3f* out, const sead::Vector3f& pos,
                                         const sead::Vector3f& defaultGravity) const {
    out->set(0.0f, 0.0f, 0.0f);
    s32 size = mAreas.size();
    for (s32 i = 0; i < size; i++) {
        sead::Vector3f gravity = {0.0f, 0.0f, 0.0f};
        if (rs::calc2DAreaDistanceGravity(&gravity, mAreas[i], pos))
            *out += gravity;
    }
    if (!al::tryNormalizeOrZero(out))
        out->set(defaultGravity);
}

void In2DAreaMoveControl::calcLockDir(sead::Vector3f* out, f32* closestSurfaceDist,
                                      const sead::Vector3f& pos) const {
    out->set(0.0f, 0.0f, 0.0f);
    *closestSurfaceDist = 100000.0f;
    s32 size = mAreas.size();
    for (s32 i = 0; i < size; i++) {
        al::AreaObj* area = mAreas[i];
        sead::Vector3f snapDir = {0.0f, 0.0f, 0.0f};
        f32 snapPower = 0.0f;
        rs::calc2DAreaSnapPower(&snapDir, &snapPower, area, pos);
        *out += snapDir;
        *closestSurfaceDist = sead::Mathf::min(*closestSurfaceDist, snapPower);
    }
    if (!al::tryNormalizeOrZero(out))
        out->set(sead::Vector3f::ey);
}

void In2DAreaMoveControl::calcLastLockDir(sead::Vector3f* out, const sead::Vector3f& pos) const {
    out->set(0.0f, 0.0f, 0.0f);
    s32 size = mLastAreas.size();
    for (s32 i = 0; i < size; i++) {
        sead::Vector3f lockDir = {0.0f, 0.0f, 0.0f};
        rs::calc2DAreaLockDir(&lockDir, mLastAreas[i], pos);
        *out += lockDir;
    }
    if (!al::tryNormalizeOrZero(out))
        out->set(sead::Vector3f::ey);
}

bool In2DAreaMoveControl::isNearSnapSurface(const sead::Vector3f& pos, f32 threshold) const {
    s32 size = mAreas.size();
    for (s32 i = 0; i < size; i++) {
        al::AreaObj* area = mAreas[i];
        f32 surfaceDistance = rs::get2DAreaSurfaceDistance(area);

        // Calculate snap direction with surface distance offset
        sead::Vector3f snapDirWithOffset = {0.0f, 0.0f, 0.0f};
        f32 snapDistWithOffset = 0.0f;
        rs::calc2DAreaSnapPowerSurfaceDistance(&snapDirWithOffset, &snapDistWithOffset, area, pos,
                                               surfaceDistance);

        // Calculate snap direction at the actual surface (no offset)
        sead::Vector3f snapDirAtSurface = {0.0f, 0.0f, 0.0f};
        f32 snapDistAtSurface = 0.0f;
        rs::calc2DAreaSnapPowerSurfaceDistance(&snapDirAtSurface, &snapDistAtSurface, area, pos,
                                               0.0f);

        // Check if directions point opposite ways or if close to surface
        if (snapDirWithOffset.dot(snapDirAtSurface) < 0.0f || snapDistWithOffset <= threshold)
            return true;
    }
    return false;
}

bool In2DAreaMoveControl::isLastAreaPushOutOpposite() const {
    s32 size = mLastAreas.size();
    for (s32 i = 0; i < size; i++)
        if (rs::isPushOut2DAreaOpposite(mLastAreas[i]))
            return true;
    return false;
}

bool In2DAreaMoveControl::isLastAreaFaceToPushOut() const {
    s32 size = mLastAreas.size();
    for (s32 i = 0; i < size; i++)
        if (rs::isFaceTo2DAreaPushOutDir(mLastAreas[i]))
            return true;
    return false;
}
