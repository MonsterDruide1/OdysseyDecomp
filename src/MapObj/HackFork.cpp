#include "MapObj/HackFork.h"

#include "Library/Camera/CameraTicket.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/Collision/PartsConnectorUtil.h"
#include "Library/Controller/PadRumbleFunction.h"
#include "Library/Event/EventFlowUtil.h"
#include "Library/Joint/JointControllerKeeper.h"
#include "Library/Joint/JointLocalAxisRotator.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorResourceFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Play/Camera/ActorMatrixCameraTarget.h"
#include "Library/Se/SeFunction.h"
#include "Library/Yaml/ByamlUtil.h"

#include "Player/CapTargetInfo.h"
#include "Player/IUsePlayerHack.h"
#include "Player/PlayerHackStartShaderCtrl.h"
#include "Scene/GuidePosInfoHolder.h"
#include "System/GameDataUtil.h"
#include "Util/Hack.h"
#include "Util/NpcAnimUtil.h"
#include "Util/NpcEventFlowUtil.h"
#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(HackFork, Wait);
NERVE_IMPL(HackFork, HackStartWait);
NERVE_IMPL(HackFork, Damping);
NERVE_IMPL(HackFork, HackStart);
NERVE_IMPL(HackFork, HackWait);
NERVE_IMPL(HackFork, HackBend);
NERVE_IMPL(HackFork, HackShoot);

NERVES_MAKE_STRUCT(HackFork, Wait, HackStartWait, Damping, HackStart, HackWait, HackBend,
                   HackShoot);
}  // namespace

PlayerHackStartShaderParam sPlayerHackStartShaderParam(true, 100.0f, 10, 20);

HackFork::HackFork(const char* name) : al::LiveActor(name) {}

// NON_MATCHING: Regswap and missing instructions https://decomp.me/scratch/NeUHJ
void HackFork::init(const al::ActorInitInfo& info) {
    const char* modelName = nullptr;
    if (alPlacementFunction::tryGetModelName(&modelName, info))
        al::initActorWithArchiveName(this, info, modelName, nullptr);
    else
        al::initActor(this, info);

    al::calcSideDir(&mSideDir, this);
    al::initJointControllerKeeper(this, 10);
    mJointRotationHolder.allocBuffer(5, nullptr);

    const char* jointNames[5] = {"Stick01", "Stick02", "Stick03", "Stick04", "Stick05"};
    for (s32 i = 0; i < 5; i++) {
        mJointRotationHolder.pushBack(
            al::initJointLocalAxisRotator(this, mSideDir, &mDampingForce, jointNames[i], false));
        al::initJointLocalXRotator(this, &mDampingStrength, jointNames[i]);
    }

    al::initNerve(this, &NrvHackFork.Wait, 0);
    al::tryGetArg(&mIsLimitterFree, info, "LimitterFree");
    bool hasCamera = false;
    bool isValid = al::tryGetArg(&hasCamera, info, "Camera");
    if (hasCamera && isValid)
        mCameraTicket = al::initObjectCamera(this, info, nullptr, nullptr);

    mMatrixCameraTarget = al::createActorMatrixCameraTarget(this, &mCameraTargetMtx);
    if (!al::isEqualString(modelName, "HackBoard")) {
        mCameraOffset = {0.0f, 0.0f, 100.0f};
        mIsHackBoard = true;
    }

    bool hasBallon = false;
    bool isBallonValid = al::tryGetArg(&hasBallon, info, "Balloon");
    if (hasBallon && isBallonValid && !rs::isSequenceTimeBalloonOrRace(this)) {
        mEventFlowExecutor = rs::initEventFlow(this, info, nullptr, nullptr);
        rs::startEventFlow(mEventFlowExecutor, "Init");
    }
    if (al::isMtpAnimExist(this)) {
        rs::setNpcMaterialAnimFromPlacementInfo(this, info);
        al::tryStartMclAnimIfExist(this, al::getPlayingMtpAnimName(this));
    }
    mMtxConnector = al::tryCreateMtxConnector(this, info);
    makeActorAlive();
    mCapTargetInfo = rs::createCapTargetInfo(this, nullptr);

    al::ByamlIter iter = {al::getModelResourceYaml(this, "InitHackCap", nullptr)};
    mJointName = al::tryGetByamlKeyStringOrNULL(iter, "JointName");
    sead::Vector3f localTrans = {0.0f, 0.0f, 0.0f};
    al::tryGetByamlV3f(&localTrans, iter, "LocalTrans");
    sead::Vector3f localRotate = {0.0f, 0.0f, 0.0f};
    al::tryGetByamlV3f(&localRotate, iter, "LocalRotate");

    sead::Matrix34f rotationMatrix;
    sead::Vector3f rotateRad(sead::Mathf::deg2rad(localRotate.x),
                             sead::Mathf::deg2rad(localRotate.y),
                             sead::Mathf::deg2rad(localRotate.z));
    rotationMatrix.makeR(rotateRad);

    sead::Matrix34f translationMatrix;
    translationMatrix.makeRT({0.0f, 0.0f, 0.0f}, localTrans * al::getScaleY(this));

    mStartingPoseMtx = rotationMatrix * translationMatrix;

    sead::Matrix34f jointMtx = *al::getJointMtxPtr(this, mJointName);
    al::normalize(&jointMtx);
    mInvJointMtx.setInverse(jointMtx);

    mCapTargetInfo->setPoseMatrix(&mCapPoseMtx);
    mInvInitialHackDir.setInverse(mUpsideDownInitialHackDir);

    initBasicPoseInfo();
    mHackStartShaderCtrl = new PlayerHackStartShaderCtrl(this, &sPlayerHackStartShaderParam);
}

