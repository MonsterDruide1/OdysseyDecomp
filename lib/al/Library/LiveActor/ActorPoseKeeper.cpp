#include "Library/LiveActor/ActorPoseKeeper.h"

#include "Library/Math/MathUtil.h"
#include "Library/Matrix/MatrixUtil.h"

namespace al {

static void rotationAndTranslationFromMatrix(sead::Vector3f* trans, sead::Vector3f* rot,
                                             const sead::Matrix34f* mtx) {
    sead::Vector3f tmp;
    mtx->getRotation(tmp);
    rot->set(sead::Mathf::rad2deg(tmp.x), sead::Mathf::rad2deg(tmp.y), sead::Mathf::rad2deg(tmp.z));
    mtx->getTranslation(*trans);
}

sead::Vector3f ActorPoseKeeperBase::sDefaultVelocity = {0.0f, -1.0f, 0.0f};

ActorPoseKeeperBase::ActorPoseKeeperBase() = default;

void ActorPoseKeeperBase::copyPose(const ActorPoseKeeperBase* other) {
    sead::Matrix34f mtx;
    mtx = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0};
    other->calcBaseMtx(&mtx);
    updatePoseMtx(&mtx);
}

ActorPoseKeeperTFSV::ActorPoseKeeperTFSV() = default;

void ActorPoseKeeperTFSV::updatePoseTrans(const sead::Vector3f& trans) {
    mTrans = trans;
}

void ActorPoseKeeperTFSV::updatePoseRotate(const sead::Vector3f& rot) {
    sead::Quatf quat;
    quat.setRPY(sead::Mathf::deg2rad(rot.x), sead::Mathf::deg2rad(rot.y),
                sead::Mathf::deg2rad(rot.z));
    calcQuatFront(&mFront, quat);
}

void ActorPoseKeeperTFSV::updatePoseQuat(const sead::Quatf& quat) {
    calcQuatFront(&mFront, quat);
}

void ActorPoseKeeperTFSV::updatePoseMtx(const sead::Matrix34f* mtx) {
    mtx->getBase(mFront, 2);
    mtx->getBase(mTrans, 3);
}

void ActorPoseKeeperTFSV::calcBaseMtx(sead::Matrix34f* mtx) const {
    makeMtxFrontUpPos(mtx, mFront, -getGravity(), mTrans);
}

ActorPoseKeeperTFGSV::ActorPoseKeeperTFGSV() = default;

void ActorPoseKeeperTFGSV::updatePoseTrans(const sead::Vector3f& trans) {
    ActorPoseKeeperTFSV::updatePoseTrans(trans);
}

void ActorPoseKeeperTFGSV::updatePoseRotate(const sead::Vector3f& rot) {
    sead::Quatf quat;
    quat.setRPY(sead::Mathf::deg2rad(rot.x), sead::Mathf::deg2rad(rot.y),
                sead::Mathf::deg2rad(rot.z));

    ActorPoseKeeperTFSV::updatePoseQuat(quat);
    calcQuatUp(&mGravity, quat);
    mGravity *= -1;
}

void ActorPoseKeeperTFGSV::updatePoseQuat(const sead::Quatf& quat) {
    ActorPoseKeeperTFSV::updatePoseQuat(quat);
    calcQuatUp(&mGravity, quat);
    mGravity *= -1;
}

void ActorPoseKeeperTFGSV::updatePoseMtx(const sead::Matrix34f* mtx) {
    ActorPoseKeeperTFSV::updatePoseMtx(mtx);
    mtx->getBase(mGravity, 1);
    mGravity *= -1;
}

void ActorPoseKeeperTFGSV::calcBaseMtx(sead::Matrix34f* mtx) const {
    makeMtxUpFrontPos(mtx, -getGravity(), getFront(), mTrans);
}

ActorPoseKeeperTFUSV::ActorPoseKeeperTFUSV() = default;

void ActorPoseKeeperTFUSV::updatePoseTrans(const sead::Vector3f& trans) {
    ActorPoseKeeperTFSV::updatePoseTrans(trans);
}

void ActorPoseKeeperTFUSV::updatePoseRotate(const sead::Vector3f& rot) {
    sead::Quatf quat;
    quat.setRPY(sead::Mathf::deg2rad(rot.x), sead::Mathf::deg2rad(rot.y),
                sead::Mathf::deg2rad(rot.z));

    ActorPoseKeeperTFSV::updatePoseQuat(quat);
    calcQuatUp(&mUp, quat);
}

