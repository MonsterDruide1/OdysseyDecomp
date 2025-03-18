#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class CameraPoser;
class PlacementInfo;
class Rail;
class RailRider;

class CameraLimitRailKeeper {
public:
    CameraLimitRailKeeper();

    void init(const PlacementInfo& info, s32 viewCount);
    void updateRider(const CameraPoser* poser);
    RailRider* getRider(const CameraPoser* poser) const;
    void calcCameraDirH(sead::Vector3f* out, const CameraPoser* poser) const;
    bool isNearInsideRailPoint(const CameraPoser* poser, f32 approx) const;
    const sead::Vector3f& getRailPos(const CameraPoser* poser) const;
    f32 calcDistanceFromNearestRailPos(const sead::Vector3f& pos) const;
    void calcNearestRailPos(sead::Vector3f* out, const sead::Vector3f& pos) const;

private:
    Rail* mRail = nullptr;
    RailRider** mRiders = nullptr;
    s32 mRiderCount = 0;
    s32 mCameraLookAtDir = 0;
    f32 mActivateDistance = 1000.0f;
    f32 mDegreeMargin = 40.0f;
    f32 mAngleElevation = 20.0f;
    f32 mAngleElevation2 = 20.0f;
    s32 mAngleElevationIterpStep = 60;
    s32 mAngleElevationResetStep = 60;
    f32 mCameraDistance = 2000.0f;
    f32 mCameraDistanceMin = 250.0f;
    f32 mCameraDistanceMax = 4000.0f;
    f32 mOffsetY = 120.0f;
    bool mIsApplyAngleElevation = false;
    bool mIsResetAngleElevation = false;
    bool mIsFixedAngle = false;
    bool mIsApplyCameraDistance = false;
    bool mIsApplyCameraDistanceRange = false;
    bool mIsApplyOffsetY = false;
    bool mIsInvalidCheckCollision = false;
};

static_assert(sizeof(CameraLimitRailKeeper) == 0x48);
}  // namespace al