void HackFork::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isSensorName(self, "Push") && !al::sendMsgPush(other, self) && !mIsHorizontal) {
        const sead::Vector3f& velocity = al::getActorVelocity(other);
        if (velocity.x * velocity.x + velocity.z * velocity.z < 4.999696f)
            rs::sendMsgPushToPlayer(other, self);
    }
}

bool HackFork::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    if (rs::isMsgEnableMapCheckPointWarp(message))
        return false;

    if (rs::isMsgMotorcycleDashAttack(message))
        return tryTouch(5.0f, "タッチ（強）");
    if (al::isMsgPlayerObjTouch(message) || al::isMsgKickStoneAttackReflect(message) ||
        rs::isMsgRadishReflect(message) || rs::isMsgSeedReflect(message)) {
        return tryTouch(2.0f, "タッチ（弱）");
    }
    if (rs::isMsgHammerBrosHammerHackAttack(message) || al::isMsgPlayerFireBallAttack(message)) {
        tryTouch(2.0f, "タッチ（弱）");
        rs::requestHitReactionToAttacker(message, self, other);
        return true;
    }
    if (rs::tryReceiveMsgInitCapTargetAndSetCapTargetInfo(message, mCapTargetInfo)) {
        resetCapMtx(self);
        return true;
    }
    if (rs::isMsgPlayerDisregardTargetMarker(message))
        return mEventFlowExecutor && al::isActive(mEventFlowExecutor);
    if (rs::isMsgTargetMarkerPosition(message)) {
        sead::Vector3f position = al::getSensorPos(this, "Stick05") + 50.0f * sead::Vector3f::ey;
        rs::setMsgTargetMarkerPosition(message, position);
        return true;
    }
    if (isNerveHackable()) {
        if (rs::isMsgCapEnableLockOn(message))
            return true;
        if (rs::isMsgStartHack(message)) {
            al::invalidateClipping(this);
            mPlayerHack = rs::startHack(self, other, nullptr);
            rs::startHackStartDemo(mPlayerHack, this);
            mDampingStrength = 0.0f;
            mDampingForce = 0.0f;
            mTouchForce = 0.0f;
            resetCapMtx(self);
            rs::setRouteHeadGuidePosPtr(this, &mHeadGuidePos);
            al::setNerve(this, &NrvHackFork.HackStartWait);
            al::startHitReaction(this, "ひょうい開始");
            return true;
        }
        if (rs::isMsgCapCancelLockOn(message))
            return true;
    }
    if (isHack()) {
        if (rs::isMsgHackerDamageAndCancel(message)) {
            if (al::isSensorName(self, "Body"))
                return rs::requestDamage(mPlayerHack);
            return false;
        }
        if (rs::isMsgHackSyncDamageVisibility(message)) {
            rs::syncDamageVisibility(this, mPlayerHack);
            return true;
        }

        if (rs::receiveMsgRequestTransferHack(message, mPlayerHack, other))
            return true;
        if (rs::isMsgCancelHack(message)) {
            rs::tryEndHackStartDemo(mPlayerHack, this);
            if (mIsHorizontal) {
                rs::endHack(&mPlayerHack);
                rs::resetRouteHeadGuidePosPtr(this);
                al::tryStartAction(this, "HackEnd");
                al::setNerve(this, &NrvHackFork.Damping);
                return true;
            }

            if (mIsHackBoard) {
                sead::Vector3f upDir;
                al::calcUpDir(&upDir, this);
                rs::endHackDir(&mPlayerHack, upDir);
            } else {
                sead::Vector3f front;
                if (al::isNearZero(mPullDirection))
                    front.set(-mPullDirection2);
                else
                    front.set(mPullDirection);
                front.y = 0.0f;
                al::tryNormalizeOrDirZ(&front);
                sead::Quatf quat;
                al::makeQuatUpFront(&quat, sead::Vector3f::ey, front);
                rs::endHackTargetQuat(&mPlayerHack, quat, front);
            }
            rs::resetRouteHeadGuidePosPtr(this);
            al::tryStartAction(this, "HackEnd");
            al::setNerve(this, &NrvHackFork.Damping);
            return true;
        }

        if (rs::isMsgHackMarioDemo(message) || rs::isMsgHackMarioDead(message)) {
            rs::endHack(&mPlayerHack);
            rs::resetRouteHeadGuidePosPtr(this);
            al::tryStartAction(this, "HackEnd");
            al::setNerve(this, &NrvHackFork.Damping);
            return true;
        }
    }
    return false;
}

