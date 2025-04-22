#pragma once

#include <basis/seadTypes.h>
#include <math/seadBoundBox.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
enum class Axis : s32;
class PlacementInfo;

class RollingCubePose {
public:
    RollingCubePose();

    bool isMovementRotate() const;
    bool isMovementSlide() const;
    void setCubeSize(const sead::BoundBox3f& cubeSize);
    void calcNearPose(sead::Quatf* nearPose, const sead::Quatf& quat) const;
    void fittingToBoundingBox(sead::Quatf* quat, sead::Vector3f* trans) const;
    void calcBoundingBoxCenter(sead::Vector3f* center) const;
    void calcBoundingBoxCenter(sead::Vector3f* center, const sead::Quatf& quat,
                               const sead::Vector3f& trans) const;
    void setNextCubePose(const RollingCubePose* nextPose);
    void calcBottomFacePoint(sead::Vector3f* facePoints) const;
    void init(const PlacementInfo& placementInfo);
    void calcRotateQT(sead::Quatf* outQuat, sead::Vector3f* outTrans, const sead::Quatf& quat,
                      const sead::Vector3f& trans, f32 angle) const;
    Axis calcBottomFaceIndex() const;

    const PlacementInfo& getPlacementInfo() const { return *mPlacementInfo; }

    const sead::Quatf& getQuat() const { return mQuat; }

    const sead::Vector3f& getTrans() const { return mTrans; }

    const sead::BoundBox3f& getCubeSize() const { return mCubeSize; }

    const sead::Vector3f& getRotateAxis() const { return mRotateAxis; }

    const sead::Vector3f& getRotateCenter() const { return mRotateCenter; }

    f32 getRotateDegree() const { return mRotateDegree; }

    const sead::Vector3f& getSlideVec() const { return mSlideVec; }

private:
    enum class MovementType { None, Rotate, Slide };

    PlacementInfo* mPlacementInfo = nullptr;
    sead::Quatf mQuat = sead::Quatf::unit;
    sead::Vector3f mTrans = sead::Vector3f::zero;
    sead::BoundBox3f mCubeSize;
    sead::Vector3f mRotateAxis = sead::Vector3f::zero;
    sead::Vector3f mRotateCenter = sead::Vector3f::zero;
    f32 mRotateDegree = 0.0f;
    sead::Vector3f mSlideVec = sead::Vector3f::zero;
    MovementType mMovementType = MovementType::Rotate;
};

}  // namespace al
