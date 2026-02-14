#include "Enemy/Bubble.h"

#include "Library/Bgm/BgmLineFunction.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/Collision/Collider.h"
#include "Library/Collision/CollisionParts.h"
#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Collision/KCollisionServer.h"
#include "Library/Collision/PartsInterpolator.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/Joint/JointSpringControllerHolder.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Movement/AnimScaleController.h"
#include "Library/Nature/NatureUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Play/Camera/ActorMatrixCameraTarget.h"
#include "Library/Rail/RailUtil.h"
#include "Library/Shadow/ActorShadowUtil.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

#include "Enemy/BubbleStateInLauncher.h"
#include "Enemy/DisregardReceiver.h"
#include "Player/IUsePlayerHack.h"
#include "Player/PlayerHackStartShaderCtrl.h"
#include "Util/CameraUtil.h"
#include "Util/CollisionUtil.h"
#include "Util/Hack.h"
#include "Util/InputInterruptTutorialUtil.h"
#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"
#include "Util/ShadowUtil.h"

namespace {
NERVE_IMPL(Bubble, StandBy)
NERVE_IMPL(Bubble, Delay)
NERVE_IMPL(Bubble, RailMove)
NERVE_IMPL(Bubble, Ready)
NERVE_IMPL(Bubble, Up)
NERVE_IMPL(Bubble, Turn)
NERVE_IMPL(Bubble, Down)
NERVE_IMPL(Bubble, Sink)
NERVE_IMPL(Bubble, Wait)
NERVE_IMPL(Bubble, Die)
NERVE_IMPL(Bubble, WaitHack)
NERVE_END_IMPL(Bubble, WaitHackStart)
NERVE_IMPL(Bubble, WaitHackFall)
NERVE_IMPL(Bubble, HackFall)
NERVE_END_IMPL(Bubble, HackMove)
NERVE_END_IMPL(Bubble, HackJump)
// NOTE: this jump is actually not as high as `HackJump` - it's just triggered by motion.
NERVE_END_IMPL_(Bubble, HackJumpHigh, HackJump)
NERVE_END_IMPL_(Bubble, HackCancelJump, HackJump)
NERVE_IMPL(Bubble, HackLand)
NERVE_END_IMPL(Bubble, HackInLauncher)
NERVE_IMPL(Bubble, HackResetPos)
NERVE_IMPL(Bubble, HackDemo)
NERVE_IMPL(Bubble, Revive)

NERVES_MAKE_STRUCT(Bubble, StandBy, HackInLauncher, Revive, WaitHackFall, Delay, Up, Turn, Down,
                   Die, HackJump, HackJumpHigh, HackCancelJump, HackFall, HackResetPos, HackDemo,
                   HackMove, WaitHack, WaitHackStart, Ready, Wait, Sink, HackLand, RailMove)
}  // namespace

static al::AnimScaleParam gAnimScaleParam(0.15f, 0.87f, 0.6f, 1.3f, -0.4f, 0.3f, 0.9f, 20, 0.25f,
                                          0.9f, 5.2f, 0.05f);
static PlayerHackStartShaderParam gPlayerHackStartShaderParam(true, 200.0f, 10, 20);

Bubble::Bubble(const char* name)
    : al::LiveActor(name), mAnimScaleController(new al::AnimScaleController(&gAnimScaleParam)) {}

void Bubble::init(const al::ActorInitInfo& info) {
    using BubbleFunctor = al::FunctorV0M<Bubble*, void (Bubble::*)()>;

    al::initActorWithArchiveName(this, info, "Bubble", nullptr);

    sead::Vector3f moveNext;
    if (al::tryGetLinksTrans(&moveNext, info, "MoveNext")) {
        mJumpHeight = sead::Mathf::abs(moveNext.y - al::getTrans(this).y);
        if (moveNext.y < al::getTrans(this).y)
            al::getTransPtr(this)->set(moveNext);
    } else {
        mJumpHeight = 800.0f;
    }

    if (al::isExistRail(this)) {
        al::setSyncRailToStart(this);
        mRailTotalLength = al::getRailTotalLength(this);

        if (!al::tryGetArg(&mRailMoveFrame, info, "RailMoveFrame") || mRailMoveFrame < 1) {
            f32 railMoveSpeed = 10.0f;
            al::tryGetArg(&railMoveSpeed, info, "RailMoveSpeed");
            if (railMoveSpeed <= 0.0f)
                railMoveSpeed = 10.0f;
            mRailMoveFrame = sead::Mathf::ceil(mRailTotalLength / railMoveSpeed);
        }
    }

    mStartActionPosition = al::getTrans(this);
    mStartingRotation.set(al::getQuat(this));
    mRevivePosition.set(al::getTrans(this));
    mReviveStartActionPosition.set(mStartActionPosition);
    mTurnFrameDelay = mJumpHeight / 13.0f;
    recalcClippingInfo();

    al::tryGetArg(&mWaitFrameNum, info, "WaitFrameNum");
    if (mWaitFrameNum < 0)
        mWaitFrameNum = 0;

    al::tryGetArg(&mDelayFrameNum, info, "DelayFrameNum");
    if (mDelayFrameNum < 0)
        mDelayFrameNum = 0;

    al::tryGetArg(&mIsWaveCheckOn, info, "IsWaveCheckOn");
    mReviveDelayTime = mTurnFrameDelay * 2 + mWaitFrameNum + 80;

    if (al::isExistRail(this)) {
        f32 travelTime = mRailMoveFrame + mReviveDelayTime + 1;
        mReviveDelayTime = travelTime * 2;
    }

    f32 shootDegree;
    al::tryGetArg(&shootDegree, info, "ShootDegree");
    al::initNerve(this, &NrvBubble.StandBy, 1);
    mBubbleStateInLauncher = new BubbleStateInLauncher(this);
    al::initNerveState(this, mBubbleStateInLauncher, &NrvBubble.HackInLauncher, "バブルランチャー");
    al::hideModel(this);

    if (!al::listenStageSwitchOnStart(this, BubbleFunctor(this, &Bubble::start)))
        start();

    al::listenStageSwitchOnKill(this, BubbleFunctor(this, &Bubble::killAll));
    mCapTargetInfo = rs::createCapTargetInfo(this, nullptr);

    bool stageStartHack = false;
    al::tryGetArg(&stageStartHack, info, "StageStartHack");
    if (!stageStartHack) {
        al::PlacementInfo placementInfo;
        if (al::tryGetLinksInfo(&placementInfo, info, "MoveNext"))
            al::tryGetArg(&stageStartHack, placementInfo, "StageStartHack");
    }

    if (stageStartHack)
        rs::requestStageStartHack(this, al::getHitSensor(this, "Cap"), mCapTargetInfo, nullptr);

    mJointSpringControllerHolder =
        al::JointSpringControllerHolder::tryCreateAndInitJointControllerKeeper(this, "Dynamics");
    mJointSpringControllerHolder->offControlAll();
    al::setEffectNamedMtxPtr(this, "LavaSurface", &mLavaSurfaceMtx);
    makeActorAlive();
    al::offCollide(this);
    mActorMatrixCameraTarget = al::createActorMatrixCameraTarget(this, &mCameraMtx);

    al::hideSilhouetteModelIfShow(this);
    rs::initHackShadow(this);

    auto* triangleFilter = new al::TriangleFilterDelegator<Bubble>(this, &Bubble::isIgnoreTriangle);
    al::setColliderFilterTriangle(this, triangleFilter);
    al::setColliderReactMovePower(this, false);

    al::invalidateColliderRobustCheck(this);
    mCollisionPartsFilter = new al::CollisionPartsFilterSpecialPurpose("Bubble");

    al::setColliderFilterCollisionParts(this, mCollisionPartsFilter);
    mClippingProbeActor = new al::LiveActor("グループ同期");

    al::initActorWithArchiveName(mClippingProbeActor, info, "Bubble", "Probe");

    al::setClippingInfo(mClippingProbeActor, al::getClippingRadius(this), &mClippingPos);
    mClippingProbeActor->makeActorAlive();

    if (al::isExistRail(this) && al::isExistLinkChild(info, "TurnTarget", 0)) {
        sead::Vector3f trans;
        al::tryGetLinksQT(&mRailTargetRotation, &trans, info, "TurnTarget");
    } else {
        mRailTargetRotation.set(mStartingRotation);
    }

    mShadowMaskOffset.set(al::getShadowMaskOffset(this, "body"));
    mShadowMaskDropLength = al::getShadowMaskDropLength(this, "body");
    mDisregardReceiver = new DisregardReceiver(this, nullptr);
    mPlayerHackStartShaderCtrl = new PlayerHackStartShaderCtrl(this, &gPlayerHackStartShaderParam);

    al::startAction(this, "Wait");
    al::startMtsAnim(this, "Scroll");
}

void Bubble::initAfterPlacement() {
    checkEffectPos();
    if (!isValidCollisionOrWaveCheck())
        al::tryUpdateEffectMaterialCode(this, mMaterialCode);
}

void Bubble::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isNerve(this, &NrvBubble.Die) || al::isNerve(this, &NrvBubble.Revive))
        return;

    if (al::isSensorEnemyBody(self)) {
        if (isHack() && mPlayerHack)
            rs::sendMsgHackerNoReaction(mPlayerHack, other, self);
        return;
    }

    if (!al::isSensorEnemyAttack(self)) {
        if (al::isSensorName(self, "Cap") || al::isSensorName(self, "CapAbove") ||
            al::isSensorName(self, "CapBelow")) {
            return;
        }

        if (al::isSensorName(self, "PechoAttack"))
            rs::sendMsgBubbleAttackToPecho(other, self);
        return;
    }

    if (!al::isNerve(this, &NrvBubble.HackJump) && !al::isNerve(this, &NrvBubble.HackJumpHigh) &&
        !al::isNerve(this, &NrvBubble.HackCancelJump) && !al::isNerve(this, &NrvBubble.HackFall) &&
        rs::sendMsgBubbleReflectH(other, self)) {
        sead::Vector3f distance = al::getSensorPos(self) - al::getSensorPos(other);
        distance.y = 0.0f;
        al::tryNormalizeOrZero(&distance);
        f32 travel = al::getVelocity(this).dot(distance);
        if (travel < 0.0f)
            al::addVelocity(this, -distance * travel);
        al::addVelocity(this, distance * 15.0f);
        mAnimScaleController->startHitReaction();
        return;
    }

    if (isHack() || al::isNerve(this, &NrvBubble.WaitHackFall)) {
        if (rs::sendMsgBubbleAttack(other, self))
            return;

        if (al::sendMsgEnemyAttackFire(other, self, "LavaRed"))
            return;

        if (al::isNerve(this, &NrvBubble.HackFall) ||
            al::isNerve(this, &NrvBubble.HackCancelJump) ||
            al::isNerve(this, &NrvBubble.HackJump) || al::isNerve(this, &NrvBubble.HackJumpHigh)) {
            if (al::getVelocity(this).y > 0.0f) {
                if (!al::isExistCollisionParts(al::getSensorHost(other)) &&
                    al::sendMsgEnemyUpperPunch(other, self)) {
                    al::setVelocityZeroV(this);
                }
            } else if (rs::sendMsgBubbleReflectV(other, self)) {
                mJumpForce.set(0.0f, 15.0f, 0.0f);
                al::setVelocityZeroV(this);
                mAnimScaleController->startHitReaction();
                al::setNerve(this, &NrvBubble.HackJump);
                return;
            }
        }
    }

    if (al::isNerve(this, &NrvBubble.StandBy) || al::isNerve(this, &NrvBubble.Wait) ||
        al::isNerve(this, &NrvBubble.Delay) || al::isNerve(this, &NrvBubble.Die) ||
        al::isNerve(this, &NrvBubble.RailMove) || al::isNerve(this, &NrvBubble.Ready)) {
        return;
    }

    if (!al::isNerve(this, &NrvBubble.WaitHackFall) &&
        (rs::sendMsgBubbleAttack(other, self) ||
         al::sendMsgEnemyAttackFire(other, self, "LavaRed") ||
         al::sendMsgEnemyAttack(other, self))) {
        return;
    }

    if (!isHack() && al::sendMsgPush(other, self))
        return;

    rs::sendMsgPushToPlayer(other, self);
}