void HackFork::initBasicPoseInfo() {
    al::calcUpDir(&mUpDir, this);

    mHackDir.set(al::getQuat(this));

    sead::Vector3f frontDir;
    al::calcFrontDir(&frontDir, this);

    sead::Quatf rotation;
    sead::QuatCalcCommon<f32>::setAxisAngle(rotation, frontDir, 180.0f);
    mUpsideDownInitialHackDir = rotation * mHackDir;

    sead::Vector3f frontDir2;
    al::calcFrontDir(&frontDir2, this);
    if (sead::Mathf::abs(frontDir2.y) > 0.5f) {
        al::invalidateShadow(this);
        mIsHorizontal = false;
    } else {
        mIsHorizontal = true;
    }
}

void HackFork::initAfterPlacement() {
    if (mMtxConnector) {
        sead::Vector3f frontDir;
        al::calcFrontDir(&frontDir, this);
        sead::Vector3f pos = al::getTrans(this) + frontDir * 100.0f;
        frontDir *= -200.0f;
        al::attachMtxConnectorToCollision(mMtxConnector, this, pos, frontDir);
    }
}

bool HackFork::tryTouch(f32 force, const char* reactionName) {
    if (mTouchDelay != 0) {
        mTouchDelay = 30;
        return false;
    }

    if (isNerveHackable()) {
        mTouchForce = force;
        mTouchDelay = 30;
        al::setNerve(this, &NrvHackFork.Damping);
        al::startHitReaction(this, reactionName);
        return true;
    }
    return false;
}

void HackFork::resetCapMtx(al::HitSensor* sensor) {
    calcHackDir(sensor);
    sead::Matrix34f jointMtx = *al::getJointMtxPtr(this, mJointName);
    al::normalize(&jointMtx);

    sead::Matrix34f poseMtx = jointMtx * mStartingPoseMtx;
    sead::Vector3f up = poseMtx.getBase(1);

    sead::Vector3f normal;
    if (mIsHorizontal)
        normal.set({0.0f, 1.0f, 0.0f});
    else
        normal = -mPullDirection2;

    al::verticalizeVec(&normal, up, normal);

    if (!al::tryNormalizeOrZero(&normal)) {
        mNextCapPoseMtx = mStartingPoseMtx;
    } else {
        sead::Matrix34f invPoseMtx;
        invPoseMtx.setInverse(poseMtx);

        normal.rotate(invPoseMtx);

        sead::Quatf quatRotation;
        al::makeQuatRotationRate(&quatRotation, sead::Vector3f::ez, normal, 1.0f);
        sead::Matrix34f quatRotationMtx;
        quatRotationMtx.fromQuat(quatRotation);

        sead::Matrix34f invJointMtx;
        invJointMtx.setInverse(jointMtx);

        mNextCapPoseMtx = (invJointMtx * poseMtx) * quatRotationMtx;
    }
    updateCapMtx();
}

bool HackFork::isNerveHackable() const {
    return al::isNerve(this, &NrvHackFork.Wait) || al::isNerve(this, &NrvHackFork.Damping);
}

bool HackFork::isHack() const {
    return al::isNerve(this, &NrvHackFork.HackStartWait) ||
           al::isNerve(this, &NrvHackFork.HackStart) || al::isNerve(this, &NrvHackFork.HackWait) ||
           al::isNerve(this, &NrvHackFork.HackBend) || al::isNerve(this, &NrvHackFork.HackShoot);
}

