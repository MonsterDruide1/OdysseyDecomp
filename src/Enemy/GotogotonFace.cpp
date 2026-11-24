#include "Enemy/GotogotonFace.h"

#include "Library/Math/MathUtil.h"
#include "Library/Math/MatrixUtil.h"

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

s32 GotogotonFace::calcNearEdgeIndexToPos(const sead::Vector3f& pos, const sead::Quatf& quat,
                                          const sead::Vector3f& trans) const {
    f32 bestDist = sead::Mathf::maxNumber();
    s32 index = -1;
    for (s32 i = 0; i < 4; i++) {
        sead::Vector3f edgePos;
        mEdges[i].calcAxisAndPos(nullptr, &edgePos, quat, trans);
        sead::Vector3f diff = edgePos - pos;
        f32 dist = diff.length();
        if (dist < bestDist) {
            index = i;
            bestDist = dist;
        }
    }

    return index;
}

s32 GotogotonFace::calcNearEdgeIndexToDir(const sead::Vector3f& dir, const sead::Quatf& quat,
                                          const sead::Vector3f& trans) const {
    sead::Vector3f center;
    calcCenterPos(&center, quat, trans);
    sead::Vector3f norm;
    calcCenterDir(&norm, quat);
    sead::Vector3f verticalNorm;
    al::verticalizeVec(&verticalNorm, norm, dir);
    if (!al::tryNormalizeOrZero(&verticalNorm))
        return -1;

    f32 bestDot = -1.0f;
    s32 index = -1;
    for (s32 i = 0; i < 4; i++) {
        sead::Vector3f edgePos;
        mEdges[i].calcAxisAndPos(nullptr, &edgePos, quat, trans);
        sead::Vector3f diff = edgePos - center;
        al::tryNormalizeOrZero(&diff);
        f32 dotProduct = dir.dot(diff);
        if (bestDot < dotProduct) {
            index = i;
            bestDot = dotProduct;
        }
    }

    return index;
}

s32 GotogotonFace::calcOppositeEdgeIndex(s32 edgeIndex) const {
    const GotogotonFace* oppositeFace = mEdges[edgeIndex].mNextFace->mOppositeFace;
    for (s32 i = 0; i < 4; i++)
        if (mEdges[i].mNextFace == oppositeFace)
            return i;

    return -1;
}

void GotogotonFace::calcRotateCenterPos(sead::Vector3f* outPos, s32 edgeIndex,
                                        const sead::Quatf& quat, const sead::Vector3f& trans,
                                        f32 degree) const {
    sead::Vector3f axis, pos;
    mEdges[edgeIndex].calcAxisAndPos(&axis, &pos, quat, trans);

    sead::Vector3f center;
    getNextFace(edgeIndex)->calcCenterPos(&center, quat, trans);
    al::rotateQuatAndTransDegree(nullptr, outPos, sead::Quatf::unit, center, axis, pos, degree);
}

void GotogotonFace::calcLandEffectMtx(sead::Matrix34f* ouMtx, const sead::Quatf& quat,
                                      const sead::Vector3f& trans, s32 edgeIndex) const {
    sead::Vector3f crossProduct = mEdges[edgeIndex].mAxis.cross(mNormal);

    sead::Matrix34f local;
    al::makeMtxUpFrontPos(&local, crossProduct, -mNormal, mCenter);

    sead::Matrix34f world;
    world.makeQT(quat, trans);

    *ouMtx = world * local;
}

void GotogotonFace::calcCenterPos(sead::Vector3f* outPos, const sead::Quatf& quat,
                                  const sead::Vector3f& trans) const {
    sead::Matrix34f mtx;
    mtx.makeQT(quat, trans);
    outPos->setMul(mtx, mCenter);
}

void GotogotonFace::calcCenterDir(sead::Vector3f* outDir, const sead::Quatf& quat) const {
    outDir->setRotated(quat, mNormal);
}

void GotogotonFace::rotate(sead::Quatf* outQuat, sead::Vector3f* outTrans, s32 edgeIndex,
                           const sead::Quatf& quat, const sead::Vector3f& trans, float _unk) const {
    sead::Vector3f axis, pos;
    mEdges[edgeIndex].calcAxisAndPos(&axis, &pos, quat, trans);
    al::rotateQuatAndTransDegree(outQuat, outTrans, quat, trans, axis, pos, _unk);
}
