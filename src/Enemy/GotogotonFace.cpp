#include "Enemy/GotogotonFace.h"

#include "Library/Math/MathUtil.h"

GotogotonFace::GotogotonFace() {}

GotogotonEdge* GotogotonFace::getEdge(s32 index) {
    return &mEdges[index];
}

const GotogotonEdge* GotogotonFace::getEdge(s32 index) const {
    return &mEdges[index];
}

const GotogotonFace* GotogotonFace::getNextFace(s32 index) const {
    return mEdges[index].mNextFace;
}

void GotogotonFace::setOppositeFace(const GotogotonFace* face) {
    mOppositeFace = face;
}

void GotogotonFace::setCenter(const sead::Vector3f& vector) {
    mCenter = vector;
}

void GotogotonFace::setNormal(const sead::Vector3f& vector) {
    al::tryNormalizeOrZero(&mNormal, vector);
}

s32 GotogotonFace::calcOppositeEdgeIndex(s32 edgeIndex) const {
    const GotogotonFace* oppositeFace = mEdges[edgeIndex].mNextFace->mOppositeFace;
    for (s32 i = 0; i < 4; i++)
        if (mEdges[i].mNextFace == oppositeFace)
            return i;

    return -1;
}

void GotogotonFace::rotate(sead::Quatf* outQuat, sead::Vector3f* outTrans, s32 edgeIndex,
                           const sead::Quatf& quat, const sead::Vector3f& trans, float _unk) const {
    sead::Vector3f axis, pos;
    mEdges[edgeIndex].calcAxisAndPos(&axis, &pos, quat, trans);
    al::rotateQuatAndTransDegree(outQuat, outTrans, quat, trans, axis, pos, _unk);
}
