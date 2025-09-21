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
    : al::LiveActor{name}, mAnimScaleController{new al::AnimScaleController(&gAnimScaleParam)} {}

void Bubble::init(const al::ActorInitInfo& initInfo) {
    using BubbleFunctor = al::FunctorV0M<Bubble*, void (Bubble::*)()>;

    al::initActorWithArchiveName(this, initInfo, "Bubble", nullptr);

    sead::Vector3f linksTrans;
    if (al::tryGetLinksTrans(&linksTrans, initInfo, "MoveNext")) {
        mJumpHeight = sead::Mathf::abs(linksTrans.y - al::getTrans(this).y);
        if (linksTrans.y < al::getTrans(this).y)
            al::getTransPtr(this)->set(linksTrans);
    } else {
        mJumpHeight = 800.0f;
    }

    if (al::isExistRail(this)) {
        al::setSyncRailToStart(this);
        mRailTotalLength = al::getRailTotalLength(this);

        if (!al::tryGetArg(&mRailMoveFrame, initInfo, "RailMoveFrame") || mRailMoveFrame < 1) {
            f32 railMoveSpeed = 10.0f;
            al::tryGetArg(&railMoveSpeed, initInfo, "RailMoveSpeed");
            if (railMoveSpeed <= 0.0)
                railMoveSpeed = 10.0;
            f32 frame = mRailTotalLength / railMoveSpeed;
            s32 foffset = (s32)frame == frame ? 0 : (u32)(0.0f <= frame);
            mRailMoveFrame = foffset + (s32)frame;
        }
    }

    mCurrentPosition = al::getTrans(this);
    mStartingRotation.set(al::getQuat(this));
    mResetPosition.set(al::getTrans(this));
    mRevivePosition.set(mCurrentPosition);
    mTurnFrameDelay = mJumpHeight / 13.0f;
    recalcClippingInfo();

    al::tryGetArg(&mWaitFrameNum, initInfo, "WaitFrameNum");
    if (mWaitFrameNum < 0)
        mWaitFrameNum = 0;

    al::tryGetArg(&mDelayFrameNum, initInfo, "DelayFrameNum");
    if (mDelayFrameNum < 0)
        mDelayFrameNum = 0;

    al::tryGetArg(&mIsWaveCheckOn, initInfo, "IsWaveCheckOn");
    mWaitDelayTime = mTurnFrameDelay * 2 + mWaitFrameNum + 80;

    if (al::isExistRail(this)) {
        f32 travelTime = mRailMoveFrame + mWaitDelayTime + 1;
        mWaitDelayTime = travelTime * 2;
    }

    f32 shootDegree;
    al::tryGetArg(&shootDegree, initInfo, "ShootDegree");
    al::initNerve(this, &NrvBubble.StandBy, 1);
    mBubbleStateInLauncher = new BubbleStateInLauncher(this);
    al::initNerveState(this, mBubbleStateInLauncher, &NrvBubble.HackInLauncher, "バブルランチャー");
    al::hideModel(this);

    if (!al::listenStageSwitchOnStart(this, BubbleFunctor(this, &Bubble::start)))
        start();

    al::listenStageSwitchOnKill(this, BubbleFunctor(this, &Bubble::killAll));
    mCapTargetInfo = rs::createCapTargetInfo(this, nullptr);

    bool stageStartHack = false;
    al::tryGetArg(&stageStartHack, initInfo, "StageStartHack");
    if (stageStartHack) {
        rs::requestStageStartHack(this, al::getHitSensor(this, "Cap"), mCapTargetInfo, nullptr);
    } else {
        al::PlacementInfo placementInfo;
        if (al::tryGetLinksInfo(&placementInfo, initInfo, "MoveNext"))
            al::tryGetArg(&stageStartHack, placementInfo, "StageStartHack");
        if (stageStartHack)
            rs::requestStageStartHack(this, al::getHitSensor(this, "Cap"), mCapTargetInfo, nullptr);
    }

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
    mProveActor = new al::LiveActor("グループ同期");

    al::initActorWithArchiveName(mProveActor, initInfo, "Bubble", "Probe");

    al::setClippingInfo(mProveActor, al::getClippingRadius(this), &mClippingPos);
    mProveActor->makeActorAlive();

    if (al::isExistRail(this) && al::isExistLinkChild(initInfo, "TurnTarget", 0)) {
        sead::Vector3f trans;
        al::tryGetLinksQT(&mRailFinalRotation, &trans, initInfo, "TurnTarget");
    } else {
        mRailFinalRotation.set(mStartingRotation);
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
            al::isSensorName(self, "CapBelow") || !al::isSensorName(self, "PechoAttack")) {
            return;
        }

        rs::sendMsgBubbleAttackToPecho(other, self);
        return;
    }

    if (!al::isNerve(this, &NrvBubble.HackJump) && !al::isNerve(this, &NrvBubble.HackJumpHigh) &&
        !al::isNerve(this, &NrvBubble.HackCancelJump) && !al::isNerve(this, &NrvBubble.HackFall) &&
        rs::sendMsgBubbleReflectH(other, self)) {
        sead::Vector3f distance = al::getSensorPos(self) - al::getSensorPos(other);
        distance.y = 0;
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
            sead::Vector3f fireA;
            sead::Vector3f fireB;
            if (al::calcFindFireSurface(&fireA, &fireB, this, mResetTargetPos, sead::Vector3f::ey,
                                        800.0f)) {
                sead::Vector3f firepos = mResetTargetPos;
                firepos.y = fireA.y;
                al::resetPosition(this, firepos);
                al::faceToTarget(this, endTargetPos);
                mJumpForce.set({0.0f, 0.0f, 0.0f});
                al::setVelocityZero(this);
                mIsInFire = true;
                al::tryUpdateEffectMaterialCode(this, al::getFireMaterialCode(this));
                mIsInBossSequence = false;
                mFireSurface.set(fireB);
                al::setNerve(this, &NrvBubble.HackMove);
            } else {
                mJumpForce.set({0.0f, 0.0f, 0.0f});
                al::setVelocityZero(this);
                al::setNerve(this, &NrvBubble.HackJump);
            }
            al::showModel(this);
            al::showSilhouetteModel(this);
            showHackCap();

            return true;
        }

        if (rs::isMsgBossMagmaQueryToBubble(message))
            return !mIsInFire;
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
        mJumpForce.set({0.0f, 0.0f, 0.0f});
        al::onCollide(this);
        mWaitDelay = 5;
        al::setVelocityZero(this);
        al::setNerve(this, &NrvBubble.WaitHackFall);
        return true;
    }

    if (!rs::isMsgStartHack(message)) {
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
            if (al::isNerve(this, &NrvBubble.HackInLauncher) ||
                al::isNerve(this, &NrvBubble.HackDemo))
                return false;

            rs::syncDamageVisibility(this, mPlayerHack);
            return true;
        }

        sead::Vector3f breathForce;
        if (rs::tryGetBossMagmaBreathForce(message, &breathForce)) {
            if (al::isSensorName(self, "Cap")) {
                sead::Vector3f force = breathForce;
                if (al::tryNormalizeOrZero(&force)) {
                    f32 forceLength = force.dot(al::getVelocity(this));
                    if (breathForce.length() > forceLength)
                        al::addVelocity(this, breathForce * 0.05f);
                }
                return true;
            }
            return false;
        }

        if (rs::isMsgBubbleLauncherStart(message)) {
            if (al::isSensorName(self, "Body")) {
                if (al::isNerve(this, &NrvBubble.HackJump) ||
                    al::isNerve(this, &NrvBubble.HackJumpHigh) ||
                    al::isNerve(this, &NrvBubble.HackCancelJump)) {
                    al::setNerve(this, &NrvBubble.HackInLauncher);
                    return true;
                }

                if (!al::isNerve(this, &NrvBubble.HackInLauncher) || mPlayerHack == nullptr)
                    return false;

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

            if (!rs::isMsgHackerDamageAndCancel(message) || rs::isMsgFireDamageAll(message))
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

        if (!al::isNoCollide(this) &&
            al::tryReceiveMsgPushAndAddVelocityH(this, message, other, self, 3.0f))
            return true;

        if (!rs::isMsgKillByShineGet(message) && !rs::isMsgKillByHomeDemo(message) &&
            !rs::isMsgKillByMoonRockDemo(message)) {
            if (!rs::isMsgHammerBrosHammerHackAttack(message) &&
                !rs::isMsgHammerBrosHammerEnemyAttack(message))
                return false;

            if (!al::isNerve(this, &NrvBubble.Up) && !al::isNerve(this, &NrvBubble.Turn) &&
                !al::isNerve(this, &NrvBubble.Down))
                return false;

            rs::requestHitReactionToAttacker(message, self, other);
            return true;
        }

        al::tryKillEmitterAndParticleAll(this);
        revive();
        return true;
    }

    if (!isCurrentNerveEnableLockOnAndStartHack())
        return false;

    if (al::isSensorName(self, "Cap") || al::isSensorName(self, "CapAbove") ||
        al::isSensorName(self, "CapBelow")) {
        startHackLocal(self, other);
        return true;
    }

    return false;
}