void HackFork::controlSpring() {
    f32 dampedForce = mDampingForce * 0.92f;
    mTouchForce += dampedForce * -0.5f;
    mDampingForce = dampedForce + mTouchForce;
}

void HackFork::checkSwing() {
    if (rs::isTriggerHackSwing(mPlayerHack)) {
        mIsHackSwing = true;
        mTimeSinceSwingStart = 0;
        return;
    }

    mTimeSinceSwingStart++;
}

bool HackFork::trySwingJump() {
    if (rs::isTriggerHackSwing(mPlayerHack)) {
        mIsHackSwing = true;
        mTimeSinceSwingStart = 0;
    } else {
        bool isHackSwing = mIsHackSwing;
        mTimeSinceSwingStart++;
        if (!isHackSwing || 10 < mTimeSinceSwingStart)
            return false;
    }

    if (mIsHorizontal) {
        mPullDirection.set({0.0f, -1.0f, 0.0f});
    } else {
        sead::Vector3f pulldirection = mPullDirection2;
        pulldirection.rotate(mHackDir * mInvInitialHackDir);
        pulldirection.y = 0;
        al::tryNormalizeOrDirZ(&pulldirection);
        mPullDirection.set(pulldirection);
    }
    mIsPullDown = mPullDirection.dot(mUpDir) < 0.0f;

    mDampingForce = 22.5f;
    sead::Vector3f frontDir;
    al::calcFrontDir(&frontDir, this);
    bendAndTwist(mPullDirection, frontDir);
    mIsSwingJump = true;
    al::setNerve(this, &NrvHackFork.HackShoot);
    return true;
}

bool HackFork::updateInput(sead::Vector3f* pullDirection, sead::Vector3f frontDir) {
    sead::Vector3f lookdir;
    al::calcCameraLookDir(&lookdir, this, 0);

    if (frontDir.dot(sead::Vector3f::ey) < sead::Mathf::cos(sead::Mathf::deg2rad(45.0f)) &&
        frontDir.dot(lookdir) > 0.0f)
        frontDir = -frontDir;

    sead::Vector3f moveDir = {0.0f, 0.0f, 0.0f};
    bool isMovePossible = rs::calcHackerMoveDir(&moveDir, mPlayerHack, frontDir);

    mInputBuffer.forcePushBack(moveDir);

    *pullDirection = {0.0f, 0.0f, 0.0f};
    for (s32 i = 0; i < mInputBuffer.size(); i++)
        *pullDirection += mInputBuffer[i];

    if (!al::tryNormalizeOrZero(pullDirection))
        al::calcUpDir(pullDirection, this);
    return isMovePossible;
}

f32 HackFork::getJumpRange() const {
    return mIsLimitterFree ? 180.0f : 45.0f;
}

void HackFork::bendAndTwist(const sead::Vector3f& direction, const sead::Vector3f& front) {
    mDampingForce = sead::Mathf::clampMax(mDampingForce + 1.0f, 22.5f);
    mTouchForce = 0.0f;

    mSideDir.setCross(front, direction);
    if (!al::tryNormalizeOrZero(&mSideDir))
        al::calcSideDir(&mSideDir, this);

    for (s32 i = 0; i < mJointRotationHolder.size(); i++)
        mJointRotationHolder[i]->setVector28(mSideDir);
}

// NON_MATCHING: Wrong loading order and registers https://decomp.me/scratch/8ReuA
void HackFork::shoot() {
    sead::Vector3f frontDir;
    al::calcFrontDir(&frontDir, this);
    sead::Quatf quat;
    al::makeQuatUpFront(&quat, frontDir, -mPullDirection);

    sead::Quatf rotationQuat;
    rotationQuat.makeVectorRotation(frontDir, sead::Vector3f::ey);
    quat = rotationQuat * quat;
    quat.normalize();

    sead::Vector3f jointPos;
    al::calcJointPos(&jointPos, this, "Stick05");

    sead::Vector3f launchDir;
    launchDir = mPullDirection;
    if (launchDir.y < -sead::Mathf::cos(sead::Mathf::deg2rad(110.0f)))
        launchDir += -sead::Vector3f::ey;
    al::normalize(&launchDir);

    f32 scale = mPullDirection.dot(-sead::Vector3f::ey);
    f32 cos = sead::Mathf::cos(sead::Mathf::deg2rad(45.0f));
    if (scale > cos) {
        scale = sead::Mathf::clamp((scale - cos) / (1.0f - cos), 0.0f, 1.0f);
        scale = (1.0f - sead::Mathf::square(1.0f - scale)) * -8.0f;
    } else {
        scale = -0.0f;
    }

    sead::Vector3f frontDir2;
    al::calcFrontDir(&frontDir2, this);
    frontDir2 *= scale;
    f32 launchSpeed = mIsSwingJump ? 61.27f : 55.7f;
    sead::Vector3f direction = frontDir2 - launchSpeed * launchDir;

    rs::endHackAirVelocity(&mPlayerHack, jointPos, quat, direction, mAirVel);
    rs::resetRouteHeadGuidePosPtr(this);
    al::startHitReaction(this, "ジャンプ");
    al::tryStartAction(this, "HackEnd");
}