bool Bubble::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    if (rs::tryReceiveMsgInitCapTargetAndSetCapTargetInfo(message, mCapTargetInfo))
        return true;

    if (rs::isMsgEnableMapCheckPointWarp(message))
        return isOnGround();

    if (mDisregardReceiver->receiveMsg(message, other, self))
        return true;

    if (rs::isMsgEndInSaucePan(message)) {
        mIsPlayerCaptured = false;
        mIsInSaucePan = false;
        return true;
    }

    if (isHack()) {
        if (!al::isNerve(this, &NrvBubble.HackResetPos) &&
            rs::tryGetBossMagmaResetPos(message, &mResetTargetPos)) {
            al::setNerve(this, &NrvBubble.HackResetPos);
            return true;
        }

        if (rs::isMsgBossMagmaCatchPlayer(message)) {
            mIsPlayerCaptured = true;
            mIsInBossSequence = true;
            return true;
        }

        if (rs::isMsgBossMagmaReleasePlayer(message)) {
            mIsPlayerCaptured = false;
            return true;
        }

        if (rs::isMsgBossMagmaDeadDemoStart(message)) {
            al::hideModelIfShow(this);
            al::hideSilhouetteModelIfShow(this);
            hideHackCap();
            al::tryKillEmitterAndParticleAll(this);
            al::setNerve(this, &NrvBubble.HackDemo);
            return true;
        }

        if (rs::isMsgBossMagmaDeadDemoEnd(message)) {
            sead::Vector3f endTargetPos;
            rs::tryGetBossMagmaDeadDemoEndTargetPos(message, &endTargetPos);
            sead::Vector3f firePos;
            sead::Vector3f fireNormal;
            if (al::calcFindFireSurface(&firePos, &fireNormal, this, mResetTargetPos,
                                        sead::Vector3f::ey, 800.0f)) {
                sead::Vector3f resetPos = mResetTargetPos;
                resetPos.y = firePos.y;
                al::resetPosition(this, resetPos);
                al::faceToTarget(this, endTargetPos);
                mJumpForce.set(0.0f, 0.0f, 0.0f);
                al::setVelocityZero(this);
                mIsOnLavaSurface = true;
                al::tryUpdateEffectMaterialCode(this, al::getFireMaterialCode(this));
                mIsInBossSequence = false;
                mFireNormal.set(fireNormal);
                al::setNerve(this, &NrvBubble.HackMove);
            } else {
                mJumpForce.set(0.0f, 0.0f, 0.0f);
                al::setVelocityZero(this);
                al::setNerve(this, &NrvBubble.HackJump);
            }
            al::showModel(this);
            al::showSilhouetteModel(this);
            showHackCap();

            return true;
        }

        if (rs::isMsgBossMagmaQueryToBubble(message))
            return !mIsOnLavaSurface;
    }

    if (al::isSensorEnemyAttack(self))
        return false;

    if (rs::isMsgHackDirectStageInit(&mPlayerHack, message)) {
        if (!al::isSensorName(self, "Cap") && !al::isSensorName(self, "CapAbove") &&
            !al::isSensorName(self, "CapBelow")) {
            return false;
        }

        al::resetPosition(this,
                          al::getTrans(this) + al::getColliderRadius(this) * sead::Vector3f::ey);
        setupHack();
        rs::setupHackShadow(this);
        al::setNerve(this, &NrvBubble.HackFall);
        return true;
    }

    if (rs::isMsgCapEnableLockOn(message)) {
        if (!isCurrentNerveEnableLockOnAndStartHack())
            return false;

        if (!al::isSensorName(self, "Cap") && !al::isSensorName(self, "CapAbove") &&
            !al::isSensorName(self, "CapBelow")) {
            return false;
        }

        al::setNerve(this, &NrvBubble.WaitHack);
        return true;
    }

    if (rs::isMsgCapCancelLockOn(message)) {
        if (!isCurrentNerveEnableLockOnAndStartHack())
            return false;

        if (!al::isSensorName(self, "Cap") && !al::isSensorName(self, "CapAbove") &&
            !al::isSensorName(self, "CapBelow"))
            return false;

        al::invalidateClipping(this);
        offGroupClipping();
        mJumpForce.set(0.0f, 0.0f, 0.0f);
        al::onCollide(this);
        mWaitReviveDelay = 5;
        al::setVelocityZero(this);
        al::setNerve(this, &NrvBubble.WaitHackFall);
        return true;
    }

    if (rs::isMsgStartHack(message)) {
        if (!isCurrentNerveEnableLockOnAndStartHack())
            return false;

        if (!al::isSensorName(self, "Cap") && !al::isSensorName(self, "CapAbove") &&
            !al::isSensorName(self, "CapBelow")) {
            return false;
        }

        startHackLocal(self, other);
        return true;
    }

    if (rs::isMsgHackInvalidEscapeNoReaction(message)) {
        if (mCancelLaunchCooldown != 0)
            return true;

        if (al::isNerve(this, &NrvBubble.HackInLauncher))
            return mBubbleStateInLauncher->isWaiting();

        return false;
    }

    if (rs::isMsgHackInvalidEscape(message)) {
        if (!isHack())
            return false;

        if (!al::isInAreaObj(this, "HackInvalidEscapeArea") &&
            !al::isNerve(this, &NrvBubble.HackResetPos))
            return false;

        return true;
    }

    if (rs::isMsgCancelHack(message) || rs::isMsgCancelHackByDokan(message)) {
        prepareEndHack();
        endHackByCancel();
        return true;
    }

    if (rs::isMsgHackMarioCheckpointFlagWarp(message)) {
        prepareEndHack();
        endHackByCancel();
        al::startVisAnim(this, "CapOff");
        al::startMtpAnim(this, "CapOff");
        return true;
    }

    if (rs::isMsgHackMarioDemo(message)) {
        al::tryKillEmitterAndParticleAll(this);
        forceEndHack();
        return true;
    }

    if (rs::isMsgHackMarioDead(message)) {
        forceEndHack();
        return true;
    }

    if (rs::isMsgHackSyncDamageVisibility(message)) {
        if (al::isNerve(this, &NrvBubble.HackInLauncher) || al::isNerve(this, &NrvBubble.HackDemo))
            return false;

        rs::syncDamageVisibility(this, mPlayerHack);
        return true;
    }

    sead::Vector3f breathForce;
    if (rs::tryGetBossMagmaBreathForce(message, &breathForce)) {
        if (!al::isSensorName(self, "Cap"))
            return false;

        sead::Vector3f force = breathForce;
        if (al::tryNormalizeOrZero(&force)) {
            f32 forceLength = force.dot(al::getVelocity(this));
            if (breathForce.length() > forceLength)
                al::addVelocity(this, breathForce * 0.05f);
        }
        return true;
    }

    if (rs::isMsgBubbleLauncherStart(message)) {
        if (!al::isSensorName(self, "Body"))
            return false;

        if (al::isNerve(this, &NrvBubble.HackJump) || al::isNerve(this, &NrvBubble.HackJumpHigh) ||
            al::isNerve(this, &NrvBubble.HackCancelJump)) {
            al::setNerve(this, &NrvBubble.HackInLauncher);
            return true;
        }

        if (al::isNerve(this, &NrvBubble.HackInLauncher) && mPlayerHack) {
            al::setNerve(this, &NrvBubble.HackInLauncher);
            return true;
        }
        return false;
    }

    if (isHack()) {
        if (!al::isNerve(this, &NrvBubble.HackInLauncher) &&
            !al::isNerve(this, &NrvBubble.WaitHackStart) &&
            !al::isNerve(this, &NrvBubble.HackResetPos)) {
            if (rs::tryReceiveMsgPushToPlayerAndAddVelocityH(this, message, other, self, 3.0f))
                return true;

            if (rs::isMsgBubbleAttack(message)) {
                al::pushAndAddVelocityH(this, other, self, 3.0f);
                return true;
            }
        }

        if (rs::isMsgHackerDamageAndCancel(message)) {
            if (rs::isMsgFireDamageAll(message))
                return false;

            if (rs::isMsgEnemyAttackFireCollision(message))
                return false;

            if (al::isMsgEnemyAttack(message)) {
                if (rs::sendMsgBubbleAttackToPecho(other, self))
                    return false;

                if (rs::sendMsgBubbleAttack(other, self))
                    return false;
            }
            return rs::requestDamage(mPlayerHack);
        }
        return false;
    }

    if (!al::isNoCollide(this) &&
        al::tryReceiveMsgPushAndAddVelocityH(this, message, other, self, 3.0f))
        return true;

    if (rs::isMsgKillByShineGet(message) || rs::isMsgKillByHomeDemo(message) ||
        rs::isMsgKillByMoonRockDemo(message)) {
        al::tryKillEmitterAndParticleAll(this);
        revive();
        return true;
    }

    if (rs::isMsgHammerBrosHammerHackAttack(message) ||
        rs::isMsgHammerBrosHammerEnemyAttack(message)) {
        if (!al::isNerve(this, &NrvBubble.Up) && !al::isNerve(this, &NrvBubble.Turn) &&
            !al::isNerve(this, &NrvBubble.Down))
            return false;

        rs::requestHitReactionToAttacker(message, self, other);
        return true;
    }
    return false;
}

inline s32 getSurfaceProbeIndex(Bubble* bubble) {
    s32 index = 0;
    if (al::isExistRail(bubble) && !al::isRailGoingToEnd(bubble))
        index = 1;
    return index;
}

