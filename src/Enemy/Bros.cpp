#include "Enemy/Bros.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"

BrosWeaponBase::BrosWeaponBase(const char* name, const al::LiveActor* bros)
    : al::LiveActor(name), mBros(bros) {}

void BrosWeaponBase::attach(const sead::Matrix34f* poseMtx, const sead::Vector3f& trans,
                            const sead::Vector3f& rotation, const char* actionName) {
    mPoseMtx = poseMtx;
    mTrans.set(trans);
    mRotation.set(rotation);

    sead::Matrix34f attachMtx;
    calcAttachMtx(&attachMtx, poseMtx, trans, rotation);
    al::updatePoseMtx(this, &attachMtx);
    al::invalidateClipping(this);
    al::offCollide(this);
    if (actionName)
        al::startAction(this, actionName);
    appear();
}

void BrosWeaponBase::calcAttachMtx(sead::Matrix34f* attachMtx, const sead::Matrix34f* poseMtx,
                                   const sead::Vector3f& trans, const sead::Vector3f& rotation) {
    sead::Matrix34f translationMatrix;
    translationMatrix.makeRT(sead::Vector3f::zero, trans);

    sead::Matrix34f rotationMatrix;
    rotationMatrix.makeRT(rotation, sead::Vector3f::zero);

    sead::Matrix34f mtxRT = rotationMatrix * translationMatrix;
    sead::Matrix34f poseMtxNorm = *poseMtx;
    al::normalize(&poseMtxNorm);

    attachMtx->setMul(poseMtxNorm, mtxRT);
}
