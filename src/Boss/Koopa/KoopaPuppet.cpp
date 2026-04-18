#include "Boss/Koopa/KoopaPuppet.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"

#include "Boss/Koopa/Koopa.h"
#include "Util/SensorMsgFunction.h"

KoopaPuppet::KoopaPuppet(Koopa* koopa) : mKoopa(koopa) {}

void KoopaPuppet::start(al::HitSensor* sensor) {
    mKoopa->startBind();
    mHitSensor = sensor;
    mBindMtx = nullptr;
}

void KoopaPuppet::end() {
    mKoopa->endBind();
    mHitSensor = nullptr;
}

void KoopaPuppet::endAndPopUp(const sead::Vector3f& popUpPos) {
    mKoopa->endBindAndPopUp(popUpPos);
    mHitSensor = nullptr;
}

bool KoopaPuppet::isBindWait() const {
    return mKoopa->isBindWait();
}

void KoopaPuppet::change2D() {
    mKoopa->changeBind2D();
}

void KoopaPuppet::changeHackStart(const sead::Vector3f& startTrans,
                                  const sead::Matrix34f* bindMtx) {
    sead::Vector3f bindUp;
    bindUp.x = bindMtx->m[0][2];
    bindUp.y = bindMtx->m[1][2];
    bindUp.z = bindMtx->m[2][2];
    mKoopa->changeBindHackStart(startTrans, bindUp);
    mBindMtx = bindMtx;
}

f32 KoopaPuppet::getBindHackStartActionFrame() const {
    return mKoopa->getBindHackStartActionFrame();
}

f32 KoopaPuppet::getBindHackStartActionFrameMax() const {
    return mKoopa->getBindHackStartActionFrameMax();
}

const sead::Vector3f& KoopaPuppet::getTrans() const {
    return al::getTrans(mKoopa);
}

void KoopaPuppet::calcQuat(sead::Quatf* quat) const {
    al::calcQuat(quat, mKoopa);
}

void KoopaPuppet::setTrans(const sead::Vector3f& trans) {
    al::setTrans(mKoopa, trans);
}

void KoopaPuppet::resetPosition(const sead::Vector3f& pos) {
    mKoopa->resetBindPosition(pos);
}

void KoopaPuppet::updatePoseQuat(const sead::Quatf& quat) {
    al::updatePoseQuat(mKoopa, quat);
}

void KoopaPuppet::startAction(const char* actionName) {
    al::startAction(mKoopa, actionName);
}

void KoopaPuppet::calcPuppetMoveDir(sead::Vector3f* dir, const sead::Vector3f& moveInput) const {
    mKoopa->calcPuppetMoveDir(dir, moveInput);
}

bool KoopaPuppet::isTriggerSwing() const {
    return mKoopa->isTriggerSwing();
}

void KoopaPuppet::hideModel() {
    mKoopa->hideBindModel();
}

void KoopaPuppet::showModel() {
    al::showModelIfHide(mKoopa);
}

void KoopaPuppet::hideSilhouetteModel() {
    al::hideSilhouetteModelIfShow(mKoopa);
}

void KoopaPuppet::showSilhouetteModel() {
    al::showSilhouetteModelIfHide(mKoopa);
}

void KoopaPuppet::changeHackTutorialElectricWire() {
    mKoopa->changeHackTutorialElectricWire();
}

void KoopaPuppet::closeHackTutorial() {
    mKoopa->closeHackTutorial();
}

KoopaPuppet* rs::tryStartKoopaPuppet(al::HitSensor* receiver, al::HitSensor* sender) {
    if (!rs::sendMsgKoopaBindStart(receiver, sender))
        return nullptr;

    auto* host = (Koopa*)al::getSensorHost(receiver);
    auto* puppet = host->getPuppet();
    puppet->start(sender);
    return puppet;
}

void rs::endKoopaPuppet(KoopaPuppet** puppet) {
    (*puppet)->end();
    *puppet = nullptr;
}

void rs::endKoopaPuppetAndPopUp(KoopaPuppet** puppet, const sead::Vector3f& popUpPos) {
    (*puppet)->endAndPopUp(popUpPos);
    *puppet = nullptr;
}
