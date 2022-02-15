#include "al/LiveActor/ActorPoseKeeper.h"

#include "al/util/LiveActorUtil.h"
#include "al/util/VectorUtil.h"

namespace al {

static void rotationAndTranslationFromMatrix(sead::Vector3f& trans, sead::Vector3f& rot,
                                             const sead::Matrix34f* mtx) {
    sead::Vector3f tmp;
    mtx->getRotation(tmp);
    rot.set(sead::Mathf::rad2deg(tmp.x), sead::Mathf::rad2deg(tmp.y), sead::Mathf::rad2deg(tmp.z));
    mtx->getTranslation(trans);
}

ActorPoseKeeperBase::ActorPoseKeeperBase() = default;
const sead::Vector3f& ActorPoseKeeperBase::getRotate() const {
    return sead::Vector3f::zero;
}
const sead::Vector3f& ActorPoseKeeperBase::getScale() const {
    return sead::Vector3f::ones;
}
const sead::Vector3f& ActorPoseKeeperBase::getVelocity() const {
    return sead::Vector3f::zero;
}
const sead::Vector3f& ActorPoseKeeperBase::getFront() const {
    return sead::Vector3f::ez;
}
const sead::Vector3f& ActorPoseKeeperBase::getUp() const {
    return sead::Vector3f::ey;
}
const sead::Quatf& ActorPoseKeeperBase::getQuat() const {
    return sead::Quatf::unit;
}
const sead::Vector3f& ActorPoseKeeperBase::getGravity() const {
    return sDefaultVelocity;
}
const sead::Matrix34f& ActorPoseKeeperBase::getMtx() const {
    return sead::Matrix34f::ident;
}
sead::Vector3f* ActorPoseKeeperBase::getRotatePtr() {
    return nullptr;
}
sead::Vector3f* ActorPoseKeeperBase::getScalePtr() {
    return nullptr;
}
sead::Vector3f* ActorPoseKeeperBase::getVelocityPtr() {
    return nullptr;
}
sead::Vector3f* ActorPoseKeeperBase::getFrontPtr() {
    return nullptr;
}
sead::Vector3f* ActorPoseKeeperBase::getUpPtr() {
    return nullptr;
}
sead::Quatf* ActorPoseKeeperBase::getQuatPtr() {
    return nullptr;
}
sead::Vector3f* ActorPoseKeeperBase::getGravityPtr() {
    return nullptr;
}
sead::Matrix34f* ActorPoseKeeperBase::getMtxPtr() {
    return nullptr;
}
void ActorPoseKeeperBase::copyPose(const ActorPoseKeeperBase* other) {
    sead::Matrix34f mtx;
    mtx = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0};
    other->calcBaseMtx(&mtx);
    updatePoseMtx(&mtx);
}
void ActorPoseKeeperBase::updatePoseRotate(const sead::Vector3f& rot) {
    sead::Quatf quat;
    quat.setRPY(sead::Mathf::deg2rad(rot.x), sead::Mathf::deg2rad(rot.y),
                sead::Mathf::deg2rad(rot.z));
    updatePoseQuat(quat);
}

ActorPoseKeeperTFSV::ActorPoseKeeperTFSV() = default;
const sead::Vector3f& ActorPoseKeeperTFSV::getFront() const {
    return mFront;
}
sead::Vector3f* ActorPoseKeeperTFSV::getFrontPtr() {
    return &mFront;
}
const sead::Vector3f& ActorPoseKeeperTFSV::getScale() const {
    return mScale;
}
sead::Vector3f* ActorPoseKeeperTFSV::getScalePtr() {
    return &mScale;
}
const sead::Vector3f& ActorPoseKeeperTFSV::getVelocity() const {
    return mVelocity;
}
sead::Vector3f* ActorPoseKeeperTFSV::getVelocityPtr() {
    return &mVelocity;
}
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

const sead::Vector3f& ActorPoseKeeperTFGSV::getGravity() const {
    return mGravity;
}
sead::Vector3f* ActorPoseKeeperTFGSV::getGravityPtr() {
    return &mGravity;
}
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

const sead::Vector3f& ActorPoseKeeperTFUSV::getUp() const {
    return mUp;
}
sead::Vector3f* ActorPoseKeeperTFUSV::getUpPtr() {
    return &mUp;
}
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

const sead::Quatf& ActorPoseKeeperTQSV::getQuat() const {
    return mQuat;
}
sead::Quatf* ActorPoseKeeperTQSV::getQuatPtr() {
    return &mQuat;
}
const sead::Vector3f& ActorPoseKeeperTQSV::getScale() const {
    return mScale;
}
sead::Vector3f* ActorPoseKeeperTQSV::getScalePtr() {
    return &mScale;
}
const sead::Vector3f& ActorPoseKeeperTQSV::getVelocity() const {
    return mVelocity;
}
sead::Vector3f* ActorPoseKeeperTQSV::getVelocityPtr() {
    return &mVelocity;
}
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

const sead::Quatf& ActorPoseKeeperTQGSV::getQuat() const {
    return mQuat;
}
sead::Quatf* ActorPoseKeeperTQGSV::getQuatPtr() {
    return &mQuat;
}
const sead::Vector3f& ActorPoseKeeperTQGSV::getGravity() const {
    return mGravity;
}
sead::Vector3f* ActorPoseKeeperTQGSV::getGravityPtr() {
    return &mGravity;
}
const sead::Vector3f& ActorPoseKeeperTQGSV::getScale() const {
    return mScale;
}
sead::Vector3f* ActorPoseKeeperTQGSV::getScalePtr() {
    return &mScale;
}
const sead::Vector3f& ActorPoseKeeperTQGSV::getVelocity() const {
    return mVelocity;
}
sead::Vector3f* ActorPoseKeeperTQGSV::getVelocityPtr() {
    return &mVelocity;
}
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

