#include "Library/Camera/CameraParamMoveLimit.h"

#include <gfx/seadCamera.h>

#include "Library/Camera/CameraPoser.h"
#include "Library/Matrix/MatrixUtil.h"
#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/ByamlUtil.h"

namespace al {

CameraParamMoveLimit* CameraParamMoveLimit::create(const CameraPoser* poser) {
    CameraParamMoveLimit* result = new CameraParamMoveLimit();
    result->mViewMtx = poser->getViewMtx();
    result->mInvViewMtx.setInverse(poser->getViewMtx());
    return result;
}

CameraParamMoveLimit::CameraParamMoveLimit() {}

void CameraParamMoveLimit::load(const ByamlIter& iter) {
    ByamlIter moveLimitIter;
    if (!iter.tryGetIterByKey(&moveLimitIter, "MoveLimit"))
        return;

    if (tryGetByamlF32(&mPlus.x, moveLimitIter, "PlusX"))
        mHasPlusX = true;
    if (tryGetByamlF32(&mMinus.x, moveLimitIter, "MinusX"))
        mHasMinusX = true;
    if (tryGetByamlF32(&mPlus.y, moveLimitIter, "PlusY"))
        mHasPlusY = true;
    if (tryGetByamlF32(&mMinus.y, moveLimitIter, "MinusY"))
        mHasMinusY = true;
    if (tryGetByamlF32(&mPlus.z, moveLimitIter, "PlusZ"))
        mHasPlusZ = true;
    if (tryGetByamlF32(&mMinus.z, moveLimitIter, "MinusZ"))
        mHasMinusZ = true;
}

void CameraParamMoveLimit::apply(sead::LookAtCamera* camera) const {
    sead::Vector3f constrainedViewAt;

    // required to enforce lifetimes of variables
    {
        f32 preRotYDegree = -mRotYDegree;
        sead::Vector3f inverseViewAt;
        inverseViewAt.setMul(mInvViewMtx, camera->getAt());
        sead::Matrix34f mtxRotateY = sead::Matrix34f::ident;
        rotateMtxYDirDegree(&mtxRotateY, mtxRotateY, preRotYDegree);

        constrainedViewAt.setMul(mtxRotateY, inverseViewAt);

        if (mHasPlusX)
            constrainedViewAt.x = constrainedViewAt.x > mPlus.x ? mPlus.x : constrainedViewAt.x;
        if (mHasPlusY)
            constrainedViewAt.y = constrainedViewAt.y > mPlus.y ? mPlus.y : constrainedViewAt.y;
        if (mHasPlusZ)
            constrainedViewAt.z = constrainedViewAt.z > mPlus.z ? mPlus.z : constrainedViewAt.z;

        if (mHasMinusX)
            constrainedViewAt.x = constrainedViewAt.x < mMinus.x ? mMinus.x : constrainedViewAt.x;
        if (mHasMinusY)
            constrainedViewAt.y = constrainedViewAt.y < mMinus.y ? mMinus.y : constrainedViewAt.y;
        if (mHasMinusZ)
            constrainedViewAt.z = constrainedViewAt.z < mMinus.z ? mMinus.z : constrainedViewAt.z;
    }

    f32 rotYDegree = mRotYDegree;
    sead::Matrix34f mtxRotateY = sead::Matrix34f::ident;
    rotateMtxYDirDegree(&mtxRotateY, mtxRotateY, rotYDegree);

    sead::Vector3f finalInverseViewAt;
    finalInverseViewAt.setMul(mtxRotateY, constrainedViewAt);

    sead::Vector3f finalViewAt;
    finalViewAt.setMul(mViewMtx, finalInverseViewAt);

    sead::Vector3f constraintAdjustment = finalViewAt - camera->getAt();

    camera->setAt(constraintAdjustment + camera->getAt());
    camera->setPos(constraintAdjustment + camera->getPos());
}

}  // namespace al
