#include "MapObj/PropellerRotateCtrl.h"

#include "Library/Joint/JointControllerKeeper.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/MatrixUtil.h"

PropellerRotateCtrl::PropellerRotateCtrl(al::LiveActor* actor, const PropellerRotateInfo& info)
    : al::JointControllerBase(16), mRotateInfo(&info) {
    appendJointId(al::getJointIndex(actor, info.mJointName));
    al::registerJointController(actor, this);
}

void PropellerRotateCtrl::update() {
    mRotateFrame =
        al::modi(mRotateFrame + mRotateInfo->mRotatePeriod + 1, mRotateInfo->mRotatePeriod);
}

void PropellerRotateCtrl::calcJointCallback(s32 jointIndex, sead::Matrix34f* mtx) {
    if (mRotateFrame < 1)
        return;

    s32 axis = mRotateInfo->mRotateAxis % 3;
    f32 direction = mRotateInfo->mRotateDirection;
    if (axis == 0) {
        direction *= al::normalize(mRotateFrame, 0, mRotateInfo->mRotatePeriod);
        al::rotateMtxXDirDegree(mtx, *mtx, direction * 360.0f);
    } else if (axis == 1)
        al::rotateMtxYDirDegree(
            mtx, *mtx,
            direction * al::normalize(mRotateFrame, 0, mRotateInfo->mRotatePeriod) * 360.0f);
    else
        al::rotateMtxZDirDegree(
            mtx, *mtx,
            direction * al::normalize(mRotateFrame, 0, mRotateInfo->mRotatePeriod) * 360.0f);
}