void Bubble::control() {
    mDisregardReceiver->set44(al::getSensorPos(this, "Body") +
                              al::getSensorRadius(this, "Body") * sead::Vector3f::ey);

    if (isValidCollisionOrWaveCheck()) {
        if (mIsOnLavaSurface) {
            mLavaSurfaceMtx.makeQT(al::getQuat(this), {0.0f, 0.0f, 0.0f});
            mLavaSurfaceMtx.setTranslation(al::getTrans(this));
        } else if (al::isOnGround(this, 0)) {
            mLavaSurfaceMtx.makeQT(al::getQuat(this), {0.0f, 0.0f, 0.0f});

            sead::Vector3f trans = al::getTrans(this);
            trans.y = al::getCollidedGroundPos(this).y;
            mLavaSurfaceMtx.setTranslation(trans);
        }
    } else {
        mLavaSurfaceMtx.makeQT(al::getQuat(this), {0.0f, 0.0f, 0.0f});
        getSurfaceProbeIndex(this);
        al::getTrans(this);
        mLavaSurfaceMtx.setTranslation(al::getTrans(this));
    }

    if (!isHack() && !al::isNerve(this, &NrvBubble.Revive) &&
        (al::isInWater(this) || al::isInDeathArea(this))) {
        al::setVelocityZero(this);
        al::startHitReaction(this, "死亡");
        revive();
    }

    if (mCancelLaunchCooldown != 0)
        mCancelLaunchCooldown--;

    if (isHack() && !al::isActiveCameraTarget(mActorMatrixCameraTarget))
        al::setCameraTarget(this, mActorMatrixCameraTarget);

    if (isHack() && !isWaitingLaunch() && !al::isHideModel(this))
        al::showSilhouetteModelIfHide(this);
    else
        al::hideSilhouetteModelIfShow(this);

    if (isHack() || al::isNerve(this, &NrvBubble.WaitHackFall))
        updateLavaWave();
    else if (!isValidCollisionOrWaveCheck())
        mIsOnLavaSurface = false;

    if (isHack()) {
        if (!al::isVisAnimPlaying(this, "CapOn"))
            al::startVisAnim(this, "CapOn");
        if (!al::isMtpAnimPlaying(this, "CapOn"))
            al::startMtpAnim(this, "CapOn");
    } else {
        if (!al::isVisAnimPlaying(this, "CapOff"))
            al::startVisAnim(this, "CapOff");
        if (!al::isMtpAnimPlaying(this, "CapOff"))
            al::startMtpAnim(this, "CapOff");
    }

    updateScrollAnimRate();
    mAnimScaleController->update();
    al::setScale(this, mAnimScaleController->getScale());

    if (isHack() && isOnGround())
        mIsHackOnGround = true;
    else
        mIsHackOnGround = false;

    if (!al::isNerve(this, &NrvBubble.StandBy) && !al::isNerve(this, &NrvBubble.Delay) &&
        (!isHack() || !rs::isActiveHackStartDemo(mPlayerHack)) &&
        !al::isClipped(mClippingProbeActor) && !mIsClipped) {
        mReviveDelayCount = al::modi(mReviveDelayCount + 1 + mReviveDelayTime, mReviveDelayTime);
    }

    mIsClipped = al::isClipped(mClippingProbeActor);
    if (!isHack())
        al::scaleVelocityHV(this, 0.9f, 1.0f);
    mCameraMtx.makeQT(mGroundRotation, {0.0f, 0.0f, 0.0f});

    mCameraMtx.setTranslation(al::getTrans(this) + (0.0f * sead::Vector3f::ey));
    if (al::isNerve(this, &NrvBubble.Up) || al::isNerve(this, &NrvBubble.Turn) ||
        al::isNerve(this, &NrvBubble.Down)) {
        mDisregardReceiver->setEDC(false, false, false);
    } else {
        mDisregardReceiver->setEDC(false, false, true);
    }

    if (mHackTurnFrame < mHackTurnDelay)
        mHackTurnFrame++;
}

void Bubble::updateCollider() {
    if (al::isNoCollide(this)) {
        al::getTransPtr(this)->add(al::getVelocity(this));
        getCollider()->onInvalidate();
    } else {
        if (isHack())
            rs::solveCollisionInHacking(this, al::getVelocity(this) + mColliderGroundOffset);
        else
            al::getTransPtr(this)->add(
                getCollider()->collide(al::getVelocity(this) + mColliderGroundOffset));
    }

    mColliderGroundOffset.set(0.0f, 0.0f, 0.0f);
}

void Bubble::appear() {
    al::LiveActor::appear();
    al::setNerve(this, &NrvBubble.StandBy);
    al::startVisAnim(this, "CapOff");

    if (!al::isValidSwitchStart(this))
        start();
}

void Bubble::calcAnim() {
    al::LiveActor::calcAnim();

    sead::Matrix34f mtx;
    mtx.setInverse(*getBaseMtx());
    sead::Vector3f offset = mShadowMaskOffset;
    offset.rotate(mtx);
    al::setShadowMaskOffset(this, offset, "body");
}

void Bubble::recalcClippingInfo() {
    sead::Vector3f max = mStartActionPosition;
    sead::Vector3f min = mStartActionPosition;
    al::updateBoundingBox(mJumpHeight * sead::Vector3f::ey + mStartActionPosition, &max, &min);

    if (al::isExistRail(this)) {
        s32 pointNum = al::getRailPointNum(this);
        for (s32 i = 0; i < pointNum; i++) {
            sead::Vector3f pos;
            al::calcRailPointPos(&pos, this, i);
            al::updateBoundingBox(pos, &max, &min);
        }

        f32 prevRailCoord = al::calcRailCoordByPoint(this, 0);
        for (s32 i = 1; i < pointNum; i++) {
            f32 currentRailCoord = al::calcRailCoordByPoint(this, i);
            sead::Vector3f pos;
            al::calcRailPosAtCoord(&pos, this, (prevRailCoord + currentRailCoord) * 0.5f);
            al::updateBoundingBox(pos, &max, &min);
            prevRailCoord = currentRailCoord;
        }
    }

    mClippingPos = (max + min) * 0.5f;
    al::setClippingInfo(this, (max - min).length() * 0.5f + 200.0f, &mClippingPos);
}

void Bubble::start() {
    if (!al::isNerve(this, &NrvBubble.StandBy))
        return;

    al::setTrans(this, mStartActionPosition);
    al::hideModelIfShow(this);
    mReviveDelayCount = 0;

    if (mDelayFrameNum == 0)
        al::setNerve(this, &NrvBubble.Ready);
    else
        al::setNerve(this, &NrvBubble.Delay);
}

void Bubble::killAll() {
    al::setNerve(this, &NrvBubble.Die);
}

bool Bubble::isIgnoreTriangle(const al::Triangle& triangle) const {
    return al::isFloorCode(triangle, "Fence");
}

void Bubble::checkEffectPos() {
    if (!mIsCheckEffectPosRequired || al::isNerve(this, &NrvBubble.StandBy) || mIsWaveCheckOn)
        return;

    al::Triangle triangle;
    if (!alCollisionUtil::getFirstPolyOnArrow(
            this, mSurfaceProbePos, &triangle, al::getTrans(this) + 100.0f * sead::Vector3f::ey,
            -200.0f * sead::Vector3f::ey, mCollisionPartsFilter, nullptr)) {
        return;
    }

    mMaterialCode = al::getMaterialCodeName(triangle);
    if (!al::isExistRail(this)) {
        mIsCheckEffectPosRequired = false;
        return;
    }

    sead::Vector3f railPos;
    al::calcRailPosAtCoord(&railPos, this, al::getRailTotalLength(this));

    // BUG: missing `mCollisionPartsFilter` as parameter
    if (alCollisionUtil::getFirstPolyOnArrow(this, &mSurfaceProbePos[1], nullptr,
                                             railPos + 100.0f * sead::Vector3f::ey,
                                             -200.0f * sead::Vector3f::ey, nullptr, nullptr)) {
        mIsCheckEffectPosRequired = false;
    }
}

bool Bubble::isValidCollisionOrWaveCheck() const {
    return !al::isNoCollide(this) || mIsWaveCheckOn;
}

bool Bubble::isHack() const {
    return (al::isNerve(this, &NrvBubble.WaitHackStart) &&
            rs::isHackStartDemoEnterMario(mPlayerHack)) ||
           al::isNerve(this, &NrvBubble.HackFall) || al::isNerve(this, &NrvBubble.HackMove) ||
           al::isNerve(this, &NrvBubble.HackJump) || al::isNerve(this, &NrvBubble.HackJumpHigh) ||
           al::isNerve(this, &NrvBubble.HackCancelJump) || al::isNerve(this, &NrvBubble.HackLand) ||
           al::isNerve(this, &NrvBubble.HackResetPos) || al::isNerve(this, &NrvBubble.HackDemo) ||
           (al::isNerve(this, &NrvBubble.HackInLauncher) && mPlayerHack);
}

void Bubble::revive() {
    if (al::isNerve(this, &NrvBubble.Revive))
        return;

    if (al::isExistRail(this)) {
        al::setRailPosToStart(this);
        if (!al::isRailGoingToEnd(this))
            al::reverseRail(this);
    }

    al::offCollide(this);
    al::validateClipping(this);
    onGroupClipping();
    al::hideModelIfShow(this);
    al::setVelocityZero(this);
    al::setNerve(this, &NrvBubble.Revive);
}

// Hack to make updateLavaWave match
inline void x1(sead::Vector3f* p, const sead::Vector3f& v) {
    __asm("");
    p->set(v);
}

void Bubble::updateLavaWave() {
    sead::Vector3f negColliderBottomOffset =
        (al::getColliderRadius(this) - al::getColliderOffsetY(this)) * sead::Vector3f::ey;

    sead::Vector3f colliderPos = al::getTrans(this) - negColliderBottomOffset;
    sead::Vector3f firePos = colliderPos - 200.0f * sead::Vector3f::ey;

    bool isInFire = al::isInFirePos(this, firePos);

    sead::Vector3f fireNormal;
    al::calcFindFireSurface(&firePos, &fireNormal, this, colliderPos, sead::Vector3f::ey, 200.0f);
    firePos.x = colliderPos.x;
    firePos.z = colliderPos.z;

    if (al::isOnGround(this, 0)) {
        if (!isInFire || firePos.y < al::getTrans(this).y - al::getColliderRadius(this) +
                                         al::getColliderOffsetY(this)) {
            mIsOnLavaSurface = false;
            return;
        }
    }

    // BUG: 'firePos + negColliderBottomOffset' should be 'firePos - negColliderBottomOffset
    if (isEnableSnapWaveSurface()) {
        if (isInFire) {
            x1(al::getTransPtr(this), firePos + negColliderBottomOffset);
            mFireNormal.set(fireNormal);
            al::tryUpdateEffectMaterialCode(this, al::getFireMaterialCode(this));
            return;
        }
    } else if (isInFire && firePos.y >= colliderPos.y) {
        x1(al::getTransPtr(this), firePos + negColliderBottomOffset);

        mIsOnLavaSurface = true;
        mFireNormal.set(fireNormal);
        al::tryUpdateEffectMaterialCode(this, al::getFireMaterialCode(this));
        return;
    }

    mIsOnLavaSurface = false;
}

void Bubble::updateScrollAnimRate() {
    if (al::isActionPlaying(this, mCurrentActionName))
        return;

    mCurrentActionName = al::getActionName(this);

    if (isHack() &&
        (al::isActionPlaying(this, "BubbleCannonJump") ||
         al::isActionPlaying(this, "HackHighJump") || al::isActionPlaying(this, "HackWaitSpeedy") ||
         al::isActionPlaying(this, "MoveSpeedy"))) {
        al::setMtsAnimFrameRate(this, 2.0f);
    } else
        al::setMtsAnimFrameRate(this, 1.0f);
}

bool Bubble::isOnGround() const {
    return al::isOnGround(this, 0) || mIsOnLavaSurface;
}

void Bubble::setupHack() {
    if (!isValidCollisionOrWaveCheck()) {
        f32 probePos = mSurfaceProbePos[getSurfaceProbeIndex(this)].y + al::getColliderRadius(this);
        if (al::getTrans(this).y < probePos)
            al::getTransPtr(this)->y = probePos;
    }

    al::invalidateClipping(this);
    offGroupClipping();
    mJumpForce = {0.0f, 0.0f, 0.0f};
    al::onCollide(this);
    mWaitReviveDelay = 5;
    al::setVelocityZero(this);
    mIsPlayerCaptured = false;
    mJointSpringControllerHolder->onControlAll();
}

