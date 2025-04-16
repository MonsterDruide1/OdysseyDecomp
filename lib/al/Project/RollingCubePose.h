#pragma once

#include <math/seadBoundBox.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class PlacementInfo;

class RollingCubePose {
public:
    RollingCubePose();

    bool isMovementRotate() const;
    bool isMovementSlide() const;
    void setCubeSize(const sead::BoundBox3f&);
    void calcNearPose(sead::Quatf*, const sead::Quatf&) const;
    void fittingToBoundingBox(sead::Quatf*, sead::Vector3f*) const;
    void calcBoundingBoxCenter(sead::Vector3f*) const;
    void calcBoundingBoxCenter(sead::Vector3f*, const sead::Quatf&, const sead::Vector3f&) const;
    void setNextCubePose(const RollingCubePose*);
    void calcBottomFacePoint(sead::Vector3f*) const;
    void init(const PlacementInfo&);
    void calcRotateQT(sead::Quatf*, sead::Vector3f*, const sead::Quatf&, const sead::Vector3f&,
                      f32) const;
    void calcBottomFaceIndex() const;

    const PlacementInfo& getPlacementInfo() const { return mPlacementInfo; }

    const sead::Quatf& getQuat() const { return mQuat; }

    const sead::Vector3f& getTrans() const { return mTrans; }

    const sead::Vector3f& getFront() const { return mFront; }

    const sead::Vector3f& getPos() const { return mPos; }

    f32 getRotateDegree() const { return mRotateDegree; }

    const sead::Vector3f& getSlideVec() const { return mSlideVec; }

private:
    const PlacementInfo& mPlacementInfo;
    sead::Quatf mQuat;
    sead::Vector3f mTrans;
    sead::BoundBox3f mCubeSize;
    sead::Vector3f mFront;
    sead::Vector3f mPos;
    f32 mRotateDegree;
    sead::Vector3f mSlideVec;
};

}  // namespace al