void HackFork::control() {
    mHackStartShaderCtrl->update();
    if (mMtxConnector) {
        al::connectPoseQT(this, mMtxConnector);
        initBasicPoseInfo();
    }
    if (!isHack()) {
        if (mCameraTicket && al::isActiveCamera(mCameraTicket))
            al::endCamera(this, mCameraTicket, -1, false);
        if (al::isActiveCameraTarget(mMatrixCameraTarget))
            al::resetCameraTarget(this, mMatrixCameraTarget);
    }
    if (mTouchDelay != 0)
        mTouchDelay--;
    if (isHack()) {
        if (mIsHorizontal) {
            sead::Vector3f frontDir;
            al::calcFrontDir(&frontDir, this);
            frontDir.y = 0.0f;
            al::tryNormalizeOrDirZ(&frontDir);
            frontDir.negate();
            sead::Vector3f sideDir;
            sideDir.setCross(sead::Vector3f::ey, frontDir);
            mCameraTargetMtx.setBase(0, sideDir);
            mCameraTargetMtx.setBase(1, sead::Vector3f::ey);
            mCameraTargetMtx.setBase(2, frontDir);
        } else {
            sead::Vector3f upDir;
            al::calcUpDir(&upDir, this);
            upDir.y = 0.0;
            al::tryNormalizeOrDirZ(&upDir);
            sead::Vector3f sideDir;
            sideDir.setCross(sead::Vector3f::ey, upDir);
            if ((al::getCameraPos(this, 0) - al::getTrans(this)).dot(upDir) >= 0.0f) {
                upDir = -upDir;
                sideDir = -sideDir;
            }
            mCameraTargetMtx.setBase(0, sideDir);
            mCameraTargetMtx.setBase(1, sead::Vector3f::ey);
            mCameraTargetMtx.setBase(2, upDir);
        }

        sead::Vector3f cameraOffset;
        cameraOffset.setRotated(al::getQuat(this), mCameraOffset);
        if (mIsHorizontal) {
            sead::Vector3f frontDir;
            al::calcFrontDir(&frontDir, this);
            cameraOffset += frontDir * 100.0f;
        }
        mCameraTargetMtx.setBase(3, cameraOffset + al::getTrans(this));
        al::calcJointPos(&mHeadGuidePos, this, "Stick05");
        mHeadGuidePos.y = sead::Mathf::max(mHeadGuidePos.y, al::getTrans(this).y) + 100.0f;
    }
}

void HackFork::calcAnim() {
    al::LiveActor::calcAnim();
    if (isHack())
        updateCapMtx();
}

void HackFork::updateCapMtx() {
    sead::Matrix34f mtx = *al::getJointMtxPtr(this, mJointName);
    al::normalize(&mtx);
    mCapPoseMtx = mtx * mNextCapPoseMtx;
}

void HackFork::calcHackDir(al::HitSensor* sensor) {
    if (mIsHorizontal)
        mPullDirection2.set({0.0f, -1.0f, 0.0f});
    else
        mPullDirection2 = rs::getPlayerPos(this) - al::getSensorPos(sensor);

    sead::Vector3f frontDir;
    al::calcFrontDir(&frontDir, this);
    al::verticalizeVec(&mPullDirection2, frontDir, mPullDirection2);
    if (!al::tryNormalizeOrZero(&mPullDirection2))
        al::calcUpDir(&mPullDirection2, this);

    mInvInitialHackDir.setInverse(mHackDir);
}

void HackFork::exeWait() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Wait");
        al::showModelIfHide(this);
    }
    if (mEventFlowExecutor)
        rs::updateEventFlow(mEventFlowExecutor);
}