bool Bubble::isCurrentNerveEnableLockOnAndStartHack() const {
    if (al::isHideModel(this))
        return false;

    if (al::isNerve(this, &NrvBubble.Up)) {
        if (al::getTrans(this).y - mStartActionPosition.y > al::getColliderRadius(this))
            return true;
    }

    if (al::isNerve(this, &NrvBubble.Turn))
        return true;

    if (al::isNerve(this, &NrvBubble.Down) &&
        al::getTrans(this).y - mStartActionPosition.y > al::getColliderRadius(this))
        return true;

    return al::isNerve(this, &NrvBubble.WaitHack) || al::isNerve(this, &NrvBubble.WaitHackFall);
}

void Bubble::offGroupClipping() {
    if (mIsOnGroupClipping) {
        al::offGroupClipping(this);
        mIsOnGroupClipping = false;
    }
}

void Bubble::startHackLocal(al::HitSensor* self, al::HitSensor* other) {
    setupHack();
    mPlayerHack = rs::startHack(self, other, nullptr);
    rs::startHackStartDemo(mPlayerHack, this);
    al::setCameraTarget(this, mActorMatrixCameraTarget);
    al::setNerve(this, &NrvBubble.WaitHackStart);
}

bool Bubble::isInvalidHackEscape() const {
    if (isHack()) {
        if (al::isInAreaObj(this, "HackInvalidEscapeArea"))
            return true;

        if (al::isNerve(this, &NrvBubble.HackResetPos))
            return true;
    }

    return false;
}

void Bubble::prepareEndHack() {
    al::startHitReaction(this, "憑依解除(通常)");
    rs::tryEndHackStartDemo(mPlayerHack, this);
    al::showModelIfHide(this);
}

void Bubble::endHackByCancel() {
    al::endBgmSituation(this, "HackBubble", false);

    if (mLaunchCameraTicket && al::isActiveCamera(mLaunchCameraTicket)) {
        al::endCamera(this, mLaunchCameraTicket, -1, false);
        mLaunchCameraTicket = nullptr;
    }

    endHackCommon();
    if (al::isNerve(this, &NrvBubble.HackInLauncher) && mBubbleStateInLauncher->isFlying())
        return;

    al::setNerve(this, &NrvBubble.WaitHackFall);
}

void Bubble::forceEndHack() {
    if (mPlayerHack) {
        al::startHitReaction(this, "憑依解除(着地強制)");
        endHackCommon();
    }

    al::setVelocityZero(this);
    revive();
}

void Bubble::updatePosOnWave() {
    if (isValidCollisionOrWaveCheck())
        updateLavaWave();
}

void Bubble::updateVelocityIfValidCollision() {
    if (!isValidCollisionOrWaveCheck())
        return;

    updateLavaWave();
    sead::Vector3f velocity;
    if (al::isOnGroundNoVelocity(this, 0))
        velocity.set(al::getOnGroundNormal(this, 0));
    else
        velocity.set(sead::Vector3f::ey);

    sead::Vector3f velocityH;
    sead::Vector3f velocityV;
    al::separateVelocityDirHV(&velocityH, &velocityV, this, velocity);
    velocityH *= 0.92f;
    if (al::isOnGroundNoVelocity(this, 0))
        velocityV.set(0.0f, 0.0f, 0.0f);

    velocityV -= 1.1f * velocity;
    al::setVelocity(this, velocityH + velocityV);
}

bool Bubble::isOnDamageFire() const {
    return mIsOnLavaSurface || al::isCollidedGroundFloorCode(this, "DamageFire");
}

void Bubble::tryStartHitReactionUp() {
    if (mIsOnHitReactionEffect || mIsCheckEffectPosRequired)
        return;

    s32 index = getSurfaceProbeIndex(this);

    if (mSurfaceProbePos[index].y < al::getTrans(this).y) {
        al::startHitReactionHitEffect(this, "発射",
                                      mSurfaceProbePos[index] + 0.0f * sead::Vector3f::ey);
        mIsOnHitReactionEffect = true;
    }
}

void Bubble::updateShadowMaskLength() {
    f32 newLength = sead::Mathf::clamp(al::getTrans(this).y - mStartActionPosition.y + 300.0f,
                                       300.0f, mShadowMaskDropLength);
    al::setShadowMaskDropLength(this, newLength);
}

void Bubble::tryStartHitReactionDown() {
    if (mIsOnHitReactionEffect || mIsCheckEffectPosRequired)
        return;

    s32 index = getSurfaceProbeIndex(this);

    if (mSurfaceProbePos[index].y > al::getTrans(this).y) {
        al::startHitReactionHitEffect(this, "着地",
                                      mSurfaceProbePos[index] + 0.0f * sead::Vector3f::ey);
        mIsOnHitReactionEffect = true;
    }
}

void Bubble::shiftSink() {
    sead::Vector3f hitEffect;
    if (mIsCheckEffectPosRequired || mIsOnHitReactionEffect) {
        if (isValidCollisionOrWaveCheck() && !trySendMsgStartInSaucePan()) {
            hitEffect =
                al::getTrans(this) - (al::getColliderRadius(this) + 0.0f) * sead::Vector3f::ey;
            al::startHitReactionHitEffect(this, "着地", hitEffect);
        }
    } else {
        s32 index = getSurfaceProbeIndex(this);
        if (!trySendMsgStartInSaucePan()) {
            hitEffect = mSurfaceProbePos[index] + 0.0f * sead::Vector3f::ey;
            al::startHitReactionHitEffect(this, "着地", hitEffect);
        }
    }
    al::setNerve(this, &NrvBubble.Sink);
}

void Bubble::headUp() {
    sead::Vector3f upDir;
    al::calcUpDir(&upDir, this);
    if (upDir.y > 0.99f)
        return;

    sead::Quatf quat;
    al::makeQuatRotationRate(&quat, upDir, sead::Vector3f::ey, 1.0f);
    quat = quat * al::getQuat(this);
    quat.normalize();
    al::setQuat(this, quat);
}

void Bubble::shiftReadyOrMove() {
    if (al::isExistRail(this) && al::isNoCollide(this))
        al::setNerve(this, &NrvBubble.RailMove);
    else
        al::setNerve(this, &NrvBubble.Ready);
}

bool Bubble::trySendMsgStartInSaucePan() {
    if (mIsInSaucePan || !al::isCollidedFloorCode(this, "DamageFire"))
        return false;

    al::HitSensor* hitSensor = al::tryGetCollidedGroundSensor(this);
    if (!hitSensor)
        return false;

    if (rs::sendMsgStartInSaucePan(hitSensor, al::getHitSensor(this, "Body"), false)) {
        al::startHitReaction(this, "入鍋");
        al::tryAddRippleLarge(this);
        mIsInSaucePan = true;
        mIsPlayerCaptured = true;
        return true;
    }

    return false;
}

void Bubble::tryHitReactionThroughFence() {
    sead::Vector3f velocity = al::getVelocity(this);
    if (al::isNearZero(velocity))
        return;

    sead::Vector3f pos;
    al::Triangle triangle;
    if (alCollisionUtil::getFirstPolyOnArrow(this, &pos, &triangle, al::getTrans(this), velocity,
                                             nullptr, nullptr) &&
        isIgnoreTriangle(triangle)) {
        al::startHitReaction(this, "金網通過");
        mAnimScaleController->startHitReaction();
    }
}

bool Bubble::tryShiftLand() {
    if (!al::isOnGround(this, 0) && !(mIsOnLavaSurface && al::getVelocity(this).y < 0.0f))
        return false;

    if (!isOnDamageFire()) {
        forceEndHack();
        return true;
    }

    if (al::isOnGround(this, 0))
        mFireNormal.set(al::getOnGroundNormal(this, 0));
    sead::Quatf quat;
    al::makeQuatRotationRate(&quat, sead::Vector3f::ey, mFireNormal, 1.0f);
    al::getVelocityPtr(this)->rotate(quat);
    sead::Vector3f* velPtr = al::getVelocityPtr(this);
    al::verticalizeVec(velPtr, mFireNormal, *velPtr);

    if (mPlayerHack) {
        mAirTime = 0;
        mLandVelocity.set(0.0f, 0.0f, 0.0f);
        updateCollisionPartsMove();
        al::getVelocityPtr(this)->add(mLandVelocity);
        if (al::isCollidedGround(this)) {
            rs::sendMsgBubbleGroundTouchTrigger(al::getCollidedGroundSensor(this),
                                                al::getHitSensor(this, "Attack"));
        }
        al::setNerve(this, &NrvBubble.HackLand);
        return true;
    }

    al::setNerve(this, &NrvBubble.Sink);
    return true;
}

bool Bubble::isHoldHackAction() const {
    return !mIsPlayerCaptured && rs::isHoldHackAction(mPlayerHack);
}

bool Bubble::tryBoundMoveWall() {
    if (!al::isCollidedWall(this) || !al::isCollidedGround(this))
        return false;

    const al::CollisionParts* collisionParts = al::getCollidedWallCollisionParts(this);
    if (collisionParts->get_15c() != 0 && !collisionParts->isMoving())
        return false;

    sead::Vector3f wallNormal = al::getCollidedWallNormal(this);
    const sead::Vector3f& groundNormal = al::getCollidedGroundNormal(this);
    if (groundNormal.dot(wallNormal) < 0.1736482f)  // cos(80°)
        return false;

    al::verticalizeVec(&wallNormal, groundNormal, wallNormal);
    if (!al::tryNormalizeOrZero(&wallNormal))
        return false;

    sead::Vector3f prevTrans = collisionParts->getPrevBaseMtx().getTranslation();
    sead::Vector3f baseTrans = collisionParts->getBaseMtx().getTranslation();
    sead::Vector3f moveH = baseTrans - prevTrans;
    al::verticalizeVec(&moveH, groundNormal, moveH);

    f32 speedH = moveH.length();
    if (al::isNearZero(speedH))
        return false;

    sead::Vector3f moveDirH = moveH * (1.0f / speedH);

    if (moveDirH.dot(wallNormal) < 0.7071068f)  // cos(45°)
        return false;

    sead::Vector3f velH;
    sead::Vector3f velV;
    al::separateVectorParallelVertical(&velH, &velV, moveDirH, al::getVelocity(this));
    if (velH.dot(moveDirH) >= speedH * 5.0f)
        return false;

    al::getVelocityPtr(this)->setAdd(5.0f * speedH * moveDirH, velV);
    mAnimScaleController->startHitReaction();
    return true;
}