void Bubble::control() {
    mDisregardReceiver->set44(al::getSensorPos(this, "Body") +
                              al::getSensorRadius(this, "Body") * sead::Vector3f::ey);

    if (isValidCollisionOrWaveCheck()) {
        if (mIsInFire) {
            mLavaSurfaceMtx.makeQT(al::getQuat(this), {0.0f, 0.0f, 0.0f});
            mLavaSurfaceMtx.setBase(3, al::getTrans(this));
        } else if (al::isOnGround(this, 0)) {
            mLavaSurfaceMtx.makeQT(al::getQuat(this), {0.0f, 0.0f, 0.0f});

            sead::Vector3f trans = al::getTrans(this);
            trans.y = al::getCollidedGroundPos(this).y;
            mLavaSurfaceMtx.setBase(3, trans);
        }
    } else {
        mLavaSurfaceMtx.makeQT(al::getQuat(this), {0.0f, 0.0f, 0.0f});
        if (al::isExistRail(this))
            al::isRailGoingToEnd(this);
        al::getTrans(this);
        mLavaSurfaceMtx.setBase(3, al::getTrans(this));
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
        mIsInFire = false;

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
        (!isHack() || !rs::isActiveHackStartDemo(mPlayerHack)) && !al::isClipped(mProveActor) &&
        !mIsClipped) {
        mWaitDelayCount = al::modi(mWaitDelayCount + mWaitDelayTime + 1, mWaitDelayTime);
    }

    mIsClipped = al::isClipped(mProveActor);
    if (!isHack())
        al::scaleVelocityHV(this, 0.9f, 1.0f);
    mCameraMtx.makeQT(mCurrentRotation, {0.0f, 0.0f, 0.0f});

    mCameraMtx.setBase(3, al::getTrans(this) + (0.0f * sead::Vector3f::ey));
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
            rs::solveCollisionInHacking(this, al::getVelocity(this) + mColliderPos);
        else
            al::getTransPtr(this)->add(
                getCollider()->collide(al::getVelocity(this) + mColliderPos));
    }

    mColliderPos.set({0.0f, 0.0f, 0.0f});
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

// NON_MATCHING: mFireSurface stored differently https://decomp.me/scratch/ffVkS
void Bubble::recalcClippingInfo() {
    sead::Vector3f max = mCurrentPosition;
    sead::Vector3f min = mCurrentPosition;
    al::updateBoundingBox(mJumpHeight * sead::Vector3f::ey + mCurrentPosition, &max, &min);

    if (al::isExistRail(this)) {
        s32 pointNum = al::getRailPointNum(this);
        for (s32 i = 0; i < pointNum; i++) {
            sead::Vector3f pos;
            al::calcRailPointPos(&pos, this, i);
            al::updateBoundingBox(pos, &max, &min);
        }

        f32 railCoord = al::calcRailCoordByPoint(this, 0);
        for (s32 i = 1; i < pointNum; i++) {
            f32 currentRailCoord = al::calcRailCoordByPoint(this, i);
            sead::Vector3f pos;
            al::calcRailPosAtCoord(&pos, this, (railCoord + currentRailCoord) * 0.5f);
            al::updateBoundingBox(pos, &max, &min);
            railCoord = currentRailCoord;
        }
    }

    mClippingPos = (max + min) * 0.5f;
    al::setClippingInfo(this, (max - min).length() * 0.5f + 200.0f, &mClippingPos);
}