void HackFork::exeHackStartWait() {
    if (al::isFirstStep(this)) {
        if (mCameraTicket) {
            al::startCamera(this, mCameraTicket, -1);
            al::requestStopCameraVerticalAbsorb(this);
        }
        al::setCameraTarget(this, mMatrixCameraTarget);
        mIsHackSwing = false;
        mTimeSinceSwingStart = 0;
        mIsSwingJump = false;
        mAirVel = 0;
    }

    checkSwing();

    if (rs::isHackStartDemoEnterMario(mPlayerHack))
        al::setNerve(this, &NrvHackFork.HackStart);
}

void HackFork::exeDamping() {
    if (al::isFirstStep(this))
        al::showModelIfHide(this);

    if (al::isNearZero(mTouchForce) && al::isNearZero(mDampingForce)) {
        if (al::isNearZero(mDampingStrength)) {
            mDampingStrength = 0.0f;
            mDampingForce = 0.0f;
            mTouchForce = 0.0f;
            al::validateClipping(this);
            al::setNerve(this, &NrvHackFork.Wait);
            return;
        }
        mDampingStrength *= 0.9f;
        return;
    }
    controlSpring();
}

void HackFork::exeHackStart() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "HackStart");
        mHackStartShaderCtrl->start();
        mPullDirection = {0.0f, 0.0f, 0.0f};
    }
    checkSwing();
    if (al::isActionEnd(this)) {
        rs::endHackStartDemo(mPlayerHack, this);
        al::setNerve(this, &NrvHackFork.HackWait);
    }
}

void HackFork::exeHackWait() {
    controlSpring();
    mInputBuffer.clear();
    sead::Vector3f frontDir;
    al::calcFrontDir(&frontDir, this);
    sead::Vector3f pullDirection;
    if (!trySwingJump() && updateInput(&pullDirection, frontDir) &&
        sead::Mathf::abs(pullDirection.dot(mUpDir)) >
            sead::Mathf::cos(sead::Mathf::deg2rad(getJumpRange()))) {
        mPullDirection.set(pullDirection);
        mIsPullDown = mPullDirection.dot(mUpDir) < 0.0f;
        al::setNerve(this, &NrvHackFork.HackBend);
    }
}

void HackFork::exeHackBend() {
    if (al::isFirstStep(this))
        mIsReadyToShoot = false;

    if (al::isLessEqualStep(this, 15) && trySwingJump())
        return;
    sead::Vector3f frontDir;
    al::calcFrontDir(&frontDir, this);

    sead::Vector3f pullDirection;
    bool isInput = updateInput(&pullDirection, frontDir);
    if (!isInput || rs::isTriggerHackSwing(mPlayerHack)) {
        if (mIsReadyToShoot) {
            if (isInput) {
                mAirVel = 60;
                mIsSwingJump = true;
            }
            al::setNerve(this, &NrvHackFork.HackShoot);
        } else {
            al::setNerve(this, &NrvHackFork.HackWait);
        }
    }
    sead::Vector3f oldPullDirection = mPullDirection;
    f32 jumpDir = mIsPullDown ? -1.0f : 1.0f;

    f32 angle = sead::Mathf::clamp((jumpDir * mUpDir).dot(pullDirection), -1.0f, 1.0f);
    if (sead::Mathf::rad2deg(sead::Mathf::cos(angle)) < getJumpRange())
        mPullDirection = mPullDirection * 0.5f + pullDirection * 0.5f;
    al::normalize(&mPullDirection);
    f32 oldDampingForce = mDampingForce;
    bendAndTwist(mPullDirection, frontDir);
    f32 rumbleVolume = (mDampingForce - oldDampingForce) * 0.5f +
                       (mPullDirection - oldPullDirection).length() * 3.3f;
    if (!al::isNearZero(rumbleVolume)) {
        al::holdSe(this, "PgBendLv");
        sead::Vector3f jointPos;
        al::calcJointPos(&jointPos, this, "Stick03");
        al::PadRumbleParam param = al::PadRumbleParam(0.0f, 1300.0f, rumbleVolume, rumbleVolume);
        alPadRumbleFunction::startPadRumbleWithParam(this, jointPos, "パルス（中）", param, -1);
    }
    if (mDampingForce >= 4.5f)
        mIsReadyToShoot = true;
}

void HackFork::exeHackShoot() {
    controlSpring();
    if (mDampingForce < 0.0f) {
        shoot();
        al::setNerve(this, &NrvHackFork.Damping);
    }
}
