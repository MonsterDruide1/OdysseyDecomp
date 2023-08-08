#include "al/Library/Rail/RailPart.h"

#include "al/Project/Rail/BezierCurve.h"
#include "al/Project/Rail/LinearCurve.h"
#include "al/Library/Math/MathUtil.h"
#include "al/Library/Math/VectorUtil.h"
#include "math/seadVector.h"

namespace al {

RailPart::RailPart() = default;
void RailPart::init(const sead::Vector3f& start, const sead::Vector3f& startHandle,
                    const sead::Vector3f& endHandle, const sead::Vector3f& end) {
    sead::Vector3f startDiff = start - startHandle;
    sead::Vector3f endDiff = end - endHandle;
    if (startDiff.equals({0, 0, 0}, 0.1) && endDiff.equals({0, 0, 0}, 0.1)) {
        mLinearCurve = new LinearCurve();
        mLinearCurve->set(start, end);
    } else {
        mBezierCurve = new BezierCurve();
        mBezierCurve->set(start, startHandle, endHandle, end);
    }
}
void RailPart::calcPos(sead::Vector3f* pos, float param) const {
    return mBezierCurve ? mBezierCurve->calcPos(pos, param) : mLinearCurve->calcPos(pos, param);
}
void RailPart::calcVelocity(sead::Vector3f* vel, float param) const {
    return mBezierCurve ? mBezierCurve->calcVelocity(vel, param) :
                          mLinearCurve->calcVelocity(vel, param);
}
void RailPart::calcDir(sead::Vector3f* dir, float param) const {
    calcVelocity(dir, param);
    if(!isNearZero(*dir, 0.001)) {
        normalize(dir);
        return;
    }

    sead::Vector3f startPos;
    calcStartPos(&startPos);
    sead::Vector3f endPos;
    calcEndPos(&endPos);

    dir->x = endPos.x-startPos.x;
    dir->y = endPos.y-startPos.y;
    dir->z = endPos.z-startPos.z;
    if(isNearZero(*dir, 0.001)) {
        *dir = {0,0,1};
    } else {
        normalize(dir);
    }
}
void RailPart::calcStartPos(sead::Vector3f* pos) const {
    return mBezierCurve ? mBezierCurve->calcStartPos(pos) : mLinearCurve->calcStartPos(pos);
}
void RailPart::calcEndPos(sead::Vector3f* pos) const {
    return mBezierCurve ? mBezierCurve->calcEndPos(pos) : mLinearCurve->calcEndPos(pos);
}
float RailPart::calcLength(float startParam, float endParam, int stepCount) const {
    return mBezierCurve ? mBezierCurve->calcLength(startParam, endParam, stepCount) :
                          mLinearCurve->calcLength(startParam, endParam);
}
float RailPart::calcCurveParam(float param) const {
    return mBezierCurve ? mBezierCurve->calcCurveParam(param) : mLinearCurve->calcCurveParam(param);
}
float RailPart::calcNearestParam(const sead::Vector3f& pos, float interval) const {
    return mBezierCurve ? mBezierCurve->calcNearestParam(pos, interval) :
                          mLinearCurve->calcNearestParam(pos);
}
void RailPart::calcNearestPos(sead::Vector3f* nearest, const sead::Vector3f& pos,
                              float interval) const {
    return mBezierCurve ? mBezierCurve->calcNearestPos(nearest, pos, interval) :
                          mLinearCurve->calcNearestPos(nearest, pos);
}
float RailPart::calcNearestLength(float* param, const sead::Vector3f& pos, float max,
                                  float interval) const {
    return mBezierCurve ? mBezierCurve->calcNearestLength(param, pos, max, interval) :
                          mLinearCurve->calcNearestLength(param, pos, max);
}
float RailPart::getPartLength() const {
    return mBezierCurve ? mBezierCurve->getLength() : mLinearCurve->getLength();
}

}  // namespace al
