#include "Project/RollingCubePose.h"

#include "Library/Math/Axis.h"
#include "Library/Math/MathUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"

namespace al {

RollingCubePose::RollingCubePose() {}

bool RollingCubePose::isMovementRotate() const {
    return mMovementType == MovementType::Rotate;
}

bool RollingCubePose::isMovementSlide() const {
    return mMovementType == MovementType::Slide;
}

void RollingCubePose::setCubeSize(const sead::BoundBox3f& cubeSize) {
    mCubeSize = cubeSize;
}

void RollingCubePose::calcNearPose(sead::Quatf* nearPose, const sead::Quatf& quat) const {
    calcFittingBoxPose(nearPose, mCubeSize, quat, mQuat);
}

void RollingCubePose::fittingToBoundingBox(sead::Quatf* quat, sead::Vector3f* trans) const {
    calcNearPose(quat, *quat);

    sead::Vector3f currentCenter;
    calcBoundingBoxCenter(&currentCenter);

    sead::Vector3f boxCenter;
    calcBoundingBoxCenter(&boxCenter, *quat, *trans);

    *trans += currentCenter - boxCenter;
}

void RollingCubePose::calcBoundingBoxCenter(sead::Vector3f* center) const {
    calcBoundingBoxCenter(center, mQuat, mTrans);
}

void RollingCubePose::calcBoundingBoxCenter(sead::Vector3f* center, const sead::Quatf& quat,
                                            const sead::Vector3f& trans) const {
    center->set(mCubeSize.getCenter());

    sead::Matrix34f mtx;
    mtx.makeQT(quat, trans);

    center->mul(mtx);
}

void RollingCubePose::setNextCubePose(const RollingCubePose* nextPose) {
    mSlideVec = sead::Vector3f::zero;

    sead::Vector3f facePoints[4];
    calcBottomFacePoint(facePoints);

    sead::Vector3f nextFacePoints[4];
    nextPose->calcBottomFacePoint(nextFacePoints);

    s32 pointCount = 0;
    sead::Vector3f firstPoint = sead::Vector3f::zero;
    sead::Vector3f secondPoint = sead::Vector3f::zero;

    for (s32 i = 0; i < 4; i++) {
        for (s32 e = 0; e < 4; e++) {
            if ((facePoints[i] - nextFacePoints[e]).length() <= 10.0f) {
                if (pointCount == 0) {
                    pointCount = 1;
                    firstPoint.set(nextFacePoints[e]);
                    continue;
                }
                if (pointCount == 1) {
                    pointCount = 2;
                    secondPoint.set(nextFacePoints[e]);
                    continue;
                }
                pointCount++;
            }
        }
    }

    mMovementType = MovementType::None;
    if (pointCount == 2) {
        mRotateAxis = secondPoint - firstPoint;
        mRotateCenter = firstPoint;
        if (tryNormalizeOrZero(&mRotateAxis)) {
            sead::Vector3f currentCenter;
            calcBoundingBoxCenter(&currentCenter);

            sead::Vector3f nextCenter;
            nextPose->calcBoundingBoxCenter(&nextCenter);

            mRotateDegree = calcAngleOnPlaneDegree(currentCenter - mRotateCenter,
                                                   nextCenter - mRotateCenter, mRotateAxis);
            mMovementType = MovementType::Rotate;
            return;
        }
        if (mMovementType != MovementType::None)
            return;
    }

    mMovementType = MovementType::Slide;

    sead::Vector3f currentCenter;
    calcBoundingBoxCenter(&currentCenter);

    sead::Vector3f nextCenter;
    nextPose->calcBoundingBoxCenter(&nextCenter);

    sead::Quatf nearPose;
    nextPose->calcNearPose(&nearPose, mQuat);
    calcQuatRotateAxisAndDegree(&mRotateAxis, &mRotateDegree, mQuat, nearPose);

    mSlideVec.x = nextCenter.x - currentCenter.x;
    mSlideVec.y = nextCenter.y - currentCenter.y;
    mRotateCenter = currentCenter;
    mSlideVec.z = nextCenter.z - currentCenter.z;
}

void RollingCubePose::calcBottomFacePoint(sead::Vector3f* facePoints) const {
    calcBoxFacePoint(facePoints, mCubeSize, calcBottomFaceIndex(), mQuat, mTrans);
}

void RollingCubePose::init(const PlacementInfo& placementInfo) {
    tryGetQuat(&mQuat, placementInfo);
    tryGetTrans(&mTrans, placementInfo);
    mPlacementInfo = new PlacementInfo(placementInfo);
}

void RollingCubePose::calcRotateQT(sead::Quatf* outQuat, sead::Vector3f* outTrans,
                                   const sead::Quatf& quat, const sead::Vector3f& trans,
                                   f32 angle) const {
    rotateQuatAndTransDegree(outQuat, outTrans, quat, trans, mRotateAxis, mRotateCenter,
                             mRotateDegree * angle);
    outTrans->setScaleAdd(angle, mSlideVec, *outTrans);
}

Axis RollingCubePose::calcBottomFaceIndex() const {
    sead::Vector3f xAxis;
    sead::Vector3f yAxis;
    sead::Vector3f zAxis;
    calcQuatLocalAxisAll(mQuat, &xAxis, &yAxis, &zAxis);
    f32 x = sead::Mathf::abs(xAxis.y);
    f32 y = sead::Mathf::abs(yAxis.y);
    f32 z = sead::Mathf::abs(zAxis.y);

    if (x > y) {
        if (x > z)
            return 0.0f > xAxis.y ? Axis::X : Axis::InvertX;
        return 0.0f > zAxis.y ? Axis::Z : Axis::InvertZ;
    }

    if (y >= z)
        return 0.0f > yAxis.y ? Axis::Y : Axis::InvertY;
    return 0.0f > zAxis.y ? Axis::Z : Axis::InvertZ;
}

}  // namespace al
