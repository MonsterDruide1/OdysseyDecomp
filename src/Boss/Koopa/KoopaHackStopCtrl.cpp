#include "Boss/Koopa/KoopaHackStopCtrl.h"

#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Scene/SceneObjUtil.h"

KoopaHackStopCtrl::KoopaHackStopCtrl() {}

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
    if (!al::isExistSceneObj<KoopaHackStopCtrl>(actor))
        return false;

    return al::getSceneObj<KoopaHackStopCtrl>(actor)->getStopActor() != nullptr;
}

bool isStatusDemoForSceneKoopaHack(const al::LiveActor* actor) {
    if (!al::isExistSceneObj<KoopaHackStopCtrl>(actor))
        return false;

    return al::getSceneObj<KoopaHackStopCtrl>(actor)->isStatusDemoForSceneKoopaHack() != false;
}

void setStatusDemoForSceneKoopaHack(const al::LiveActor* actor) {
    al::getSceneObj<KoopaHackStopCtrl>(actor)->setStatusDemoForSceneKoopaHack(true);
}

void resetStatusDemoForSceneKoopaHack(const al::LiveActor* actor) {
    al::getSceneObj<KoopaHackStopCtrl>(actor)->setStatusDemoForSceneKoopaHack(false);
}

}  // namespace KoopaHackFunction
