#include "Enemy/GotogotonEdge.h"

#include <math/seadMatrix.h>

GotogotonEdge::GotogotonEdge() {}

void GotogotonEdge::setTrans(const sead::Vector3f& trans) {
    mTrans = trans;
}

void GotogotonEdge::setAxis(const sead::Vector3f& axis) {
    mAxis = axis;
}

void GotogotonEdge::setNextFace(const GotogotonFace* face) {
    mNextFace = face;
}

void GotogotonEdge::calcAxisAndPos(sead::Vector3f* outAxis, sead::Vector3f* outPos,
                                   const sead::Quatf& quat, const sead::Vector3f& trans) const {
    sead::Matrix34f mtx;
    mtx.makeQT(quat, trans);

    if (outPos)
        outPos->setMul(mtx, mTrans);

    if (outAxis)
        outAxis->setRotated(quat, mAxis);
}