const sead::Quatf& ActorPoseKeeperTQGMSV::getQuat() const {
    return mQuat;
}
sead::Quatf* ActorPoseKeeperTQGMSV::getQuatPtr() {
    return &mQuat;
}
const sead::Vector3f& ActorPoseKeeperTQGMSV::getGravity() const {
    return mGravity;
}
sead::Vector3f* ActorPoseKeeperTQGMSV::getGravityPtr() {
    return &mGravity;
}
const sead::Matrix34f& ActorPoseKeeperTQGMSV::getMtx() const {
    return mMtx;
}
sead::Matrix34f* ActorPoseKeeperTQGMSV::getMtxPtr() {
    return &mMtx;
}
const sead::Vector3f& ActorPoseKeeperTQGMSV::getScale() const {
    return mScale;
}
sead::Vector3f* ActorPoseKeeperTQGMSV::getScalePtr() {
    return &mScale;
}
const sead::Vector3f& ActorPoseKeeperTQGMSV::getVelocity() const {
    return mVelocity;
}
sead::Vector3f* ActorPoseKeeperTQGMSV::getVelocityPtr() {
    return &mVelocity;
}
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

const sead::Vector3f& ActorPoseKeeperTRSV::getRotate() const {
    return mRotate;
}
sead::Vector3f* ActorPoseKeeperTRSV::getRotatePtr() {
    return &mRotate;
}
const sead::Vector3f& ActorPoseKeeperTRSV::getScale() const {
    return mScale;
}
sead::Vector3f* ActorPoseKeeperTRSV::getScalePtr() {
    return &mScale;
}
const sead::Vector3f& ActorPoseKeeperTRSV::getVelocity() const {
    return mVelocity;
}
sead::Vector3f* ActorPoseKeeperTRSV::getVelocityPtr() {
    return &mVelocity;
}
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
    rotationAndTranslationFromMatrix(mTrans, mRotate, mtx);
}
void ActorPoseKeeperTRSV::calcBaseMtx(sead::Matrix34f* mtx) const {
    al::makeMtxRotateTrans(mtx, getRotate(), mTrans);
}

ActorPoseKeeperTRMSV::ActorPoseKeeperTRMSV() {
    mMtx = sead::Matrix34f::ident;
}

const sead::Vector3f& ActorPoseKeeperTRMSV::getRotate() const {
    return mRotate;
}
sead::Vector3f* ActorPoseKeeperTRMSV::getRotatePtr() {
    return &mRotate;
}
const sead::Matrix34f& ActorPoseKeeperTRMSV::getMtx() const {
    return mMtx;
}
sead::Matrix34f* ActorPoseKeeperTRMSV::getMtxPtr() {
    return &mMtx;
}
const sead::Vector3f& ActorPoseKeeperTRMSV::getScale() const {
    return mScale;
}
sead::Vector3f* ActorPoseKeeperTRMSV::getScalePtr() {
    return &mScale;
}
const sead::Vector3f& ActorPoseKeeperTRMSV::getVelocity() const {
    return mVelocity;
}
sead::Vector3f* ActorPoseKeeperTRMSV::getVelocityPtr() {
    return &mVelocity;
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
    rotationAndTranslationFromMatrix(mTrans, mRotate, mtx);
}
void ActorPoseKeeperTRMSV::calcBaseMtx(sead::Matrix34f* mtx) const {
    *mtx = mMtx;
}

// NON_MATCHING: mismatch about storing mGravity
ActorPoseKeeperTRGMSV::ActorPoseKeeperTRGMSV() {
    mMtx = sead::Matrix34f::ident;
}

const sead::Vector3f& ActorPoseKeeperTRGMSV::getRotate() const {
    return mRotate;
}
sead::Vector3f* ActorPoseKeeperTRGMSV::getRotatePtr() {
    return &mRotate;
}
const sead::Vector3f& ActorPoseKeeperTRGMSV::getGravity() const {
    return mGravity;
}
sead::Vector3f* ActorPoseKeeperTRGMSV::getGravityPtr() {
    return &mGravity;
}
const sead::Matrix34f& ActorPoseKeeperTRGMSV::getMtx() const {
    return mMtx;
}
sead::Matrix34f* ActorPoseKeeperTRGMSV::getMtxPtr() {
    return &mMtx;
}
const sead::Vector3f& ActorPoseKeeperTRGMSV::getScale() const {
    return mScale;
}
sead::Vector3f* ActorPoseKeeperTRGMSV::getScalePtr() {
    return &mScale;
}
const sead::Vector3f& ActorPoseKeeperTRGMSV::getVelocity() const {
    return mVelocity;
}
sead::Vector3f* ActorPoseKeeperTRGMSV::getVelocityPtr() {
    return &mVelocity;
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
    rotationAndTranslationFromMatrix(mTrans, mRotate, mtx);
}
void ActorPoseKeeperTRGMSV::calcBaseMtx(sead::Matrix34f* mtx) const {
    *mtx = mMtx;
}

}  // namespace al