void ActorPoseKeeperTFUSV::updatePoseQuat(const sead::Quatf& quat) {
    ActorPoseKeeperTFSV::updatePoseQuat(quat);
    calcQuatUp(&mUp, quat);
}

void ActorPoseKeeperTFUSV::updatePoseMtx(const sead::Matrix34f* mtx) {
    ActorPoseKeeperTFSV::updatePoseMtx(mtx);
    mtx->getBase(mUp, 1);
}

void ActorPoseKeeperTFUSV::calcBaseMtx(sead::Matrix34f* mtx) const {
    if (mIsFrontUp)
        makeMtxFrontUpPos(mtx, getFront(), getUp(), mTrans);
    else
        makeMtxUpFrontPos(mtx, getUp(), getFront(), mTrans);
}

ActorPoseKeeperTQSV::ActorPoseKeeperTQSV() = default;

void ActorPoseKeeperTQSV::updatePoseTrans(const sead::Vector3f& trans) {
    mTrans = trans;
}

void ActorPoseKeeperTQSV::updatePoseRotate(const sead::Vector3f& rot) {
    mQuat.setRPY(sead::Mathf::deg2rad(rot.x), sead::Mathf::deg2rad(rot.y),
                 sead::Mathf::deg2rad(rot.z));
}

void ActorPoseKeeperTQSV::updatePoseQuat(const sead::Quatf& quat) {
    mQuat.x = quat.x;
    mQuat.y = quat.y;
    mQuat.z = quat.z;
    mQuat.w = quat.w;
}

void ActorPoseKeeperTQSV::updatePoseMtx(const sead::Matrix34f* mtx) {
    mtx->toQuat(mQuat);
    mtx->getBase(mTrans, 3);
}

void ActorPoseKeeperTQSV::calcBaseMtx(sead::Matrix34f* mtx) const {
    mtx->makeQT(mQuat, mTrans);
}

ActorPoseKeeperTQGSV::ActorPoseKeeperTQGSV() = default;

void ActorPoseKeeperTQGSV::updatePoseTrans(const sead::Vector3f& trans) {
    mTrans = trans;
}

void ActorPoseKeeperTQGSV::updatePoseRotate(const sead::Vector3f& rot) {
    mQuat.setRPY(sead::Mathf::deg2rad(rot.x), sead::Mathf::deg2rad(rot.y),
                 sead::Mathf::deg2rad(rot.z));
}

void ActorPoseKeeperTQGSV::updatePoseQuat(const sead::Quatf& quat) {
    mQuat.x = quat.x;
    mQuat.y = quat.y;
    mQuat.z = quat.z;
    mQuat.w = quat.w;
}

void ActorPoseKeeperTQGSV::updatePoseMtx(const sead::Matrix34f* mtx) {
    mtx->toQuat(mQuat);
    mtx->getTranslation(mTrans);
}

void ActorPoseKeeperTQGSV::calcBaseMtx(sead::Matrix34f* mtx) const {
    mtx->makeQT(mQuat, mTrans);
}

ActorPoseKeeperTQGMSV::ActorPoseKeeperTQGMSV() = default;

void ActorPoseKeeperTQGMSV::updatePoseTrans(const sead::Vector3f& trans) {
    mTrans = trans;
    mMtx.makeQT(mQuat, mTrans);
}

void ActorPoseKeeperTQGMSV::updatePoseRotate(const sead::Vector3f& rot) {
    mQuat.setRPY(sead::Mathf::deg2rad(rot.x), sead::Mathf::deg2rad(rot.y),
                 sead::Mathf::deg2rad(rot.z));
    mMtx.makeQT(mQuat, mTrans);
}

void ActorPoseKeeperTQGMSV::updatePoseQuat(const sead::Quatf& quat) {
    mQuat.x = quat.x;
    mQuat.y = quat.y;
    mQuat.z = quat.z;
    mQuat.w = quat.w;
    mMtx.makeQT(mQuat, mTrans);
}

