#include "Enemy/EnemyCap.h"

#include <math/seadMatrix.h>
#include <prim/seadSafeString.h>

#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorResourceFunction.h"
#include "Library/LiveActor/SubActorKeeper.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Matrix/MatrixUtil.h"
#include "Library/Movement/EnemyStateBlowDown.h"
#include "Library/Movement/EnemyStateBlowDownParam.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Obj/PartsFunction.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Yaml/ByamlUtil.h"

namespace {
NERVE_IMPL(EnemyCap, Wait);
NERVE_IMPL(EnemyCap, BlowDown);

struct {
    NERVE_MAKE(EnemyCap, Wait);
    NERVE_MAKE(EnemyCap, BlowDown);
} NrvEnemyCap;

}  // namespace

EnemyCap* EnemyCap::createEnemyCap(const char* name) {
    return new EnemyCap(name);
}

EnemyCap::EnemyCap(const char* name) : al::LiveActor(name) {}

const al::EnemyStateBlowDownParam sEnemyCapBlowDownParam =
    al::EnemyStateBlowDownParam(nullptr, 45.0f, 54.0f, 2.6f, 0.94f, 60, 1);

void EnemyCap::initPartsFixFile(al::LiveActor* cap, const al::ActorInitInfo& info,
                                const char* archiveName, const char* suffix) {
    mCap = cap;
    /*
    Non const workaround that causes a mismatch
    sead::Matrix34f baseMtx = *mCap->getBaseMtx();
    mCapBaseMtx = &baseMtx;
    */
    // Casting to get rid of the pointer's const'ness. I'll change mCapBaseMtx to be a const* if
    // this is what we decide to do
    mCapBaseMtx = (sead::Matrix34f*)mCap->getBaseMtx();
    al::initChildActorWithArchiveNameNoPlacementInfo(this, info, archiveName, 0);
    al::initNerve(this, &NrvEnemyCap.Wait, 1);
    mStateBlowDown = new al::EnemyStateBlowDown(this, &sEnemyCapBlowDownParam, "吹き飛び状態");
    al::initNerveState(this, mStateBlowDown, &NrvEnemyCap.BlowDown, "吹き飛び");

    al::StringTmp<128> fileName;
    al::createFileNameBySuffix(&fileName, "InitPartsFixInfo", suffix);
    if (al::isExistModelResourceYaml(mCap, fileName.cstr(), 0)) {
        al::ByamlIter resourceYaml(al::getModelResourceYaml(mCap, fileName.cstr(), 0));
        const char* jointName = nullptr;
        resourceYaml.tryGetStringByKey(&jointName, "JointName");
        if (jointName)
            mCapBaseMtx = al::getJointMtxPtr(mCap, jointName);
        al::tryGetByamlV3f(&mLocalTrans, resourceYaml, "LocalTrans");
        al::tryGetByamlV3f(&mLocalRotate, resourceYaml, "LocalRotate");
        al::tryGetByamlV3f(&mLocalScale, resourceYaml, "LocalScale");
        if (!al::isNearZero(mLocalTrans, 0.001f) || !al::isNearZero(mLocalRotate, 0.001f))
            mIsNotAtOrigin = true;
        mIsUseFollowMtxScale = al::tryGetByamlKeyBoolOrFalse(resourceYaml, "UseFollowMtxScale");
        mIsUseLocalScale = al::tryGetByamlKeyBoolOrFalse(resourceYaml, "UseLocalScale");
    }
    makeActorAlive();
}

void EnemyCap::makeActorAlive() {
    al::invalidateClipping(this);
    al::offCollide(this);
    al::setVelocityZero(this);
    updatePose();
    al::LiveActor::makeActorAlive();
    mIsCapVisible = false;
    al::setNerve(this, &NrvEnemyCap.Wait);
}

