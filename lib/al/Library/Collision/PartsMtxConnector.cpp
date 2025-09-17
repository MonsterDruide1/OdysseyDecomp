#include "Library/Collision/PartsMtxConnector.h"

#include "Library/Matrix/MatrixUtil.h"

namespace al {

MtxConnector::MtxConnector() = default;

MtxConnector::MtxConnector(const sead::Quatf& quat, const sead::Vector3f& vec)
    : mBaseQuat{quat}, mBaseTrans{vec} {}

bool MtxConnector::isConnecting() const {
    return mParentMtx != nullptr;
}

void MtxConnector::clear() {
    mMtx = sead::Matrix34f::ident;
    mParentMtx = nullptr;
}

void MtxConnector::init(const sead::Matrix34f* mtxA, const sead::Matrix34f& mtxB) {
    clear();
    if (mtxA) {
        mMtx = mtxB;
        mParentMtx = mtxA;
    }
}

void MtxConnector::init(const sead::Matrix34f* mtx) {
    clear();
    if (mtx) {
        mMtx.setInverse(*mtx);
        mParentMtx = mtx;
    }
}

void MtxConnector::multVec(sead::Vector3f* vecA, const sead::Vector3f& vecB) const {
    if (!isConnecting()) {
        vecA->set(vecB);
        return;
    }

    sead::Matrix34f mtx = *mParentMtx * mMtx;
    mtx.setTranslation(sead::Vector3f::zero);
    vecA->setMul(mtx, vecB);
}

void MtxConnector::multTrans(sead::Vector3f* vecA, const sead::Vector3f& vecB) const {
    if (!isConnecting()) {
        vecA->set(vecB);
        return;
    }

    vecA->setMul(*mParentMtx * mMtx, vecB);
}

void MtxConnector::multMtx(sead::Matrix34f* mtxA, const sead::Matrix34f& mtxB) const {
    if (!isConnecting()) {
        *mtxA = mtxB;
        return;
    }

    mtxA->setMul(mMtx, mtxB);
    mtxA->setMul(*mParentMtx, *mtxA);
}

void MtxConnector::multQT(sead::Quatf* outQuat, sead::Vector3f* outPos,
                          sead::Vector3f* outScale) const {
    multQT(outQuat, outPos, outScale, mBaseQuat, mBaseTrans);
}

void MtxConnector::multQT(sead::Quatf* outQuat, sead::Vector3f* outPos, sead::Vector3f* outScale,
                          const sead::Quatf& rotation, const sead::Vector3f& trans) const {
    if (!mParentMtx)
        return;

    sead::Matrix34f outMtx;
    sead::Matrix34f mtx;
    mtx.makeQT(rotation, trans);

    multMtx(&outMtx, mtx);

    outMtx.toQuat(*outQuat);
    outPos->set(outMtx.getTranslation());
    outQuat->normalize();
    if (outScale)
        calcMtxScale(outScale, outMtx);
}

void MtxConnector::multQT(sead::Quatf* outQuat, sead::Vector3f* outPos, const sead::Quatf& baseQuat,
                          const sead::Vector3f& baseTrans) const {
    multQT(outQuat, outPos, nullptr, baseQuat, baseTrans);
}

const sead::Quatf& MtxConnector::getBaseQuat() const {
    return mBaseQuat;
}

const sead::Vector3f& MtxConnector::getBaseTrans() const {
    return mBaseTrans;
}

void MtxConnector::setBaseQuatTrans(const sead::Quatf& quat, const sead::Vector3f& trans) {
    mBaseQuat = quat;
    mBaseTrans = trans;
}

void MtxConnector::calcConnectInfo(sead::Vector3f* outPos, sead::Quatf* outQuat,
                                   sead::Vector3f* outScale, const sead::Vector3f& rotation,
                                   const sead::Vector3f& trans) const {
    sead::Matrix34f mtx;
    calcMtxWithOffset(&mtx, rotation, trans);

    if (outPos)
        *outPos = mtx.getBase(3);

    if (outQuat) {
        sead::Matrix34f mtxNorm;
        normalizeMtxScale(&mtxNorm, mtx);
        mtxNorm.toQuat(*outQuat);
    }

    if (outScale)
        calcMtxScale(outScale, mtx);
}

void MtxConnector::calcMtxWithOffset(sead::Matrix34f* outMtx, const sead::Vector3f& trans,
                                     const sead::Vector3f& rotation) const {
    sead::Matrix34f mtx;
    mtx.makeR(rotation);

    multMtx(outMtx, mtx);
}

bool MtxConnector::tryGetParentTrans(sead::Vector3f* trans) const {
    if (!mParentMtx)
        return false;
    const sead::Vector3f& translation = mParentMtx->getBase(3);

    trans->x = translation.x;
    trans->y = translation.y;
    trans->z = translation.z;
    return true;
}

}  // namespace al