void ActorPoseKeeperTQGMSV::updatePoseMtx(const sead::Matrix34f* mtx) {
    mtx->toQuat(mQuat);
    mtx->getTranslation(mTrans);
    mMtx.makeQT(mQuat, mTrans);
}

void ActorPoseKeeperTQGMSV::calcBaseMtx(sead::Matrix34f* mtx) const {
    *mtx = mMtx;
}

ActorPoseKeeperTRSV::ActorPoseKeeperTRSV() = default;

void ActorPoseKeeperTRSV::updatePoseTrans(const sead::Vector3f& trans) {
    mTrans = trans;
}

void ActorPoseKeeperTRSV::updatePoseRotate(const sead::Vector3f& rot) {
    mRotate = rot;
}

void ActorPoseKeeperTRSV::updatePoseQuat(const sead::Quatf& quat) {
    sead::Vector3f tmp;
    quat.calcRPY(tmp);
    mRotate = {sead::Mathf::rad2deg(tmp.x), sead::Mathf::rad2deg(tmp.y),
               sead::Mathf::rad2deg(tmp.z)};
}

void ActorPoseKeeperTRSV::updatePoseMtx(const sead::Matrix34f* mtx) {
    rotationAndTranslationFromMatrix(&mTrans, &mRotate, mtx);
}

void ActorPoseKeeperTRSV::calcBaseMtx(sead::Matrix34f* mtx) const {
    makeMtxRotateTrans(mtx, getRotate(), mTrans);
}

ActorPoseKeeperTRMSV::ActorPoseKeeperTRMSV() {
    mMtx = sead::Matrix34f::ident;
}

void ActorPoseKeeperTRMSV::updatePoseTrans(const sead::Vector3f& trans) {
    mTrans = trans;
    sead::Vector3f rot = getRotate() * (sead::numbers::pi / 180);
    mMtx.makeRT(rot, mTrans);
}

void ActorPoseKeeperTRMSV::updatePoseRotate(const sead::Vector3f& rot) {
    mRotate = rot;
    sead::Vector3f rot2 = getRotate() * (sead::numbers::pi / 180);
    mMtx.makeRT(rot2, mTrans);
}

void ActorPoseKeeperTRMSV::updatePoseQuat(const sead::Quatf& quat) {
    sead::Vector3f tmp;
    quat.calcRPY(tmp);
    mRotate = tmp * (180 / sead::numbers::pi);
    sead::Vector3f rot = getRotate() * (sead::numbers::pi / 180);
    mMtx.makeRT(rot, mTrans);
}

void ActorPoseKeeperTRMSV::updatePoseMtx(const sead::Matrix34f* mtx) {
    mMtx = *mtx;
    rotationAndTranslationFromMatrix(&mTrans, &mRotate, mtx);
}

void ActorPoseKeeperTRMSV::calcBaseMtx(sead::Matrix34f* mtx) const {
    *mtx = mMtx;
}

ActorPoseKeeperTRGMSV::ActorPoseKeeperTRGMSV() {
    mMtx = sead::Matrix34f::ident;
}

void ActorPoseKeeperTRGMSV::updatePoseTrans(const sead::Vector3f& trans) {
    mTrans = trans;
    sead::Vector3f rot = getRotate() * (sead::numbers::pi / 180);
    mMtx.makeRT(rot, mTrans);
}

void ActorPoseKeeperTRGMSV::updatePoseRotate(const sead::Vector3f& rot) {
    mRotate = rot;
    sead::Vector3f rot2 = getRotate() * (sead::numbers::pi / 180);
    mMtx.makeRT(rot2, mTrans);
}

void ActorPoseKeeperTRGMSV::updatePoseQuat(const sead::Quatf& quat) {
    sead::Vector3f tmp;
    quat.calcRPY(tmp);
    mRotate = tmp * (180 / sead::numbers::pi);
    sead::Vector3f rot = getRotate() * (sead::numbers::pi / 180);
    mMtx.makeRT(rot, mTrans);
}

void ActorPoseKeeperTRGMSV::updatePoseMtx(const sead::Matrix34f* mtx) {
    mMtx = *mtx;
    rotationAndTranslationFromMatrix(&mTrans, &mRotate, mtx);
}

void ActorPoseKeeperTRGMSV::calcBaseMtx(sead::Matrix34f* mtx) const {
    *mtx = mMtx;
}

}  // namespace al