// NON_MATCHING: Unfinished
void EnemyCap::updatePose() {
    if (!mIsNotAtOrigin)
        return al::updatePoseMtx(this, mCapBaseMtx);

    sead::Matrix34f rotationMatrix;
    sead::Vector3f rotate(sead::Mathf::deg2rad(mLocalRotate.x),
                          sead::Mathf::deg2rad(mLocalRotate.y),
                          sead::Mathf::deg2rad(mLocalRotate.z));
    rotationMatrix.makeR(rotate);

    sead::Matrix34f translationMatrix;
    sead::Vector3f translation(mLocalTrans.x, mLocalTrans.y, mLocalTrans.z);
    translationMatrix.makeRT(rotate, translation);

    sead::Matrix34f poseMatrix;
    poseMatrix.setMul(rotationMatrix, translationMatrix);

    if (mIsUseFollowMtxScale)
        al::calcMtxScale(&mLocalScale, poseMatrix);
}

void EnemyCap::calcAnim() {
    if (al::isNerve(this, &NrvEnemyCap.Wait))
        updatePose();
    al::LiveActor::calcAnim();
}

void EnemyCap::exeWait() {
    if (!syncHostVisible())
        return;
    al::setModelAlphaMask(this, al::getModelAlphaMask(mCap));
}

bool EnemyCap::syncHostVisible() {
    return al::updateSyncHostVisible(&mIsCapVisible, this, mCap, field_142);
}

void EnemyCap::exeBlowDown() {
    if (al::isFirstStep(this)) {
        al::onCollide(this);
        al::offSyncClippingSubActor(mCap, this);
        al::offSyncAlphaMaskSubActor(mCap, this);
    }
    al::rotateQuatXDirDegree(this, -15.0f);
    if (al::updateNerveState(this)) {
        al::offCollide(this);
        al::startHitReaction(this, "消滅");
        al::onSyncClippingSubActor(mCap, this);
        al::onSyncAlphaMaskSubActor(mCap, this);
        kill();
    }
}

void EnemyCap::startBlowDown(const al::HitSensor* source) {
    if (isBlowDown())
        return;
    mStateBlowDown->start(source);
    al::setNerve(this, &NrvEnemyCap.BlowDown);
}

void EnemyCap::startBlowDown() {
    if (isBlowDown())
        return;
    mStateBlowDown->start(mCap);
    al::setNerve(this, &NrvEnemyCap.BlowDown);
}

bool EnemyCap::isBlowDown() const {
    return al::isNerve(this, &NrvEnemyCap.BlowDown);
}

void EnemyCap::setBlowDownParam(const al::EnemyStateBlowDownParam* param) {
    mStateBlowDown->setParam(param);
}

namespace rs {
EnemyCap* tryCreateEnemyCap(al::LiveActor* actor, const al::ActorInitInfo& info) {
    const char* str = nullptr;
    al::tryGetStringArg(&str, info, "CapName");
    return tryCreateEnemyCap(actor, info, str);
}

EnemyCap* tryCreateEnemyCap(al::LiveActor* actor, const al::ActorInitInfo& info,
                            const char* archiveName) {
    return tryCreateEnemyCapSuffix(actor, info, archiveName, archiveName);
}

EnemyCap* tryCreateEnemyCapSuffix(al::LiveActor* actor, const al::ActorInitInfo& info,
                                  const char* archiveName, const char* suffix) {
    if (!archiveName)
        return nullptr;
    EnemyCap* cap = new EnemyCap("帽子");
    if (!al::isExistSubActorKeeper(actor))
        al::initSubActorKeeperNoFile(actor, info, 1);
    cap->initPartsFixFile(actor, info, archiveName, suffix);
    al::registerSubActor(actor, cap);
    al::onSyncClippingSubActor(actor, cap);
    al::onSyncAlphaMaskSubActor(actor, cap);
    return cap;
}

bool tryStartEnemyCapBlowDown(EnemyCap* cap, const al::HitSensor* sensor) {
    if (!isOnEnemyCap(cap))
        return false;
    cap->startBlowDown(sensor);
    return true;
}

bool tryStartEnemyCapBlowDown(EnemyCap* cap) {
    if (!isOnEnemyCap(cap))
        return false;
    cap->startBlowDown();
    return true;
}

bool tryAppearEnemyCap(EnemyCap* cap) {
    if (!cap)
        return false;
    cap->appear();
    return true;
}

bool isOnEnemyCap(EnemyCap* cap) {
    return cap && !cap->isBlowDown();
}

}  // namespace rs