void Bubble::start() {
    if (!al::isNerve(this, &NrvBubble.StandBy))
        return;

    al::setTrans(this, mCurrentPosition);
    al::hideModelIfShow(this);
    mWaitDelayCount = 0;

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
            this, mSurfaceProvePos, &triangle, al::getTrans(this) + 100.0f * sead::Vector3f::ey,
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

    if (alCollisionUtil::getFirstPolyOnArrow(this, &mSurfaceProvePos[1], nullptr,
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

void Bubble::updateLavaWave() {
    sead::Vector3f colliderY =
        (al::getColliderRadius(this) - al::getColliderOffsetY(this)) * sead::Vector3f::ey;

    sead::Vector3f colliderPos = al::getTrans(this) - colliderY;
    sead::Vector3f checkPos = colliderPos - 200.0f * sead::Vector3f::ey;

    bool isInFire = al::isInFirePos(this, checkPos);

    sead::Vector3f fireSurface;
    al::calcFindFireSurface(&checkPos, &fireSurface, this, colliderPos, sead::Vector3f::ey, 200.0f);
    checkPos.x = colliderPos.x;
    checkPos.z = colliderPos.z;

    if (al::isOnGround(this, 0)) {
        if (!isInFire || checkPos.y < al::getTrans(this).y - al::getColliderRadius(this) +
                                          al::getColliderOffsetY(this)) {
            mIsInFire = false;
            return;
        }
    }

    if (isEnableSnapWaveSurface()) {
        if (isInFire) {
            al::getTransPtr(this)->set(colliderY + checkPos);
            mFireSurface.set(fireSurface);
            al::tryUpdateEffectMaterialCode(this, al::getFireMaterialCode(this));
            return;
        }
    } else if (isInFire && checkPos.y >= colliderPos.y) {
        al::getTransPtr(this)->set(colliderY + checkPos);
        mIsInFire = true;
        mFireSurface.set(fireSurface);
        al::tryUpdateEffectMaterialCode(this, al::getFireMaterialCode(this));
        return;
    }

    mIsInFire = false;
}

void Bubble::updateScrollAnimRate() {
    if (al::isActionPlaying(this, mActionName))
        return;

    mActionName = al::getActionName(this);

    if (isHack() &&
        (al::isActionPlaying(this, "BubbleCannonJump") ||
         al::isActionPlaying(this, "HackHighJump") || al::isActionPlaying(this, "HackWaitSpeedy") ||
         al::isActionPlaying(this, "MoveSpeedy"))) {
        al::setMtsAnimFrameRate(this, 2.0f);
        return;
    }

    al::setMtsAnimFrameRate(this, 1.0f);
}

bool Bubble::isOnGround() const {
    if (al::isOnGround(this, 0))
        return true;
    return mIsInFire;
}

inline s32 getSurfaceProveIndex(Bubble* bubble) {
    s32 index = 0;
    if (al::isExistRail(bubble) && !al::isRailGoingToEnd(bubble))
        index = 1;
    return index;
}

void Bubble::setupHack() {
    if (!isValidCollisionOrWaveCheck()) {
        f32 provePos = mSurfaceProvePos[getSurfaceProveIndex(this)].y + al::getColliderRadius(this);
        if (al::getTrans(this).y < provePos)
            al::getTransPtr(this)->y = provePos;
    }

    al::invalidateClipping(this);
    offGroupClipping();
    mJumpForce = {0.0f, 0.0f, 0.0f};
    al::onCollide(this);
    mWaitDelay = 5;
    al::setVelocityZero(this);
    mIsPlayerCaptured = false;
    mJointSpringControllerHolder->onControlAll();
}

bool Bubble::isCurrentNerveEnableLockOnAndStartHack() const {
    if (al::isHideModel(this))
        return false;

    if (al::isNerve(this, &NrvBubble.Up)) {
        if (al::getTrans(this).y - mCurrentPosition.y > al::getColliderRadius(this))
            return true;
    }

    if (al::isNerve(this, &NrvBubble.Turn))
        return true;

    if (al::isNerve(this, &NrvBubble.Down) &&
        al::getTrans(this).y - mCurrentPosition.y > al::getColliderRadius(this))
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

    if (mCameraTicket && al::isActiveCamera(mCameraTicket)) {
        al::endCamera(this, mCameraTicket, -1, false);
        mCameraTicket = nullptr;
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
        velocityV.set({0.0f, 0.0f, 0.0f});

    velocityV -= 1.1f * velocity;
    al::setVelocity(this, velocityH + velocityV);
}

bool Bubble::isOnDamageFire() const {
    return mIsInFire || al::isCollidedGroundFloorCode(this, "DamageFire");
}

void Bubble::tryStartHitReactionUp() {
    if (mIsOnHitReactionEffect || mIsCheckEffectPosRequired)
        return;

    s32 index = getSurfaceProveIndex(this);

    if (mSurfaceProvePos[index].y < al::getTrans(this).y) {
        al::startHitReactionHitEffect(this, "発射",
                                      mSurfaceProvePos[index] + 0.0f * sead::Vector3f::ey);
        mIsOnHitReactionEffect = true;
    }
}

void Bubble::updateShadowMaskLength() {
    f32 newLength = sead::Mathf::clamp(al::getTrans(this).y - mCurrentPosition.y + 300.0f, 300.0f,
                                       mShadowMaskDropLength);
    al::setShadowMaskDropLength(this, newLength);
}

void Bubble::tryStartHitReactionDown() {
    if (mIsOnHitReactionEffect || mIsCheckEffectPosRequired)
        return;

    s32 index = getSurfaceProveIndex(this);

    if (mSurfaceProvePos[index].y > al::getTrans(this).y) {
        al::startHitReactionHitEffect(this, "着地",
                                      mSurfaceProvePos[index] + 0.0f * sead::Vector3f::ey);
        mIsOnHitReactionEffect = true;
    }
}

void Bubble::shiftSink() {
    sead::Vector3f hitEffect;
    if (mIsCheckEffectPosRequired || mIsOnHitReactionEffect) {
        if (!isValidCollisionOrWaveCheck() || trySendMsgStartInSaucePan()) {
            al::setNerve(this, &NrvBubble.Sink);
            return;
        }

        hitEffect = al::getTrans(this) - (al::getColliderRadius(this) + 0.0f) * sead::Vector3f::ey;
    } else {
        s32 index = getSurfaceProveIndex(this);
        if (trySendMsgStartInSaucePan()) {
            al::setNerve(this, &NrvBubble.Sink);
            return;
        }

        hitEffect = mSurfaceProvePos[index] + 0.0f * sead::Vector3f::ey;
    }
    al::startHitReactionHitEffect(this, "着地", hitEffect);
    al::setNerve(this, &NrvBubble.Sink);
}

void Bubble::headUp() {
    sead::Vector3f upDir;
    al::calcUpDir(&upDir, this);
    if (upDir.y > 0.99f)
        return;

    sead::Quatf quat;
    al::makeQuatRotationRate(&quat, upDir, sead::Vector3f::ey, 1.0f);
    sead::QuatCalcCommon<f32>::setMul(quat, quat, al::getQuat(this));
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
    if (!al::isOnGround(this, 0) && !(mIsInFire && al::getVelocity(this).y < 0.0f))
        return false;

    if (!isOnDamageFire()) {
        forceEndHack();
        return true;
    }

    if (al::isOnGround(this, 0))
        mFireSurface.set(al::getOnGroundNormal(this, 0));
    sead::Quatf quat;
    al::makeQuatRotationRate(&quat, sead::Vector3f::ey, mFireSurface, 1.0);
    al::getVelocityPtr(this)->rotate(quat);
    sead::Vector3f* velPtr = al::getVelocityPtr(this);
    al::verticalizeVec(velPtr, mFireSurface, *velPtr);

    if (mPlayerHack) {
        mShiftFallDelay = 0;
        mLandPos.set({0.0f, 0.0f, 0.0f});
        updateCollisionPartsMove();
        al::getVelocityPtr(this)->add(mLandPos);
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

// NON_MATCHING: https://decomp.me/scratch/VidWI
bool Bubble::tryBoundMoveWall() {
    if (!al::isCollidedWall(this) || !al::isCollidedGround(this))
        return false;

    al::CollisionParts* collisionParts = al::getCollidedWallCollisionParts(this);
    if (collisionParts->get_15c() != 0 && !collisionParts->isMoving())
        return false;

    sead::Vector3f wallNormal = al::getCollidedWallNormal(this);
    const sead::Vector3f& groundNormal = al::getCollidedGroundNormal(this);
    if (groundNormal.dot(wallNormal) < 0.1736482f)  // cos(80°)
        return false;

    al::verticalizeVec(&wallNormal, groundNormal, wallNormal);
    if (!al::tryNormalizeOrZero(&wallNormal))
        return false;

    sead::Vector3f transMtx =
        collisionParts->getBaseMtx().getBase(3) - collisionParts->getPrevBaseMtx().getBase(3);
    al::verticalizeVec(&transMtx, groundNormal, transMtx);

    f32 lengthMtx = transMtx.length();
    if (al::isNearZero(lengthMtx))
        return false;

    const f32 inv = 1.0f / lengthMtx;
    transMtx.x *= inv;
    transMtx.y *= inv;
    transMtx.z *= inv;

    if (transMtx.dot(wallNormal) < 0.7071068f)  // cos(45°)
        return false;

    sead::Vector3f parallel;
    sead::Vector3f vertical;
    al::separateVectorParallelVertical(&parallel, &vertical, transMtx, al::getVelocity(this));
    if (parallel.dot(transMtx) >= lengthMtx * 5.0f)
        return false;

    al::getVelocityPtr(this)->set(5.0f * lengthMtx * transMtx + vertical);
    mAnimScaleController->startHitReaction();
    return true;
}

// NON_MATCHING: Stack issues https://decomp.me/scratch/d5cE6
void Bubble::updateHackOnGround() {
    static sead::Vector3f magic = {0.0f, 0.0f, 0.0f};

    *al::getVelocityPtr(this) -= mLandPos;
    if (!al::isOnGroundNoVelocity(this, 0)) {
        if (mIsInFire) {
            mShiftFallDelay = 0;
            sead::Vector3f* velPtr = al::getVelocityPtr(this);
            al::verticalizeVec(velPtr, sead::Vector3f::ey, *velPtr);
        } else {
            sead::Vector3f arrow;
            sead::Vector3f upDir;
            al::calcUpDir(&upDir, this);

            sead::Vector3f dir = al::getTrans(this) + al::getColliderOffsetY(this) * upDir;

            if (alCollisionUtil::getFirstPolyOnArrow(this, &arrow, nullptr, dir,
                                                     (-50.0f - al::getColliderRadius(this)) *
                                                         sead::Vector3f::ey,
                                                     mCollisionPartsFilter, nullptr)) {
                mColliderPos = arrow - (dir - al::getColliderRadius(this) * sead::Vector3f::ey);
            }
            mShiftFallDelay++;
        }
    } else if (mIsInFire) {
        mShiftFallDelay = 0;
        sead::Vector3f* velPtr = al::getVelocityPtr(this);
        al::verticalizeVec(velPtr, sead::Vector3f::ey, *velPtr);
    } else {
        mShiftFallDelay = 0;
        mFireSurface.set(al::getOnGroundNormal(this, 0));
        sead::Vector3f* velPtr = al::getVelocityPtr(this);
        al::verticalizeVec(velPtr, mFireSurface, *velPtr);
    }
    updateCollisionPartsMove();
    al::setQuat(this, mCurrentRotation);
    accelStick();
    mCurrentRotation.set(al::getQuat(this));
    sead::Vector3f updir2;
    al::calcUpDir(&updir2, this);

    sead::Quatf quat;
    al::makeQuatRotationRate(&quat, updir2, mUpDir, 1.0f);
    quat *= al::getQuat(this);
    al::setQuat(this, quat);

    sead::Quatf quat2;
    al::makeQuatRotationRate(&quat2, mUpDir, mFireSurface, 0.1f);
    sead::QuatCalcCommon<f32>::setMul(quat2, quat2, al::getQuat(this));
    al::setQuat(this, quat2);

    al::calcUpDir(&mUpDir, this);

    sead::Quatf quat3;
    al::makeQuatZDegree(&quat3, ((f32)mHackTurnFrame / (f32)mHackTurnDelay) * mHackTurnAngle);
    quat3.normalize();

    sead::Quatf quat4;
    sead::QuatCalcCommon<f32>::setMul(quat4, al::getQuat(this), quat3);
    al::setQuat(this, quat4);
    al::getVelocityPtr(this)->add(mLandPos);
}

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
    sead::Vector3f frontDir;
    al::calcFrontDir(&frontDir, this);
    frontDir.y = 0.0;
    al::tryNormalizeOrDirZ(&frontDir);
    frontDir *= colliderRadius;

    sead::Vector3f crossDir;
    crossDir.setCross(sead::Vector3f::ey, frontDir);
    normalize2(&crossDir, colliderRadius);

    sead::Vector3f genDir = al::getTrans(this) + colliderRadius * sead::Vector3f::ey;
    sead::Vector3f dir[4];
    dir[0] = genDir + (-crossDir + frontDir);
    dir[1] = genDir + (crossDir + frontDir);
    dir[2] = genDir + (-crossDir - frontDir);
    dir[3] = genDir + (crossDir - frontDir);
    sead::Vector3f posA = sead::Vector3f::ey * (colliderRadius * -3.0f);

    bool isFire[4] = {false, false, false, false};
    bool isPoly[4] = {false, false, false, false};
    f32 height[4] = {0.0f, 0.0f, 0.0f, 0.0f};

    for (s32 i = 0; i < 4; i++) {
        sead::Vector3f arrow;
        al::Triangle triangle;
        if (alCollisionUtil::getFirstPolyOnArrow(this, &arrow, &triangle, dir[i], posA,
                                                 mCollisionPartsFilter, nullptr)) {
            height[i] = arrow.y;
            isFire[i] = al::isFloorCode(triangle, "DamageFire");
            isPoly[i] = true;
        }
    }

    sead::Vector3f fireSurfaceB;
    sead::Vector3f arrow;
    for (s32 i = 0; i < 4; i++) {
        if (!al::calcFindFireSurface(&fireSurfaceB, &arrow, this, dir[i], sead::Vector3f::ey,
                                     200.0f))
            continue;
        if (isPoly[i] && height[i] > fireSurfaceB.y)
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

    u32 hitCount = 0;
    sead::Vector3f hitDirection = {0.0f, 0.0f, 0.0f};
    for (s32 i = 0; i < 4; i++) {
        if (isPoly[i] && !isFire[i]) {
            hitDirection += dir[i];
            hitCount++;
        }
    }

    if (hitCount != 0) {
        sead::Vector3f launchDirection = -(1.0f / hitCount) * hitDirection + al::getTrans(this);
        launchDirection.y = 0.0f;

        al::tryNormalizeOrZero(&launchDirection);
        if (launchDirection.dot(al::getVelocity(this)) > 0.0f) {
            sead::Vector3f* velPtr = al::getVelocityPtr(this);
            al::verticalizeVec(velPtr, launchDirection, *velPtr);
        }

        al::addVelocity(this, launchDirection * 3.0f);
    }

    return true;
}

bool Bubble::tryShiftFall() {
    if (mShiftFallDelay > 9) {
        mJumpForce.set({0.0f, 0.0f, 0.0f});
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

void Bubble::revertTargetQuatInHackJump(sead::Quatf* quatA, sead::Quatf* quatB) {
    quatA->set(al::getQuat(this));
    sead::Quatf invRotation;
    mCurrentRotation.inverse(&invRotation);
    quatB->set(invRotation * al::getQuat(this));
    quatB->normalize();
    al::setQuat(this, mCurrentRotation);
}

void Bubble::calcHackerMoveVec(sead::Vector3f* moveVec, const sead::Vector3f& inputDir) const {
    if (mIsPlayerCaptured) {
        moveVec->set({0.0f, 0.0f, 0.0f});
        return;
    }
    rs::calcHackerMoveVec(moveVec, mPlayerHack, inputDir);
}

// NON_MATCHING: Different quat multiplication https://decomp.me/scratch/s3det
void Bubble::makeDisplayQuatInHackJump(const sead::Quatf& quatA, const sead::Quatf& quatB,
                                       const sead::Quatf& quatC, bool isValue) {
    sead::Vector3f direction = al::getTrans(this) - mPreviousTrans;
    mPreviousTrans.set(al::getTrans(this));

    sead::Quatf quat = mCurrentRotation;
    if (al::tryNormalizeOrZero(&direction)) {
        f32 angle = al::calcAngleDegree(sead::Vector3f::ey, direction);
        if (isValue)
            angle = !(al::getVelocity(this).y >= 0.0f) ? 180.0f : 0.0f;
        sead::Vector3f dir = sead::Vector3f::ey;
        dir.rotate(quatA);
        dir.negate();
        sead::Quatf newQuat;
        al::makeQuatRotateDegree(
            &newQuat, sead::Vector3f::ex,
            sead::Mathf::max(al::calcAngleDegree(sead::Vector3f::ey, dir), angle));
        quat *= newQuat;
        quat *= quatC;
    } else {
        quat *= quatB;
    }

    sead::Quatf slerpQuat;
    al::slerpQuat(&slerpQuat, quatA, quat, 0.1f);
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
    if (al::getVelocity(this).y < 0.0f || !al::isCollidedCeiling(this))
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

    if (mJumpDelay < 0xf)
        mJumpDelay++;

    if (isTriggerHackSwing())
        mJumpDelay = 0;

    if (!isOnGround())
        return false;

    if (al::getVelocity(this).y >= 0.0f)
        return false;

    if (!isOnDamageFire())
        return false;

    if (mJumpDelay >= 0xf)
        return false;

    mJumpForce.set({0.0f, 25.0f, 0.0f});
    al::getVelocityPtr(this)->y = 0.0f;
    al::startHitReaction(this, "水切りジャンプ");
    al::setNerve(this, &NrvBubble.HackJumpHigh);

    if (al::isOnGround(this, 0))
        mFireSurface.set(al::getOnGroundNormal(this, 0));

    mShiftFallDelay = 0;
    mLandPos.set({0.0f, 0.0f, 0.0f});

    if (al::isCollidedGround(this)) {
        rs::sendMsgBubbleGroundTouchTrigger(al::getCollidedGroundSensor(this),
                                            al::getHitSensor(this, "Attack"));
    }

    al::tryAddRippleMiddle(this);
    return true;
}

bool Bubble::calcHackerMoveDir(sead::Vector3f* moveDir, const sead::Vector3f& inputDir) const {
    if (mIsPlayerCaptured) {
        moveDir->set({0.0f, 0.0f, 0.0f});
        return false;
    }
    return rs::calcHackerMoveDir(moveDir, mPlayerHack, inputDir);
}

bool Bubble::isWaitingLaunch() const {
    return al::isNerve(this, &NrvBubble.HackInLauncher) && mBubbleStateInLauncher->isWaiting();
}

void Bubble::launch(const sead::Vector3f& dir, f32 force, al::CameraTicket* cameraTicket) {
    if (!al::isNerve(this, &NrvBubble.HackInLauncher))
        return;

    mBubbleStateInLauncher->shoot(dir, force);
    if (cameraTicket)
        mCameraTicket = cameraTicket;
    rs::hideTutorial(this);
    mIsShowTutorial = true;
}

void Bubble::launchCancel(const sead::Vector3f& dir) {
    if (!al::isNerve(this, &NrvBubble.HackInLauncher))
        return;

    mBubbleStateInLauncher->kill();
    al::showModelIfHide(this);
    al::showSilhouetteModelIfHide(this);
    showHackCap();

    sead::Vector3f forceH = al::getTrans(this) - dir;
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
    mResetPosition.set(trans);
    mStartingRotation.set(quat);
    mCurrentPosition.set(trans);
    mRevivePosition.set(mCurrentPosition);
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
    sead::Vector3f colliderHitPos = colliderPos;
    colliderHitPos.y += colliderRadius;
    sead::Vector3f colliderHitRadius = colliderHitPos - colliderPos;

    alCollisionUtil::SphereMoveHitInfo sphereMoveHitInfo;

    if (alCollisionUtil::checkStrikeSphereMove(this, &sphereMoveHitInfo, 8, colliderPos,
                                               colliderRadius, colliderHitRadius, nullptr,
                                               nullptr) != 0) {
        colliderHitPos = sphereMoveHitInfo._0 * colliderHitRadius + colliderPos;
    }

    sead::Vector3f upDir;
    sead::Quatf rotationRate;
    al::calcUpDir(&upDir, this);
    al::makeQuatRotationRate(&rotationRate, upDir, sead::Vector3f::ey, 1.0f);

    rotationRate *= al::getQuat(this);
    rs::endHackFromTargetPos(&mPlayerHack, colliderHitPos, rotationRate, sead::Vector3f::zero);
    al::showModelIfHide(this);
    rs::endHackShadow(this);
    mJointSpringControllerHolder->offControlAll();
    mAnimScaleController->startHitReaction();
    al::endBgmSituation(this, "HackBubble", false);
}

void Bubble::calcLaunchPos(sead::Vector3f* pos, const sead::Vector3f& vecA, f32 valA,
                           f32 valB) const {
    // Remind me on how to get the fractional part
    s32 index = (s32)valB == valB ? 0 : -!(valB >= 0.0f);
    index += (s32)valB;
    f32 t = valB - index;

    sead::Vector3f startPos;
    sead::Vector3f endPos;
    mBubbleStateInLauncher->calcLaunchPos(&startPos, vecA, valA, index);
    mBubbleStateInLauncher->calcLaunchPos(&endPos, vecA, valA, index + 1);

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
    return al::isOnGroundNoVelocity(this, 0) || mIsInFire;
}

void Bubble::updateCollisionPartsMove() {
    al::CollisionParts* collisionParts = al::tryGetCollidedGroundCollisionParts(this);
    if (!collisionParts) {
        if (mIsInFire || mShiftFallDelay > 9)
            mLandPos.set({0.0f, 0.0f, 0.0f});
        return;
    }
    sead::Vector3f delta = al::getTrans(this);
    delta *= collisionParts->getPrevBaseInvMtx();
    delta *= collisionParts->getBaseMtx();
    mLandPos.set(delta - al::getTrans(this));
}

// NON_MATCHING: Bad branch order https://decomp.me/scratch/6PN8m
void Bubble::accelStick() {
    sead::Vector3f velocity;
    if (al::isOnGroundNoVelocity(this, 0))
        velocity.set(al::getOnGroundNormal(this, 0));
    else
        velocity.set(sead::Vector3f::ey);

    sead::Vector3f local_70 = mStickForce;
    if (al::tryNormalizeOrZero(&local_70)) {
        sead::Vector3f local_80;
        local_80.setCross(local_70, velocity);
        if (al::tryNormalizeOrZero(&local_80)) {
            f32 fVar8 = local_80.dot(al::getVelocity(this));
            *al::getVelocityPtr(this) -= fVar8 * local_80;
            *al::getVelocityPtr(this) += (fVar8 * 0.9f) * local_80;
        }
        f32 fVar8 = sead::Mathf::clampMax(
            mStickForce.length() - (local_70.dot(al::getVelocity(this))), 0.5f);
        if (fVar8 > 0.0f)
            al::getVelocityPtr(this)->add(fVar8 * local_70);
    }

    f32 fVar4 = velocity.dot(al::getVelocity(this));
    sead::Vector3f rate = velocity * fVar4;
    sead::Vector3f local_80 = (al::getVelocity(this) - rate) * 0.92f + (rate - velocity * 1.1f);
    al::setVelocity(this, local_80);
    mStickForce.set({0.0f, 0.0f, 0.0f});

    if (mIsPlayerCaptured) {
        mHackTurnAngle = mHackTurnAngle * 0.9f;
        return;
    }

    bool action = rs::isHoldHackAction(mPlayerHack);
    if (mIsPlayerCaptured) {
        mHackTurnAngle = mHackTurnAngle * 0.9f;
        return;
    }

    f32 fVar8 = action ? 1.6f : 1.4f;

    if (!rs::addHackActorAccelStick(this, mPlayerHack, &local_80, fVar8, velocity)) {
        mHackTurnAngle = mHackTurnAngle * 0.9f;
        return;
    }

    al::turnToDirection(this, local_80, 10.0f);

    sead::Vector3f ezy;
    ezy.setRotated(mCurrentRotation, sead::Vector3f::ez);
    local_80.y = 0.0f;

    if (!al::tryNormalizeOrZero(&local_80))
        local_80.set(ezy);

    sead::Vector3f cross = ezy.cross(local_80);
    f32 tan = atan2f(cross.length(), ezy.dot(local_80));

    f32 letal = (sead::Mathf::clamp(tan, 0.0f, 1.5707964f) * 72.0f) / 1.5707964f;
    f32 bzen = mHackTurnAngle;
    f32 notLetal = -letal;

    if (ezy.z * local_80.x - ezy.x * local_80.z <= 0.0f)
        notLetal = letal;

    f32 AAAA = 0.39999998f;
    f32 BBBB = 0.6f;
    if (bzen * notLetal >= 0.0f) {
        if (sead::Mathf::abs(bzen) < sead::Mathf::abs(notLetal)) {
            AAAA = 0.95f;
            BBBB = 0.05f;
        }
    }
    mHackTurnAngle = bzen * AAAA + notLetal * BBBB;
}

bool Bubble::addHackActorAccelStick(sead::Vector3f* stickAccel, f32 scale,
                                    const sead::Vector3f& dir) {
    if (mIsPlayerCaptured) {
        stickAccel->set({0.0f, 0.0f, 0.0f});
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
    return mIsInFire && !al::isNerve(this, &NrvBubble.HackJump) &&
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
            al::setQuat(this, mRailFinalRotation);
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
                                          mSurfaceProvePos[getSurfaceProveIndex(this)] +
                                              0.0f * sead::Vector3f::ey);
        }
        checkEffectPos();
        if (!isValidCollisionOrWaveCheck())
            al::tryUpdateEffectMaterialCode(this, mMaterialCode);
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
            mCurrentPosition.set(al::getTrans(this));
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
    sead::Vector3f nexPos = al::getTrans(this);
    nexPos.y =
        mCurrentPosition.y + al::calcNerveSquareOutValue(this, mTurnFrameCount, 0.0f, mJumpHeight);

    if (isValidCollisionOrWaveCheck())
        al::setVelocity(this, nexPos - al::getTrans(this));
    else
        al::setTrans(this, nexPos);
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
            mCurrentPosition.set(al::getTrans(this));
            mCurrentPosition.y -= mJumpHeight;
            mIsOnHitReactionEffect = true;
        } else {
            mIsOnHitReactionEffect = false;
        }
    }

    tryStartHitReactionDown();
    sead::Vector3f trans = al::getTrans(this);

    f32 step = al::getNerveStep(this);
    f32 rate = (step / mTurnFrameCount) * (step / mTurnFrameCount);
    if (!isValidCollisionOrWaveCheck())
        rate = sead::Mathf::clamp(rate, 0.0f, 1.0f);

    trans.y = mCurrentPosition.y + mJumpHeight * (1.0f - rate);

    if (isValidCollisionOrWaveCheck())
        al::setVelocity(this, trans - al::getTrans(this));
    else
        al::setTrans(this, trans);

    updatePosOnWave();

    updateShadowMaskLength();

    if (isValidCollisionOrWaveCheck()) {
        if (!isOnGround())
            return;
        if (!isOnDamageFire()) {
            al::startHitReaction(this, "地面での消滅");
            revive();
            return;
        }
        al::tryAddRippleMiddle(this);
    } else {
        if (!al::isGreaterEqualStep(this, mTurnFrameCount))
            return;
    }
    shiftSink();
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
        al::getTransPtr(this)->y = mCurrentPosition.y + nerveRate * -150.0f;
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
            al::getTransPtr(this)->y = mCurrentPosition.y;
        }
    }

    updateVelocityIfValidCollision();

    if (isValidCollisionOrWaveCheck() && !isOnDamageFire()) {
        forceEndHack();
        return;
    }

    if (al::isGreaterEqualStep(this, mWaitFrameNum) && !mIsInSaucePan) {
        if (!al::isNoCollide(this) && mWaitDelay < 1) {
            revive();
            return;
        }

        mWaitDelay--;

        if (!al::isValidSwitchStart(this) || al::isOnSwitchStart(this))
            shiftReadyOrMove();
        return;
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
            if (!al::isActionPlaying(this, "Down") && !al::isActionPlaying(this, "Turn"))
                al::startAction(this, "HackStart");
            else
                al::startAction(this, "HackStartDown");
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

    if (!isOnGround())
        return;

    if (isOnDamageFire()) {
        mCurrentPosition.set(al::getTrans(this));

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

void Bubble::exeHackFall() {
    if (al::isFirstStep(this))
        mCurrentRotation.set(al::getQuat(this));

    al::getVelocityPtr(this)->y += -1.1f;
    tryHitReactionThroughFence();
    tryShiftLand();
}

void Bubble::exeHackMove() {
    if (al::isFirstStep(this)) {
        mShiftFallDelay = 0;
        mCurrentRotation.set(al::getQuat(this));
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
        if (mIsPlayerCaptured || !rs::isHoldHackAction(mPlayerHack))
            al::tryStartActionIfNotPlaying(this, "HackWait");
        else
            al::tryStartActionIfNotPlaying(this, "HackWaitSpeedy");
    } else if (mIsPlayerCaptured || !rs::isHoldHackAction(mPlayerHack)) {
        al::tryStartActionIfNotPlaying(this, "Move");
    } else {
        al::tryStartActionIfNotPlaying(this, "MoveSpeedy");
        if (!mIsInBossSequence)
            rs::requestDownToDefaultCameraAngleBySpeed(this, 6.0f, 0);
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
    al::setQuat(this, mCurrentRotation);
}

// NON_MATCHING: Major issues https://decomp.me/scratch/QivBr
void Bubble::exeHackJump() {
    bool isJumpHigh = al::isNerve(this, &NrvBubble.HackJumpHigh);
    f32 cosntA = !isJumpHigh ? 1.1f : 1.2f;
    f32 cosntB = !isJumpHigh ? 0.5f : 2.5f;

    sead::Quatf rotateB;
    sead::Quatf rotateA;
    al::makeQuatRotateDegree(&rotateA, sead::Vector3f::ez, 180.0f);
    al::makeQuatRotateDegree(&rotateB, sead::Vector3f::ey, 180.0f);
    sead::Quatf quatC;
    sead::QuatCalcCommon<f32>::setMul(quatC, rotateA, rotateB);

    if (al::isFirstStep(this)) {
        mJumpDelay = 15;
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
        mJumpFrame = 0;
        mPreviousTrans.set(al::getTrans(this));
        mCurrentRotation.set(al::getQuat(this));

        sead::Vector3f upDir;
        al::calcUpDir(&upDir, this);
        if (upDir.y < 0.99f) {
            if (upDir.y < 0.0f) {
                sead::Quatf quattB;
                al::makeQuatRotationRate(&quattB, upDir, -sead::Vector3f::ey, 1.0f);

                sead::QuatCalcCommon<f32>::setMul(mCurrentRotation, quattB, mCurrentRotation);
                mCurrentRotation *= sead::Quatf(sead::Mathf::cos(-1.5707964f),
                                                sead::Mathf::sin(-1.5707964f), 1.0f, 0.0f);
            } else {
                sead::Quatf quattA;
                al::makeQuatRotationRate(&quattA, upDir, sead::Vector3f::ey, 1.0f);
                sead::QuatCalcCommon<f32>::setMul(mCurrentRotation, quattA, mCurrentRotation);
            }
        }
        sead::QuatCalcCommon<f32>::setMul(rotateA, quatC, al::getQuat(this));
        al::setQuat(this, rotateA);
    }
    revertTargetQuatInHackJump(&rotateA, &rotateB);
    al::getVelocityPtr(this)->y -= cosntA;
    sead::Vector3f frontDir;
    al::calcFrontDir(&frontDir, this);
    sead::Vector3f hackerMoveVec = {0.0f, 0.0f, 0.0f};
    calcHackerMoveVec(&hackerMoveVec, sead::Vector3f::ey);

    sead::Vector3f hackerMoveVec2 = {0.0f, 0.0f, 0.0f};
    bool bVar1 = false;
    if (al::tryNormalizeOrZero(&hackerMoveVec2, hackerMoveVec)) {
        if (frontDir.dot(hackerMoveVec2) < sead::Mathf::cos(1.9198622f)) {
            sead::Vector3f vel = al::getVelocity(this);
            vel.y = 0.0f;
            if (!(vel.dot(hackerMoveVec2) > 0.0f)) {
                if (!al::isNearZero(vel, 1.0f)) {
                    al::scaleVelocityHV(this, 0.85f, 1.0f);
                    bVar1 = true;
                    goto LAB_71000e5c48;
                }
            }
            al::addVelocity(this, -vel * 0.07999998f + hackerMoveVec2 * 0.57f);
            bVar1 = true;
        } else {
            al::turnToDirection(this, hackerMoveVec2, 5.0f);

            sead::Vector3f frontDir2;
            al::calcFrontDir(&frontDir2, this);

            sead::Vector3f verticalVec;
            al::verticalizeVec(&verticalVec, sead::Vector3f::ey, al::getVelocity(this));
            al::addVelocity(this, -verticalVec);
            f32 fVar14 = frontDir2.dot(verticalVec);
            f32 fVar16 = sead::Mathf::clamp(frontDir2.dot(hackerMoveVec2), 0.0f, 1.0f);

            f32 neil = cosntB > fVar14 ? fVar14 * 0.92f + cosntB * fVar16 : fVar14;

            al::addVelocity(this, frontDir2 * neil + (verticalVec - frontDir2 * fVar14) * 0.98f);
            bVar1 = false;
        }
    }
LAB_71000e5c48:
    mCurrentRotation.set(al::getQuat(this));
    makeDisplayQuatInHackJump(rotateA, rotateB, quatC, bVar1);

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
    if (!(al::getVelocity(this).y < 0.0f) && al::isCollidedCeiling(this)) {
        if (!(al::getCollidedCeilingPos(this).y - al::getTrans(this).y <= 0.0f)) {
            if (!al::sendMsgEnemyAttackFire(al::getCollidedCeilingSensor(this),
                                            al::getHitSensor(this, "Attack"), "LavaRed") &&
                isHoldHackJump()) {
                if (!al::sendMsgEnemyUpperPunch(al::getCollidedCeilingSensor(this),
                                                al::getHitSensor(this, "Attack")))

                    al::setVelocityZeroV(this);
                else
                    mJumpFrame = 20;
            } else {
                al::setVelocityZeroV(this);
            }
            if (mJumpFrame != 0) {
                mJumpFrame--;
                al::getVelocityPtr(this)->y += cosntA;
            }
        } else

            mJumpFrame = 0;
    } else {
        mJumpFrame = 0;
    }

    if (al::isCollidedWall(this)) {
        rs::sendMsgBubbleWallTouch(al::getCollidedWallSensor(this),
                                   al::getHitSensor(this, "Attack"));
    }

    tryHitReactionThroughFence();
    if (!tryShiftContinuousJump())
        tryShiftLand();
}

void Bubble::endHackJump() {
    al::setQuat(this, mCurrentRotation);
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
            moveDir.set({0.0f, 0.0f, 0.0f});
            sead::Vector3f* velPtr = al::getVelocityPtr(this);
            al::parallelizeVec(velPtr, mFireSurface, *velPtr);
        } else if (!rs::calcHackerMoveDir(&moveDir, mPlayerHack, sead::Vector3f::ey)) {
            sead::Vector3f* velPtr = al::getVelocityPtr(this);
            al::parallelizeVec(velPtr, mFireSurface, *velPtr);
        }

        al::tryAddRippleMiddle(this);
        mShiftFallDelay = 0;
        al::startBgmSituation(this, "HackBubble", false, true);
        mUpDir = mFireSurface;
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

// Custom sead mult
inline void setQuatMul(sead::Quatf* out, const sead::Quatf& u, const sead::Quatf& v) {
    f32 w = (u.w * v.w) - (u.x * v.x) - (u.y * v.y) - (u.z * v.z);
    f32 x = (u.w * v.x) + (u.x * v.w) + (u.y * v.z) - (u.z * v.y);
    f32 y = (u.w * v.y) - (u.x * v.z) + (u.y * v.w) + (u.z * v.x);
    f32 z = (u.w * v.z) + (u.x * v.y) - (u.y * v.x) + (u.z * v.w);
    out->x = x;
    out->y = y;
    out->z = z;
    out->w = w;  // <-- this should go before out.x
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

        sead::Quatf u;
        al::makeQuatRotationRate(&u, initialDirection, sead::Vector3f::ey, 1.0f);
        setQuatMul(&mCurrentRotation, u, al::getQuat(this));

        if (isOnDamageFire())
            al::startHitReaction(this, "バブルキャノン着地");
        else if (!isHack2)
            al::startHitReaction(this, "地面での消滅");
        return;
    }

    if (al::updateNerveState(this)) {
        if (mPlayerHack) {
            mJumpForce.set({0.0f, 0.0f, 0.0f});
            al::startHitReaction(this, "バブルキャノン着地");
            al::setNerve(this, &NrvBubble.HackJump);
        } else {
            al::setNerve(this, &NrvBubble.WaitHackFall);
        }
    }
}

void Bubble::endHackInLauncher() {
    if (mCameraTicket) {
        al::endCamera(this, mCameraTicket, -1, false);
        mCameraTicket = nullptr;
    }

    if (mIsShowTutorial) {
        rs::showTutorial(this);
        mIsShowTutorial = false;
    }
}

// NON_MATCHING: Unreachable code not https://decomp.me/scratch/R6cb7
void Bubble::exeHackResetPos() {
    sead::Quatf quatA;
    sead::Quatf quatB;
    al::makeQuatRotateDegree(&quatA, sead::Vector3f::ez, 180.0f);
    al::makeQuatRotateDegree(&quatB, sead::Vector3f::ey, 180.0f);
    sead::Quatf quatC;
    sead::QuatCalcCommon<f32>::setMul(quatC, quatA, quatB);

    if (al::isFirstStep(this)) {
        sead::Vector3f trans = mResetTargetPos - al::getTrans(this);
        trans.y = 0.0f;

        const f32 len = trans.length();
        if (len > 0) {
            const f32 inv_len = (len / 180.0f) / len;
            trans.x *= inv_len;
            trans.y *= inv_len;
            trans.z *= inv_len;
        }
        trans.y = mResetTargetPos.y / 180.0f + 99.0f;

        al::setVelocity(this, trans);
        al::startAction(this, "HackJump");
        sead::Vector3f direction = al::getTrans(this) - mResetTargetPos;
        al::tryNormalizeOrZero(&direction);
        al::faceToDirection(this, direction);
        mPreviousTrans.set(al::getTrans(this));
        mCurrentRotation.set(al::getQuat(this));
        sead::QuatCalcCommon<f32>::setMul(quatA, al::getQuat(this), quatC);
        al::setQuat(this, quatA);
    }

    al::addVelocityToGravity(this, 1.1f);
    revertTargetQuatInHackJump(&quatA, &quatB);
    makeDisplayQuatInHackJump(quatA, quatB, quatC, true);

    if (al::isGreaterEqualStep(this, 180)) {
        sead::Vector3f surfaceA;
        sead::Vector3f surfaceB;
        if (!al::calcFindFireSurface(&surfaceA, &surfaceB, this, al::getTrans(this),
                                     sead::Vector3f::ey, 800.0f)) {
            mJumpForce.set({0.0f, 0.0f, 0.0f});
            al::setVelocityZero(this);
            al::setNerve(this, &NrvBubble.HackJump);
        } else {
            sead::Vector3f trans = al::getTrans(this);
            trans.y = surfaceA.y;
            al::resetPosition(this, trans);
            mJumpForce.set({0.0f, 0.0f, 0.0f});
            al::setVelocityZero(this);
            mIsInFire = true;
            al::tryUpdateEffectMaterialCode(this, al::getFireMaterialCode(this));
            mFireSurface.set(surfaceB);
            al::setNerve(this, &NrvBubble.HackLand);
        }
    }
}

void Bubble::exeHackDemo() {}

void Bubble::exeRevive() {
    if (al::isFirstStep(this)) {
        rs::startReset(this);
        al::stopAction(this);
        al::resetPosition(this, mResetPosition);
        al::setQuat(this, mStartingRotation);
        mCurrentRotation.set(mStartingRotation);
        mCurrentPosition = mRevivePosition;
        mAnimScaleController->resetScale();
        mIsInSaucePan = false;
    }

    if (mWaitDelayCount == mWaitDelayTime - 1) {
        rs::endReset(this);
        al::setNerve(this, &NrvBubble.Ready);
        return;
    }
}
