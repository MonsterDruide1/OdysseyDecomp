#include "Boss/Koopa/KoopaHackStopCtrl.h"

#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Scene/SceneObjUtil.h"

KoopaHackStopCtrl::KoopaHackStopCtrl() = default;

void KoopaHackStopCtrl::startStop(const al::LiveActor* actor) {
    mStopActor = actor;
}

void KoopaHackStopCtrl::endStop(const al::LiveActor* actor) {
    mStopActor = nullptr;
}

void KoopaHackStopCtrl::resetPosture(const al::LiveActor* actor, const sead::Quatf& quat,
                                     const sead::Vector3f& trans) {
    mIsNeedResetPosture = true;
    mResetQuat.set(quat);
    mResetTrans.set(trans);
}

bool KoopaHackStopCtrl::tryResetPosture(al::LiveActor* actor) {
    if (!mIsNeedResetPosture)
        return false;

    mIsNeedResetPosture = false;
    al::resetQuatPosition(actor, mResetQuat, mResetTrans);
    return true;
}

namespace KoopaHackFunction {

void startStopKoopaHack(al::LiveActor* actor) {
    al::getSceneObj<KoopaHackStopCtrl>(actor)->startStop(actor);
}

void endStopKoopaHack(al::LiveActor* actor) {
    al::getSceneObj<KoopaHackStopCtrl>(actor)->endStop(actor);
}

void resetPostureKoopaHack(al::LiveActor* actor, const sead::Quatf& quat,
                           const sead::Vector3f& trans) {
    al::getSceneObj<KoopaHackStopCtrl>(actor)->resetPosture(actor, quat, trans);
}

bool isStopKoopaHack(const al::LiveActor* actor) {
    return al::isExistSceneObj<KoopaHackStopCtrl>(actor) &&
           al::getSceneObj<KoopaHackStopCtrl>(actor)->isStop();
}

bool isStatusDemoForSceneKoopaHack(const al::LiveActor* actor) {
    return al::isExistSceneObj<KoopaHackStopCtrl>(actor) &&
           al::getSceneObj<KoopaHackStopCtrl>(actor)->isStatusDemoForSceneKoopaHack();
}

void setStatusDemoForSceneKoopaHack(const al::LiveActor* actor) {
    al::getSceneObj<KoopaHackStopCtrl>(actor)->setStatusDemoForSceneKoopaHack(true);
}

void resetStatusDemoForSceneKoopaHack(const al::LiveActor* actor) {
    al::getSceneObj<KoopaHackStopCtrl>(actor)->setStatusDemoForSceneKoopaHack(false);
}

}  // namespace KoopaHackFunction