void Bubble::updateHackOnGround() {
    static sead::Vector3f magic = {0.0f, 0.0f, 0.0f};

    *al::getVelocityPtr(this) -= mLandVelocity;
    if (!al::isOnGroundNoVelocity(this, 0)) {
        if (mIsOnLavaSurface) {
            mAirTime = 0;
            sead::Vector3f* velPtr = al::getVelocityPtr(this);
            al::verticalizeVec(velPtr, sead::Vector3f::ey, *velPtr);
        } else {
            sead::Vector3f groundPos;
            sead::Vector3f upDir;
            al::calcUpDir(&upDir, this);

            sead::Vector3f colliderPos = al::getTrans(this) + al::getColliderOffsetY(this) * upDir;

            if (alCollisionUtil::getFirstPolyOnArrow(this, &groundPos, nullptr, colliderPos,
                                                     (-50.0f - al::getColliderRadius(this)) *
                                                         sead::Vector3f::ey,
                                                     mCollisionPartsFilter, nullptr)) {
                mColliderGroundOffset =
                    groundPos - (colliderPos - al::getColliderRadius(this) * sead::Vector3f::ey);
            }
            mAirTime++;
        }
    } else if (mIsOnLavaSurface) {
        mAirTime = 0;
        sead::Vector3f* velPtr = al::getVelocityPtr(this);
        al::verticalizeVec(velPtr, sead::Vector3f::ey, *velPtr);
    } else {
        mAirTime = 0;
        mFireNormal.set(al::getOnGroundNormal(this, 0));
        sead::Vector3f* velPtr = al::getVelocityPtr(this);
        al::verticalizeVec(velPtr, mFireNormal, *velPtr);
    }
    updateCollisionPartsMove();
    al::setQuat(this, mGroundRotation);
    accelStick();
    mGroundRotation.set(al::getQuat(this));
    sead::Vector3f updir2;
    al::calcUpDir(&updir2, this);

    sead::Quatf quat;
    al::makeQuatRotationRate(&quat, updir2, mUpDir, 1.0f);
    al::setQuat(this, quat * al::getQuat(this));

    sead::Quatf quat2;
    al::makeQuatRotationRate(&quat2, mUpDir, mFireNormal, 0.1f);
    al::setQuat(this, quat2 * al::getQuat(this));

    al::calcUpDir(&mUpDir, this);

    sead::Quatf quat3;
    al::makeQuatZDegree(&quat3, ((f32)mHackTurnFrame / (f32)mHackTurnDelay) * mHackTurnAngle);
    quat3.normalize();
    al::setQuat(this, al::getQuat(this) * quat3);

    al::getVelocityPtr(this)->add(mLandVelocity);
}

// TODO: might be moved into `sead`
inline f32 normalize2(sead::Vector3f* v, f32 scalar) {
    const f32 len = v->length();
    if (len > 0) {
        const f32 inv_len = scalar / len;
        v->x *= inv_len;
        v->y *= inv_len;
        v->z *= inv_len;
    }

    return len;
}

bool Bubble::constrainLavaDomain() {
    if (!isOnGroundNoVelocity())
        return true;

    f32 colliderRadius = al::getColliderRadius(this);
    sead::Vector3f front;
    al::calcFrontDir(&front, this);
    front.y = 0.0;
    al::tryNormalizeOrDirZ(&front);
    front *= colliderRadius;

    sead::Vector3f side;
    side.setCross(sead::Vector3f::ey, front);
    normalize2(&side, colliderRadius);

    sead::Vector3f basePos = al::getTrans(this) + colliderRadius * sead::Vector3f::ey;
    sead::Vector3f checkPos[4];
    checkPos[0] = basePos + (-side + front);
    checkPos[1] = basePos + (side + front);
    checkPos[2] = basePos + (-side - front);
    checkPos[3] = basePos + (side - front);
    sead::Vector3f checkDir = sead::Vector3f::ey * (colliderRadius * -3.0f);

    bool isFire[4] = {false, false, false, false};
    bool isPoly[4] = {false, false, false, false};
    f32 height[4] = {0.0f, 0.0f, 0.0f, 0.0f};

    for (s32 i = 0; i < 4; i++) {
        sead::Vector3f hitPos;
        al::Triangle triangle;
        if (alCollisionUtil::getFirstPolyOnArrow(this, &hitPos, &triangle, checkPos[i], checkDir,
                                                 mCollisionPartsFilter, nullptr)) {
            height[i] = hitPos.y;
            isFire[i] = al::isFloorCode(triangle, "DamageFire");
            isPoly[i] = true;
        }
    }

    sead::Vector3f firePos;
    sead::Vector3f fireNormal;
    for (s32 i = 0; i < 4; i++) {
        if (!al::calcFindFireSurface(&firePos, &fireNormal, this, checkPos[i], sead::Vector3f::ey,
                                     200.0f))
            continue;
        if (isPoly[i] && height[i] > firePos.y)
            continue;

        isFire[i] = true;
        isPoly[i] = true;
    }

    if ((!al::isOnGroundNoVelocity(this, 0) ||
         !al::isCollidedGroundFloorCode(this, "DamageFire")) &&
        (!isPoly[0] || !isFire[0]) && (!isPoly[1] || !isFire[1]) && (!isPoly[2] || !isFire[2]) &&
        (!isPoly[3] || !isFire[3])) {
        forceEndHack();
        return false;
    }

    u32 noFirePosCount = 0;
    sead::Vector3f noFirePosSum = {0.0f, 0.0f, 0.0f};
    for (s32 i = 0; i < 4; i++) {
        if (isPoly[i] && !isFire[i]) {
            noFirePosSum += checkPos[i];
            noFirePosCount++;
        }
    }

    if (noFirePosCount != 0) {
        sead::Vector3f pushDirection = -(1.0f / noFirePosCount) * noFirePosSum + al::getTrans(this);
        pushDirection.y = 0.0f;

        al::tryNormalizeOrZero(&pushDirection);
        if (pushDirection.dot(al::getVelocity(this)) > 0.0f) {
            sead::Vector3f* velPtr = al::getVelocityPtr(this);
            al::verticalizeVec(velPtr, pushDirection, *velPtr);
        }

        al::addVelocity(this, pushDirection * 3.0f);
    }

    return true;
}

bool Bubble::tryShiftFall() {
    if (mAirTime > 9) {
        mJumpForce.set(0.0f, 0.0f, 0.0f);
        al::setNerve(this, &NrvBubble.HackJump);
        return true;
    }

    return false;
}

bool Bubble::isTriggerHackSwing() const {
    return !mIsPlayerCaptured && rs::isTriggerHackSwing(mPlayerHack);
}

void Bubble::faceToMoveVec() {
    sead::Vector3f moveVec;
    calcHackerMoveVec(&moveVec, sead::Vector3f::ey);

    if (al::isNearZero(moveVec))
        return;

    al::normalize(&moveVec);
    al::faceToDirection(this, moveVec);
    sead::Vector3f velocity = al::getVelocity(this);
    velocity.y = 0.0f;
    if (!al::tryNormalizeOrZero(&velocity))
        return;

    sead::Quatf quat;
    al::makeQuatRotationRate(&quat, velocity, moveVec, 1.0f);
    quat.normalize();

    sead::Vector3f newVelocity = al::getVelocity(this);
    newVelocity.rotate(quat);
    al::setVelocity(this, newVelocity);
}

bool Bubble::isTriggerHackJump() const {
    return !mIsPlayerCaptured && rs::isTriggerHackPreInputJump(mPlayerHack);
}

void Bubble::revertTargetQuatInHackJump(sead::Quatf* prevQuat, sead::Quatf* deltaQuat) {
    prevQuat->set(al::getQuat(this));
    sead::Quatf invRotation;
    invRotation.setInverse(mGroundRotation);
    deltaQuat->set(invRotation * al::getQuat(this));
    deltaQuat->normalize();
    al::setQuat(this, mGroundRotation);
}

void Bubble::calcHackerMoveVec(sead::Vector3f* moveVec, const sead::Vector3f& inputDir) const {
    if (mIsPlayerCaptured) {
        moveVec->set(0.0f, 0.0f, 0.0f);
        return;
    }
    rs::calcHackerMoveVec(moveVec, mPlayerHack, inputDir);
}

void Bubble::makeDisplayQuatInHackJump(const sead::Quatf& prevQuat, const sead::Quatf& deltaQuat,
                                       const sead::Quatf& quatZY, bool flipDirection) {
    sead::Vector3f moveDir = al::getTrans(this) - mPreviousTrans;
    mPreviousTrans.set(al::getTrans(this));

    sead::Quatf newQuat = mGroundRotation;
    if (al::tryNormalizeOrZero(&moveDir)) {
        f32 angle = al::calcAngleDegree(sead::Vector3f::ey, moveDir);
        if (flipDirection)
            angle = !(al::getVelocity(this).y >= 0.0f) ? 180.0f : 0.0f;
        sead::Vector3f dir = sead::Vector3f::ey;
        dir.rotate(prevQuat);
        dir.negate();
        sead::Quatf rotationQuat;
        al::makeQuatRotateDegree(
            &rotationQuat, sead::Vector3f::ex,
            sead::Mathf::max(al::calcAngleDegree(sead::Vector3f::ey, dir), angle));
        newQuat = newQuat * rotationQuat * quatZY;
    } else {
        newQuat *= deltaQuat;
    }

    sead::Quatf slerpQuat;
    al::slerpQuat(&slerpQuat, prevQuat, newQuat, 0.1f);
    al::setQuat(this, slerpQuat);
}

bool Bubble::isDropAttackCollision() const {
    if (al::getVelocity(this).y > 0.0f)
        return false;

    if (!al::isOnGround(this, 0))
        return false;

    const sead::Vector3f& groundPos = al::getCollidedGroundPos(this);
    const sead::Vector3f& trans = al::getTrans(this);
    return !(groundPos.y - trans.y >= 0.0f);
}

bool Bubble::isRiseAttackCollision() const {
    if (al::getVelocity(this).y < 0.0f)
        return false;

    if (!al::isCollidedCeiling(this))
        return false;

    const sead::Vector3f& ceilingPos = al::getCollidedCeilingPos(this);
    const sead::Vector3f& trans = al::getTrans(this);
    return !(ceilingPos.y - trans.y <= 0.0f);
}

bool Bubble::isHoldHackJump() const {
    return !mIsPlayerCaptured && rs::isHoldHackJump(mPlayerHack);
}

bool Bubble::tryShiftContinuousJump() {
    if (!al::isNerve(this, &NrvBubble.HackJumpHigh))
        return false;

    if (mContinuousJumpDelay < 15)
        mContinuousJumpDelay++;

    if (isTriggerHackSwing())
        mContinuousJumpDelay = 0;

    if (!isOnGround())
        return false;

    if (al::getVelocity(this).y >= 0.0f)
        return false;

    if (!isOnDamageFire())
        return false;

    if (mContinuousJumpDelay >= 15)
        return false;

    mJumpForce.set({0.0f, 25.0f, 0.0f});
    al::getVelocityPtr(this)->y = 0.0f;
    al::startHitReaction(this, "水切りジャンプ");
    al::setNerve(this, &NrvBubble.HackJumpHigh);

    if (al::isOnGround(this, 0))
        mFireNormal.set(al::getOnGroundNormal(this, 0));

    mAirTime = 0;
    mLandVelocity.set(0.0f, 0.0f, 0.0f);

    if (al::isCollidedGround(this)) {
        rs::sendMsgBubbleGroundTouchTrigger(al::getCollidedGroundSensor(this),
                                            al::getHitSensor(this, "Attack"));
    }

    al::tryAddRippleMiddle(this);
    return true;
}

bool Bubble::calcHackerMoveDir(sead::Vector3f* moveDir, const sead::Vector3f& inputDir) const {
    if (mIsPlayerCaptured) {
        moveDir->set(0.0f, 0.0f, 0.0f);
        return false;
    }
    return rs::calcHackerMoveDir(moveDir, mPlayerHack, inputDir);
}

bool Bubble::isWaitingLaunch() const {
    return al::isNerve(this, &NrvBubble.HackInLauncher) && mBubbleStateInLauncher->isWaiting();
}

