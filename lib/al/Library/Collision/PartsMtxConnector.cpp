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

void MtxConnector::init(const sead::Matrix34f* parentMtx, const sead::Matrix34f& mtx) {
    clear();
    if (parentMtx) {
        mMtx = mtx;
        mParentMtx = parentMtx;
    }
}

void MtxConnector::init(const sead::Matrix34f* parentMtx) {
    clear();
    if (parentMtx) {
        mMtx.setInverse(*parentMtx);
        mParentMtx = parentMtx;
    }
}

void MtxConnector::multVec(sead::Vector3f* outVec, const sead::Vector3f& vec) const {
    if (!isConnecting()) {
        outVec->set(vec);
        return;
    }

    sead::Matrix34f mtx = *mParentMtx * mMtx;
    mtx.setTranslation(sead::Vector3f::zero);
    outVec->setMul(mtx, vec);
}

void MtxConnector::multTrans(sead::Vector3f* outVec, const sead::Vector3f& vec) const {
    if (!isConnecting()) {
        outVec->set(vec);
        return;
    }

    outVec->setMul(*mParentMtx * mMtx, vec);
}

void MtxConnector::multMtx(sead::Matrix34f* outMtx, const sead::Matrix34f& mtx) const {
    if (!isConnecting()) {
        *outMtx = mtx;
        return;
    }

    outMtx->setMul(mMtx, mtx);
    outMtx->setMul(*mParentMtx, *outMtx);
}

void MtxConnector::multQT(sead::Quatf* outQuat, sead::Vector3f* outPos,
                          sead::Vector3f* outScale) const {
    multQT(outQuat, outPos, outScale, mBaseQuat, mBaseTrans);
}

void MtxConnector::multQT(sead::Quatf* outQuat, sead::Vector3f* outPos, sead::Vector3f* outScale,
                          const sead::Quatf& quat, const sead::Vector3f& trans) const {
    if (!isConnecting())
        return;

    sead::Matrix34f mtx;
    mtx.makeQT(quat, trans);
    multMtx(&mtx, mtx);

    mtx.toQuat(*outQuat);
    mtx.getTranslation(*outPos);
    outQuat->normalize();

    if (outScale)
        calcMtxScale(outScale, mtx);
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
                                   sead::Vector3f* outScale, const sead::Vector3f& trans,
                                   const sead::Vector3f& rotation) const {
    sead::Matrix34f mtx;
    calcMtxWithOffset(&mtx, trans, rotation);

    if (outPos)
        mtx.getTranslation(*outPos);

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
    sead::Matrix34f offsetMtx;
    offsetMtx.makeT(trans);

    sead::Matrix34f mtx;
    mtx.makeR(rotation);

    multMtx(outMtx, offsetMtx * mtx);
}

bool MtxConnector::tryGetParentTrans(sead::Vector3f* outTrans) const {
    if (!mParentMtx)
        return false;

    mParentMtx->getTranslation(*outTrans);
    return true;
}

}  // namespace al
