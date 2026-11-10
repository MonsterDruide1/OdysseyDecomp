#include "Npc/CapWorldNpcTailJointController.h"

#include "Library/Camera/CameraUtil.h"
#include "Library/Joint/JointControllerKeeper.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/MatrixUtil.h"
#include "Library/Model/ModelJointFunction.h"

CapWorldNpcTailJointController::CapWorldNpcTailJointController(const al::LiveActor* capWorldNpc)
    : al::JointControllerBase(16), mCapWorldNpc(capWorldNpc), mTailMatrices(),
      mTailJointIndices() {
    mTailMatrices.allocBuffer(3, nullptr, 8);
    mTailJointIndices.allocBuffer(3, nullptr, 8);
}

void CapWorldNpcTailJointController::init() {
    static const char* jointNames[] = {"TailEf01", "TailEf02", "TailEf03"};
    for (s32 i = 0; i < 3; ++i) {
        s32 jointIndex = al::getJointIndex(mCapWorldNpc, jointNames[i]);
        appendJointId(jointIndex);
        mTailJointIndices.pushBack(new s32);
        *mTailJointIndices[i] = jointIndex;
        mTailMatrices.pushBack(new sead::Matrix34f);
    }
}

void CapWorldNpcTailJointController::calcJointCallback(s32 jointIndex,
                                                       sead::Matrix34f* jointMtx) {
    if (*mTailJointIndices.unsafeAt(0) == jointIndex) {
        *mTailMatrices.at(0) = *jointMtx;
        return;
    }
    if (*mTailJointIndices.unsafeAt(1) == jointIndex) {
        *mTailMatrices.at(1) = *jointMtx;
        return;
    }
    *mTailMatrices.at(2) = *jointMtx;

    for (s32 i = 0; i < 3; ++i) {
        sead::Matrix34f* tailMtx = mTailMatrices.at(i);
        if (!tailMtx)
            continue;
        sead::Matrix34f mtx = *tailMtx;
        sead::Vector3f scale;
        al::calcMtxScale(&scale, *tailMtx);
        sead::Matrix34f newMtx = mtx;
        sead::Vector3f side;
        sead::Vector3f front = al::getCameraPos(mCapWorldNpc, 0) - mtx.getTranslation();
        sead::Vector3f pos(mtx.m[0][0], mtx.m[1][3], mtx.m[2][3]);
        bool isFrontNormalized = al::tryNormalizeOrZero(&front);
        bool isSideNormalized = al::tryNormalizeOrZero(&side, mtx.getBase(0));
        if (isFrontNormalized && isSideNormalized) {
            al::verticalizeVec(&front, side, front);
            if (al::tryNormalizeOrZero(&front)) {
                al::makeMtxSideFrontPos(&newMtx, side, front, pos);
                al::preScaleMtx(&newMtx, scale);
                *mTailMatrices.unsafeAt(i) = newMtx;
            }
        }
    }

    alModelJointFunction::setDirectJointMtx(mCapWorldNpc->getModelKeeper(),
                                            *mTailJointIndices.unsafeAt(0),
                                            *mTailMatrices.at(0));
    alModelJointFunction::setDirectJointMtx(mCapWorldNpc->getModelKeeper(),
                                            *mTailJointIndices.unsafeAt(1),
                                            *mTailMatrices.at(1));
    *jointMtx = *mTailMatrices.unsafeAt(2);
}

const char* CapWorldNpcTailJointController::getCtrlTypeName() const {
    return "キャップ尻尾回転コントローラー";
}

namespace rs {

void initCapWorldNpcTailJointController(al::LiveActor* capWorldNpc) {
    CapWorldNpcTailJointController* controller =
        new CapWorldNpcTailJointController(capWorldNpc);
    controller->init();
    al::registerJointController(capWorldNpc, controller);
    al::createMatAnimSlotForMts(capWorldNpc);
    al::startMatAnim(capWorldNpc, "TailScroll");
}

}  // namespace rs