void Bubble::launch(const sead::Vector3f& targetPos, f32 launchDegree,
                    al::CameraTicket* cameraTicket) {
    if (!al::isNerve(this, &NrvBubble.HackInLauncher))
        return;

    mBubbleStateInLauncher->shoot(targetPos, launchDegree);
    if (cameraTicket)
        mLaunchCameraTicket = cameraTicket;
    rs::hideTutorial(this);
    mIsShowTutorial = true;
}

void Bubble::launchCancel(const sead::Vector3f& targetPos) {
    if (!al::isNerve(this, &NrvBubble.HackInLauncher))
        return;

    mBubbleStateInLauncher->kill();
    al::showModelIfHide(this);
    al::showSilhouetteModelIfHide(this);
    showHackCap();

    sead::Vector3f forceH = al::getTrans(this) - targetPos;
    forceH.y = 0.0f;
    al::tryNormalizeOrZero(&forceH);
    mJumpForce.set(25.0f * sead::Vector3f::ey + 7.0f * forceH);

    al::faceToDirection(this, forceH);
    mCancelLaunchCooldown = 5;
    al::setNerve(this, &NrvBubble.HackCancelJump);
}

void Bubble::showHackCap() {
    rs::showHackCap(mPlayerHack);
}

void Bubble::hideHackCap() {
    rs::hideHackCap(mPlayerHack);
}

bool Bubble::isTriggerJump() const {
    return mPlayerHack && (isTriggerHackJump() || isTriggerHackSwing());
}

bool Bubble::isTriggerAction() const {
    return mPlayerHack && isTriggerHackAction();
}

bool Bubble::isTriggerHackAction() const {
    return !mIsPlayerCaptured && rs::isTriggerHackAction(mPlayerHack);
}

bool Bubble::isTriggerCancel() const {
    return mPlayerHack && HackFunction::isTriggerCancelBubbleLauncher(mPlayerHack);
}

void Bubble::startJumpAnim(f32 force) {
    if (force > 0.0f) {
        al::startAction(this, "Up");
        return;
    }

    if (al::isActionPlaying(this, "Down") || al::isActionPlaying(this, "Turn"))
        return;

    al::startAction(this, "Turn");
}

void Bubble::resetAndAppear(const sead::Vector3f& trans, const sead::Quatf& quat, f32 unused) {
    mRevivePosition.set(trans);
    mStartingRotation.set(quat);
    mStartActionPosition.set(trans);
    mReviveStartActionPosition.set(mStartActionPosition);
    al::resetPosition(this, trans);
    al::setQuat(this, quat);
    recalcClippingInfo();
    appear();
}

void Bubble::endHackCommon() {
    al::resetCameraTarget(this, mActorMatrixCameraTarget);
    sead::Vector3f colliderPos;
    al::calcColliderPos(&colliderPos, this);
    f32 colliderRadius = al::getColliderRadius(this);
    sead::Vector3f targetPos = colliderPos;
    targetPos.y += colliderRadius;
    sead::Vector3f move = targetPos - colliderPos;

    alCollisionUtil::SphereMoveHitInfo sphereMoveHitInfos[8];

    if (alCollisionUtil::checkStrikeSphereMove(this, sphereMoveHitInfos, 8, colliderPos,
                                               colliderRadius, move, nullptr, nullptr) != 0) {
        targetPos = sphereMoveHitInfos[0]._0 * move + colliderPos;
    }

    sead::Vector3f upDir;
    sead::Quatf rotationRate;
    al::calcUpDir(&upDir, this);
    al::makeQuatRotationRate(&rotationRate, upDir, sead::Vector3f::ey, 1.0f);

    rotationRate *= al::getQuat(this);
    rs::endHackFromTargetPos(&mPlayerHack, targetPos, rotationRate, sead::Vector3f::zero);
    al::showModelIfHide(this);
    rs::endHackShadow(this);
    mJointSpringControllerHolder->offControlAll();
    mAnimScaleController->startHitReaction();
    al::endBgmSituation(this, "HackBubble", false);
}

void Bubble::calcLaunchPos(sead::Vector3f* pos, const sead::Vector3f& targetPos, f32 launchDegree,
                           f32 frame) const {
    s32 index = sead::Mathf::floor(frame);
    f32 t = frame - index;

    sead::Vector3f startPos;
    sead::Vector3f endPos;
    mBubbleStateInLauncher->calcLaunchPos(&startPos, targetPos, launchDegree, index);
    mBubbleStateInLauncher->calcLaunchPos(&endPos, targetPos, launchDegree, index + 1);

    // Lerp
    pos->set((1.0f - t) * startPos + t * endPos);
}

void Bubble::onGroupClipping() {
    if (!mIsOnGroupClipping) {
        al::onGroupClipping(this);
        mIsOnGroupClipping = true;
    }
}

bool Bubble::isOnGroundNoVelocity() const {
    return al::isOnGroundNoVelocity(this, 0) || mIsOnLavaSurface;
}

void Bubble::updateCollisionPartsMove() {
    const al::CollisionParts* collisionParts = al::tryGetCollidedGroundCollisionParts(this);
    if (!collisionParts) {
        if (mIsOnLavaSurface || mAirTime > 9)
            mLandVelocity.set(0.0f, 0.0f, 0.0f);
        return;
    }
    sead::Vector3f trans = al::getTrans(this);
    trans *= collisionParts->getPrevBaseInvMtx();
    trans *= collisionParts->getBaseMtx();
    mLandVelocity.set(trans - al::getTrans(this));
}

// NON_MATCHING: Bad branch order https://decomp.me/scratch/ACufd
void Bubble::accelStick() {
    sead::Vector3f upDir;
    if (al::isOnGroundNoVelocity(this, 0))
        upDir.set(al::getOnGroundNormal(this, 0));
    else
        upDir.set(sead::Vector3f::ey);

    sead::Vector3f stickForceNormalized = mStickForce;
    if (al::tryNormalizeOrZero(&stickForceNormalized)) {
        sead::Vector3f cross;
        cross.setCross(upDir, stickForceNormalized);
        if (al::tryNormalizeOrZero(&cross)) {
            f32 dot = cross.dot(al::getVelocity(this));
            *al::getVelocityPtr(this) -= dot * cross;
            *al::getVelocityPtr(this) += (dot * 0.9f) * cross;
        }
        f32 force = sead::Mathf::clampMax(
            mStickForce.length() - (stickForceNormalized.dot(al::getVelocity(this))), 0.5f);
        if (force > 0.0f)
            al::getVelocityPtr(this)->add(force * stickForceNormalized);
    }

    sead::Vector3f velocityDir = upDir * upDir.dot(al::getVelocity(this));
    al::setVelocity(this,
                    (al::getVelocity(this) - velocityDir) * 0.92f + (velocityDir - upDir * 1.1f));
    mStickForce.set({0.0f, 0.0f, 0.0f});

    if (mIsPlayerCaptured) {
        mHackTurnAngle *= 0.9f;
        return;
    }

    // this screams for addHackActorAccelStick(&stickAccel, scale, upDir)
    sead::Vector3f stickAccel;
    f32 scale = rs::isHoldHackAction(mPlayerHack) ? 1.6f : 1.4f;
    if (mIsPlayerCaptured) {
        stickAccel.set(0.0f, 0.0f, 0.0f);
        mHackTurnAngle *= 0.9f;
        return;
    }

    if (rs::addHackActorAccelStick(this, mPlayerHack, &stickAccel, scale, upDir)) {
        al::turnToDirection(this, stickAccel, 10.0f);

        sead::Vector3f rotationAxis;
        rotationAxis.setRotated(mGroundRotation, sead::Vector3f::ez);
        stickAccel.y = 0.0f;

        if (!al::tryNormalizeOrZero(&stickAccel))
            stickAccel.set(rotationAxis);

        f32 dot = stickAccel.dot(rotationAxis);
        f32 angle = atan2f(rotationAxis.cross(stickAccel).length(), dot);
        angle = (sead::Mathf::clamp(angle, 0.0f, sead::Mathf::piHalf()) * 72.0f) /
                sead::Mathf::piHalf();

        if (rotationAxis.z * stickAccel.x - rotationAxis.x * stickAccel.z > 0.0f)
            angle = -angle;

        bool condition = !(mHackTurnAngle * angle < 0.0f) &&
                         sead::Mathf::abs(mHackTurnAngle) > sead::Mathf::abs(angle);

        if (condition) {
            mHackTurnAngle *= 0.4f;
            mHackTurnAngle += angle * 0.6f;
        } else {
            mHackTurnAngle *= 0.95f;
            mHackTurnAngle += angle * 0.05f;
        }
    }
}

bool Bubble::addHackActorAccelStick(sead::Vector3f* stickAccel, f32 scale,
                                    const sead::Vector3f& dir) {
    if (mIsPlayerCaptured) {
        stickAccel->set(0.0f, 0.0f, 0.0f);
        return false;
    }
    return rs::addHackActorAccelStick(this, mPlayerHack, stickAccel, scale, dir);
}

bool Bubble::isGroundOverTheWave(bool checkHeight, const sead::Vector3f& wavePos) const {
    if (al::isOnGround(this, 0)) {
        if (!checkHeight)
            return true;

        if (wavePos.y <
            al::getTrans(this).y - al::getColliderRadius(this) + al::getColliderOffsetY(this)) {
            return true;
        }
    }

    return false;
}

bool Bubble::isEnableSnapWaveSurface() const {
    return mIsOnLavaSurface && !al::isNerve(this, &NrvBubble.HackJump) &&
           !al::isNerve(this, &NrvBubble.HackJumpHigh) &&
           !al::isNerve(this, &NrvBubble.HackCancelJump) && !al::isNerve(this, &NrvBubble.Down);
}

void Bubble::exeStandBy() {}

void Bubble::exeDelay() {
    if (al::isGreaterEqualStep(this, mDelayFrameNum))
        al::setNerve(this, &NrvBubble.Ready);
}

void Bubble::exeRailMove() {
    if (al::isFirstStep(this))
        al::startAction(this, "Move");

    al::tryAddRippleSmall(this);
    f32 nerveRate = al::calcNerveRate(this, mRailMoveFrame);
    if (!al::isRailGoingToEnd(this))
        nerveRate = 1.0 - nerveRate;
    al::setSyncRailToCoord(this, mRailTotalLength * nerveRate);

    updatePosOnWave();

    if (al::isGreaterEqualStep(this, mRailMoveFrame)) {
        if (al::isRailGoingToEnd(this))
            al::setQuat(this, mRailTargetRotation);
        else
            al::setQuat(this, mStartingRotation);
        al::reverseRail(this);
        al::setNerve(this, &NrvBubble.Ready);
    }
}

void Bubble::exeReady() {
    if (al::isFirstStep(this)) {
        if (isValidCollisionOrWaveCheck()) {
            al::startHitReactionHitEffect(
                this, "予兆",
                al::getTrans(this) - (al::getColliderRadius(this) + 0.0f) * sead::Vector3f::ey);
        } else if (!mIsCheckEffectPosRequired) {
            al::startHitReactionHitEffect(this, "予兆",
                                          mSurfaceProbePos[getSurfaceProbeIndex(this)] +
                                              0.0f * sead::Vector3f::ey);
        }
        Bubble::initAfterPlacement();
    }
    updateVelocityIfValidCollision();
    if (isValidCollisionOrWaveCheck() && !isOnDamageFire())
        forceEndHack();
    else if (al::isGreaterEqualStep(this, 60))
        al::setNerve(this, &NrvBubble.Up);
}

void Bubble::exeUp() {
    if (al::isFirstStep(this)) {
        al::showModel(this);
        al::startAction(this, "Up");
        if (isValidCollisionOrWaveCheck()) {
            mStartActionPosition.set(al::getTrans(this));
            al::startHitReactionHitEffect(
                this, "発射",
                al::getTrans(this) - (al::getColliderRadius(this) + 0.0f) * sead::Vector3f::ey);
            mIsOnHitReactionEffect = true;
        } else {
            mIsOnHitReactionEffect = false;
        }
        al::tryAddRippleMiddle(this);
        mTurnFrameCount = mTurnFrameDelay;
    }
    tryStartHitReactionUp();
    sead::Vector3f nextPos = al::getTrans(this);
    nextPos.y = mStartActionPosition.y +
                al::calcNerveSquareOutValue(this, mTurnFrameCount, 0.0f, mJumpHeight);

    if (isValidCollisionOrWaveCheck())
        al::setVelocity(this, nextPos - al::getTrans(this));
    else
        al::setTrans(this, nextPos);
    updateShadowMaskLength();

    if (!al::isActionPlaying(this, "Turn") && al::isCollidedCeiling(this))
        mTurnFrameCount = al::getNerveStep(this) + 8;

    if (al::isStep(this, mTurnFrameCount - 8))
        al::tryStartActionIfNotPlaying(this, "Turn");

    if (al::isGreaterEqualStep(this, mTurnFrameCount))
        al::setNerve(this, &NrvBubble.Turn);
}

void Bubble::exeTurn() {
    al::isFirstStep(this);
    if (al::isGreaterEqualStep(this, 4))
        al::setNerve(this, &NrvBubble.Down);
}

void Bubble::exeDown() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Down");
        if (isValidCollisionOrWaveCheck()) {
            mStartActionPosition.set(al::getTrans(this));
            mStartActionPosition.y -= mJumpHeight;
            mIsOnHitReactionEffect = true;
        } else {
            mIsOnHitReactionEffect = false;
        }
    }

    tryStartHitReactionDown();
    sead::Vector3f nextPos = al::getTrans(this);

    f32 step = al::getNerveStep(this);
    f32 rate = sead::Mathf::square(step / mTurnFrameCount);
    if (!isValidCollisionOrWaveCheck())
        rate = sead::Mathf::clamp(rate, 0.0f, 1.0f);

    nextPos.y = mStartActionPosition.y + mJumpHeight * (1.0f - rate);

    if (isValidCollisionOrWaveCheck())
        al::setVelocity(this, nextPos - al::getTrans(this));
    else
        al::setTrans(this, nextPos);

    updatePosOnWave();

    updateShadowMaskLength();

    if (isValidCollisionOrWaveCheck()) {
        if (isOnGround()) {
            if (!isOnDamageFire()) {
                al::startHitReaction(this, "地面での消滅");
                revive();
            } else {
                al::tryAddRippleMiddle(this);
                shiftSink();
            }
        }
    } else {
        if (al::isGreaterEqualStep(this, mTurnFrameCount))
            shiftSink();
    }
}

void Bubble::exeSink() {
    if (al::isFirstStep(this)) {
        al::tryAddRippleMiddle(this);
        if (al::isCollidedGround(this)) {
            rs::sendMsgBubbleGroundTouchTrigger(al::getCollidedGroundSensor(this),
                                                al::getHitSensor(this, "Attack"));
        }
    }

    f32 nerveRate = al::calcNerveRate(this, 10);
    if (isValidCollisionOrWaveCheck()) {
        updateLavaWave();
        al::setVelocityY(this, -30.0f);
        al::setColliderOffsetY(this, nerveRate * 150.0f);
    } else {
        al::getTransPtr(this)->y = mStartActionPosition.y + nerveRate * -150.0f;
    }

    if (al::isGreaterEqualStep(this, 10)) {
        headUp();
        al::setNerve(this, &NrvBubble.Wait);
    }
}

void Bubble::exeWait() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Wait");
        al::hideModel(this);
        al::setVelocityZero(this);
        if (isValidCollisionOrWaveCheck()) {
            al::getTransPtr(this)->y += al::getColliderOffsetY(this);
            al::setColliderOffsetY(this, 0.0f);
        } else {
            al::getTransPtr(this)->y = mStartActionPosition.y;
        }
    }

    updateVelocityIfValidCollision();

    if (isValidCollisionOrWaveCheck() && !isOnDamageFire()) {
        forceEndHack();
        return;
    }

    if (al::isGreaterEqualStep(this, mWaitFrameNum) && !mIsInSaucePan) {
        if (!al::isNoCollide(this) && mWaitReviveDelay < 1) {
            revive();
            return;
        }

        mWaitReviveDelay--;

        if (!al::isValidSwitchStart(this) || al::isOnSwitchStart(this))
            shiftReadyOrMove();
    }
}

void Bubble::exeDie() {
    al::startHitReaction(this, "死亡");
    kill();
}

void Bubble::exeWaitHack() {
    setVelocityZero(this);
}

void Bubble::exeWaitHackStart() {
    al::setVelocityZero(this);
    if (rs::isHackStartDemoEnterMario(mPlayerHack)) {
        al::startHitReaction(this, "憑依開始");
        if (!al::isActionPlaying(this, "HackStartDown") &&
            !al::isActionPlaying(this, "HackStart")) {
            if (al::isActionPlaying(this, "Down") || al::isActionPlaying(this, "Turn"))
                al::startAction(this, "HackStartDown");
            else
                al::startAction(this, "HackStart");
            rs::setupHackShadow(this);
            mPlayerHackStartShaderCtrl->start();
            mJointSpringControllerHolder->offControlAll();
        }
    }

    if (al::isActionPlaying(this, "HackStartDown") || al::isActionPlaying(this, "HackStart")) {
        mPlayerHackStartShaderCtrl->update();
        if (al::isActionEnd(this)) {
            mPlayerHackStartShaderCtrl->end();
            rs::endHackStartDemo(mPlayerHack, this);
            al::setNerve(this, &NrvBubble.HackFall);
            return;
        }
    }
}

void Bubble::endWaitHackStart() {
    mJointSpringControllerHolder->onControlAll();
}

void Bubble::exeWaitHackFall() {
    if (al::isFirstStep(this))
        al::setVelocityZero(this);

    al::addVelocityToGravity(this, 1.1f);
    al::scaleVelocityHV(this, 1.0f, 0.995f);

    if (isOnGround()) {
        if (isOnDamageFire()) {
            mStartActionPosition.set(al::getTrans(this));

            if (!trySendMsgStartInSaucePan()) {
                al::startHitReactionHitEffect(
                    this, "着地",
                    al::getTrans(this) - (al::getColliderRadius(this) + 0.0f) * sead::Vector3f::ey);
            }
            al::setNerve(this, &NrvBubble.Sink);
        } else {
            al::startHitReaction(this, "地面での消滅");
            revive();
        }

        al::setVelocityZero(this);
    }
}

void Bubble::exeHackFall() {
    if (al::isFirstStep(this))
        mGroundRotation.set(al::getQuat(this));

    al::getVelocityPtr(this)->y -= 1.1f;
    tryHitReactionThroughFence();
    tryShiftLand();
}

void Bubble::exeHackMove() {
    if (al::isFirstStep(this)) {
        mAirTime = 0;
        mGroundRotation.set(al::getQuat(this));
        al::calcUpDir(&mUpDir, this);
    }

    if (al::getNerveStep(this) % 20 == 0 && al::isOnGround(this, 0)) {
        const char* materialCodeName = al::getCollidedFloorMaterialCodeName(this);
        if (materialCodeName && al::isEqualString("LavaRed", materialCodeName))
            al::tryAddRippleRandomBlur(this, al::getTrans(this), 0.45f, 110.0f, 0.0f);
        else
            al::tryAddRippleRandomBlur(this, al::getTrans(this), 0.45f, 110.0f, 100.0f);
    }

    if (mIsPlayerCaptured) {
        al::tryStartActionIfNotPlaying(this, "HackWait");
    } else if (!rs::isOnHackMoveStick(mPlayerHack)) {
        if (isHoldHackAction())
            al::tryStartActionIfNotPlaying(this, "HackWaitSpeedy");
        else
            al::tryStartActionIfNotPlaying(this, "HackWait");
    } else if (!mIsPlayerCaptured && rs::isHoldHackAction(mPlayerHack)) {  // isHoldHackAction()
        al::tryStartActionIfNotPlaying(this, "MoveSpeedy");
        if (!mIsInBossSequence)
            rs::requestDownToDefaultCameraAngleBySpeed(this, 6.0f, 0);
    } else {
        al::tryStartActionIfNotPlaying(this, "Move");
    }

    if (al::isCollidedWall(this)) {
        rs::sendMsgBubbleWallTouch(al::getCollidedWallSensor(this),
                                   al::getHitSensor(this, "Attack"));
        if (!tryBoundMoveWall()) {
            f32 force = al::getCollidedWallNormal(this).dot(al::getVelocity(this));
            if (force < 0.0f)
                *al::getVelocityPtr(this) -= force * al::getCollidedWallNormal(this);
        }
    }

    updateHackOnGround();
    if (!constrainLavaDomain() || tryShiftFall())
        return;

    if (isTriggerHackSwing()) {
        faceToMoveVec();
        mJumpForce.set({0.0f, 25.0f, 0.0f});
        al::setNerve(this, &NrvBubble.HackJumpHigh);
        return;
    }

    if (isTriggerHackJump()) {
        faceToMoveVec();
        mJumpForce.set({0.0f, 45.0f, 0.0f});
        al::setNerve(this, &NrvBubble.HackJump);
    }
}

void Bubble::endHackMove() {
    al::setQuat(this, mGroundRotation);
}

inline sead::Quatf getQuatZY() {
    sead::Quatf quatZ;
    sead::Quatf quatY;
    al::makeQuatRotateDegree(&quatZ, sead::Vector3f::ez, 180.0f);
    al::makeQuatRotateDegree(&quatY, sead::Vector3f::ey, 180.0f);
    return quatZ * quatY;
}

// NON_MATCHING: Wrong math operation https://decomp.me/scratch/P9h4T
void Bubble::exeHackJump() {
    bool isJumpSwing = al::isNerve(this, &NrvBubble.HackJumpHigh);
    f32 gravity = !isJumpSwing ? 1.1f : 1.2f;
    f32 frontAccel = !isJumpSwing ? 0.5f : 2.5f;

    sead::Quatf prevQuat;
    sead::Quatf deltaQuat;
    sead::Quatf quatZY = getQuatZY();

    if (al::isFirstStep(this)) {
        mContinuousJumpDelay = 15;
        al::getVelocityPtr(this)->add(mJumpForce);
        al::limitVelocityH(this, 20.0f);
        if (al::isNerve(this, &NrvBubble.HackJumpHigh)) {
            al::startHitReaction(this, "憑依振りジャンプ開始");
            al::startAction(this, "HackHighJump");
        } else {
            bool isHackCancelJump = al::isNerve(this, &NrvBubble.HackCancelJump);
            al::startHitReaction(this, "憑依ジャンプ開始");
            if (isHackCancelJump)
                al::startAction(this, "HackCancelJump");
            else
                al::startAction(this, "HackJump");
        }
        al::endBgmSituation(this, "HackBubble", false);
        mUpperPunchFreeze = 0;
        mPreviousTrans.set(al::getTrans(this));
        mGroundRotation.set(al::getQuat(this));

        sead::Vector3f upDir;
        al::calcUpDir(&upDir, this);
        if (upDir.y < 0.99f) {
            if (upDir.y < 0.0f) {
                al::makeQuatRotationRate(&prevQuat, upDir, -sead::Vector3f::ey, 1.0f);
                mGroundRotation = prevQuat * mGroundRotation;
                sead::Quatf rotateAxis;
                rotateAxis.setAxisRadian(sead::Vector3f::ey, -sead::Mathf::pi());
                mGroundRotation = mGroundRotation * rotateAxis;
            } else {
                al::makeQuatRotationRate(&prevQuat, upDir, sead::Vector3f::ey, 1.0f);
                mGroundRotation = prevQuat * mGroundRotation;
            }
        }
        prevQuat = al::getQuat(this) * quatZY;
        al::setQuat(this, prevQuat);
    }
    // Note: This overrides any data that prevQuat or deltaQuat has
    revertTargetQuatInHackJump(&prevQuat, &deltaQuat);
    al::getVelocityPtr(this)->y -= gravity;
    sead::Vector3f frontDir;
    al::calcFrontDir(&frontDir, this);
    sead::Vector3f hackerMoveVec = {0.0f, 0.0f, 0.0f};
    calcHackerMoveVec(&hackerMoveVec, sead::Vector3f::ey);

    bool flipDirection = false;
    sead::Vector3f hackerMoveDir = {0.0f, 0.0f, 0.0f};
    if (al::tryNormalizeOrZero(&hackerMoveDir, hackerMoveVec)) {
        // if angle between 140 and 220 degree => 180 +/- 40
        if (frontDir.dot(hackerMoveDir) < sead::Mathf::cos(sead::Mathf::deg2rad(220.0f))) {
            sead::Vector3f vel = al::getVelocity(this);
            vel.y = 0.0f;
            if (vel.dot(hackerMoveDir) > 0.0f || al::isNearZero(vel, 1.0f)) {
                al::addVelocity(this, -vel * 0.08f + hackerMoveDir * 0.57f);
                flipDirection = true;
            } else {
                al::scaleVelocityHV(this, 0.85f, 1.0f);
                flipDirection = true;
            }
        } else {
            al::turnToDirection(this, hackerMoveDir, 5.0f);

            al::calcFrontDir(&frontDir, this);

            sead::Vector3f velH;
            al::verticalizeVec(&velH, sead::Vector3f::ey, al::getVelocity(this));
            al::addVelocity(this, -velH);
            f32 frontHSpeed = frontDir.dot(velH);
            sead::Vector3f velHtoFront = (velH - frontDir * frontHSpeed) * 0.98f;

            f32 newFrontSpeed =
                frontHSpeed * 0.92f +
                frontAccel * sead::Mathf::clamp(frontDir.dot(hackerMoveDir), 0.0f, 1.0f);

            al::addVelocity(this, velHtoFront +
                                      frontDir * sead::Mathf::clampMax(newFrontSpeed, frontHSpeed));
            flipDirection = false;
        }
    }

    mGroundRotation.set(al::getQuat(this));
    makeDisplayQuatInHackJump(prevQuat, deltaQuat, quatZY, flipDirection);

    if (isDropAttackCollision()) {
        if (!al::sendMsgEnemyAttackFire(al::getCollidedGroundSensor(this),
                                        al::getHitSensor(this, "Attack"), "LavaRed")) {
            if (!rs::sendMsgBubbleAttack(al::getCollidedGroundSensor(this),
                                         al::getHitSensor(this, "Attack"))) {
                if (rs::sendMsgBubbleReflectV(al::getCollidedGroundSensor(this),
                                              al::getHitSensor(this, "Attack"))) {
                    mJumpForce = {0.0f, 15.0f, 0.0f};
                    al::setVelocityZeroV(this);
                    al::setNerve(this, &NrvBubble.HackJump);
                    return;
                }
            }
        }
    }

    if (isRiseAttackCollision()) {
        if (al::sendMsgEnemyAttackFire(al::getCollidedCeilingSensor(this),
                                       al::getHitSensor(this, "Attack"), "LavaRed"))
            al::setVelocityZeroV(this);
        else if (isHoldHackJump() && al::sendMsgEnemyUpperPunch(al::getCollidedCeilingSensor(this),
                                                                al::getHitSensor(this, "Attack")))
            mUpperPunchFreeze = 20;
        else
            al::setVelocityZeroV(this);

        if (mUpperPunchFreeze != 0) {
            mUpperPunchFreeze--;
            al::getVelocityPtr(this)->y += gravity;
        }
    } else {
        mUpperPunchFreeze = 0;
    }

    if (al::isCollidedWall(this)) {
        rs::sendMsgBubbleWallTouch(al::getCollidedWallSensor(this),
                                   al::getHitSensor(this, "Attack"));
    }

    tryHitReactionThroughFence();
    tryShiftContinuousJump() || tryShiftLand();
}

void Bubble::endHackJump() {
    al::setQuat(this, mGroundRotation);
}

void Bubble::exeHackLand() {
    if (al::isFirstStep(this)) {
        if (al::isActionPlaying(this, "HackStart"))
            al::startAction(this, "LandFrontUp");
        else if (al::isActionPlaying(this, "HackStartDown"))
            al::startAction(this, "LandFrontDown");
        else
            al::startAction(this, "Land");

        if (!trySendMsgStartInSaucePan())
            al::startHitReaction(this, "憑依着地");

        sead::Vector3f moveDir;
        if (mIsPlayerCaptured) {
            moveDir.set(0.0f, 0.0f, 0.0f);
            sead::Vector3f* velPtr = al::getVelocityPtr(this);
            al::parallelizeVec(velPtr, mFireNormal, *velPtr);
        } else if (!rs::calcHackerMoveDir(&moveDir, mPlayerHack, sead::Vector3f::ey)) {
            sead::Vector3f* velPtr = al::getVelocityPtr(this);
            al::parallelizeVec(velPtr, mFireNormal, *velPtr);
        }

        al::tryAddRippleMiddle(this);
        mAirTime = 0;
        al::startBgmSituation(this, "HackBubble", false, true);
        mUpDir = mFireNormal;
        mHackTurnFrame = 0;
        mHackTurnDelay = 60;
    }

    updateHackOnGround();
    if (!constrainLavaDomain())
        return;

    if (tryShiftFall())
        return;

    if (al::isGreaterEqualStep(this, 1) && isTriggerHackSwing()) {
        mJumpForce.set({0.0f, 25.0f, 0.0f});
        al::setNerve(this, &NrvBubble.HackJumpHigh);
        return;
    }

    if (isTriggerHackJump()) {
        mJumpForce.set({0.0f, 45.0f, 0.0f});
        al::setNerve(this, &NrvBubble.HackJump);
        return;
    }

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvBubble.HackMove);
}

void Bubble::exeHackInLauncher() {
    tryHitReactionThroughFence();
    bool isHack2 = isHack();
    if (!isHack2 && mIsShowTutorial) {
        rs::showTutorial(this);
        mIsShowTutorial = false;
    }

    if (tryShiftLand()) {
        mBubbleStateInLauncher->kill();
        sead::Vector3f initialDirection = sead::Vector3f::ey;
        initialDirection.rotate(al::getQuat(this));

        sead::Quatf quat;
        al::makeQuatRotationRate(&quat, initialDirection, sead::Vector3f::ey, 1.0f);
        mGroundRotation = quat * al::getQuat(this);

        if (isOnDamageFire())
            al::startHitReaction(this, "バブルキャノン着地");
        else if (!isHack2)
            al::startHitReaction(this, "地面での消滅");
        return;
    }

    if (al::updateNerveState(this)) {
        if (mPlayerHack) {
            mJumpForce.set(0.0f, 0.0f, 0.0f);
            al::startHitReaction(this, "バブルキャノン着地");
            al::setNerve(this, &NrvBubble.HackJump);
        } else {
            al::setNerve(this, &NrvBubble.WaitHackFall);
        }
    }
}

void Bubble::endHackInLauncher() {
    if (mLaunchCameraTicket) {
        al::endCamera(this, mLaunchCameraTicket, -1, false);
        mLaunchCameraTicket = nullptr;
    }

    if (mIsShowTutorial) {
        rs::showTutorial(this);
        mIsShowTutorial = false;
    }
}

void Bubble::exeHackResetPos() {
    sead::Quatf prevQuat;
    sead::Quatf deltaQuat;
    sead::Quatf quatZY = getQuatZY();

    if (al::isFirstStep(this)) {
        sead::Vector3f deltaPos = mResetTargetPos - al::getTrans(this);
        sead::Vector3f newVelocity = deltaPos;
        newVelocity.y = 0.0f;
        normalize2(&newVelocity, newVelocity.length() / 180.0f);
        newVelocity.y = deltaPos.y / 180.0f + 99.0f;

        al::setVelocity(this, newVelocity);
        al::startAction(this, "HackJump");
        sead::Vector3f direction = al::getTrans(this) - mResetTargetPos;
        al::tryNormalizeOrZero(&direction);
        al::faceToDirection(this, direction);
        mPreviousTrans.set(al::getTrans(this));
        mGroundRotation.set(al::getQuat(this));

        al::setQuat(this, al::getQuat(this) * quatZY);
    }

    al::addVelocityToGravity(this, 1.1f);

    revertTargetQuatInHackJump(&prevQuat, &deltaQuat);
    makeDisplayQuatInHackJump(prevQuat, deltaQuat, quatZY, true);

    if (al::isGreaterEqualStep(this, 180)) {
        sead::Vector3f firePos;
        sead::Vector3f fireNormal;
        if (!al::calcFindFireSurface(&firePos, &fireNormal, this, al::getTrans(this),
                                     sead::Vector3f::ey, 800.0f)) {
            mJumpForce.set(0.0f, 0.0f, 0.0f);
            al::setVelocityZero(this);
            al::setNerve(this, &NrvBubble.HackJump);
        } else {
            sead::Vector3f trans = al::getTrans(this);
            trans.y = firePos.y;
            al::resetPosition(this, trans);
            mJumpForce.set(0.0f, 0.0f, 0.0f);
            al::setVelocityZero(this);
            mIsOnLavaSurface = true;
            al::tryUpdateEffectMaterialCode(this, al::getFireMaterialCode(this));
            mFireNormal.set(fireNormal);
            al::setNerve(this, &NrvBubble.HackLand);
        }
    }
}

void Bubble::exeHackDemo() {}

void Bubble::exeRevive() {
    if (al::isFirstStep(this)) {
        rs::startReset(this);
        al::stopAction(this);
        al::resetPosition(this, mRevivePosition);
        al::setQuat(this, mStartingRotation);
        mGroundRotation.set(mStartingRotation);
        mStartActionPosition = mReviveStartActionPosition;
        mAnimScaleController->resetScale();
        mIsInSaucePan = false;
    }

    if (mReviveDelayCount == mReviveDelayTime - 1) {
        rs::endReset(this);
        al::setNerve(this, &NrvBubble.Ready);
        return;
    }
}
