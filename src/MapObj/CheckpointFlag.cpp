#include "MapObj/CheckpointFlag.h"

#include <basis/seadNew.h>

#include "Library/Layout/LayoutInitInfo.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Message/MessageHolder.h"
#include "Library/Nature/NatureUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"

#include "MapObj/AirBubble.h"
#include "MapObj/CheckpointFlagWatcher.h"
#include "MapObj/PlayerStartInfoHolder.h"
#include "Npc/BirdMtxGlideCtrl.h"
#include "System/GameDataFunction.h"
#include "Util/ItemUtil.h"
#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"
#include "Util/StageLayoutFunction.h"

namespace {
NERVE_IMPL(CheckpointFlag, Before);
NERVE_IMPL(CheckpointFlag, After);
NERVE_IMPL(CheckpointFlag, Get);
NERVE_IMPL(CheckpointFlag, Shake);

NERVES_MAKE_STRUCT(CheckpointFlag, Before, After, Get, Shake);
}  // namespace

CheckpointFlag::CheckpointFlag(const char* name) : al::LiveActor(name) {}

const sead::Vector3f gBirdVector = {75.0f, 0.0f, 0.0f};
const sead::Vector3f gTransIn = {0.0f, 100.0f, 0.0f};
const sead::Vector3f gBalloonTrans = {0.0f, 75.0f, 0.0f};

void CheckpointFlag::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "CheckpointFlag", nullptr);
    al::initNerve(this, &NrvCheckpointFlag.Before, 0);
    rs::registerCheckpointFlagToWatcher(this);
    mMessageSystem = al::getLayoutInitInfo(info).getMessageSystem();
    mInfo = al::createLinksPlayerActorInfo(this, info);
    mPlaceId = al::createPlacementId(info);
    al::tryGetArg(&mIsZeroGravity, info, "IsZeroGravity");
    if (al::isInWater(this)) {
        if (al::isInWaterPos(this, al::getSensorPos(this, "BodyTop"))) {
            mAirBubble = new AirBubble("中間ポイントの空気泡", false);
            al::initCreateActorNoPlacementInfo(mAirBubble, info);
        }
    }
    if (mIsHome) {
        rs::registerLinkedPlayerStartInfoToHolder(this, info, nullptr, nullptr, nullptr);
        mFlagName = al::getSystemMessageString(this, "GlossaryObject", "Home");
        setAfter();
        makeActorAlive();
        return;
    }
    f32 frame = rs::getStageShineAnimFrame(this);
    if (frame < 0.0f)
        frame = 0.0f;
    al::startMclAnimAndSetFrameAndStop(this, "Color", frame);
    al::StringTmp<128> messageLabel("");
    rs::makeActorMessageLabel(&messageLabel, info, rs::getCheckpointLabelPrefix());
    const char* stageName = rs::getPlacementStageName(this, info);
    if (al::isExistLabelInStageMessage(this, stageName, messageLabel.cstr()))
        mFlagName = al::getStageMessageString(this, stageName, messageLabel.cstr());
    if (GameDataFunction::isGotCheckpoint(this, mPlaceId))
        setAfter();
    rs::registerLinkedPlayerStartInfoToHolder(this, info, nullptr, nullptr, nullptr);
    GameDataFunction::registerCheckpointTrans(this, mPlaceId, al::getTrans(this));
    mBirdMtxGlideCtrl = BirdMtxGlideCtrl::tryCreateAliveWaitByLinksBird(
        al::getJointMtxPtr(this, "Pole2"), gBirdVector, info, "Bird");
    al::trySyncStageSwitchAppear(this);
}

void CheckpointFlag::initAfterPlacement() {}

void CheckpointFlag::control() {
    mHitCooldown--;
    if (mAirBubbleTimer > -1)
        mAirBubbleTimer--;
    if (mBirdMtxGlideCtrl)
        mBirdMtxGlideCtrl->update();
}

void CheckpointFlag::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isNerve(this, &NrvCheckpointFlag.After) && al::isSensorName(self, "Push"))
        rs::sendMsgPushToPlayer(other, self) || al::sendMsgPush(other, self);
}

bool CheckpointFlag::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                al::HitSensor* self) {
    if (al::isNerve(this, &NrvCheckpointFlag.Get))
        return false;
    if (al::isMsgPlayerDisregard(message))
        return !al::isSensorName(self, "BodyTop");
    if (rs::isMsgPlayerDisregardTargetMarker(message))
        return true;
    if (mIsGot) {
        if (rs::isMsgItemGet(message) || al::isMsgPlayerKick(message) ||
            al::isMsgPlayerObjTouch(message) || rs::isMsgYoshiTongueAttack(message)) {
            bool isGot = mIsGot;
            AirBubble* airBubble = mAirBubble;
            s32 hitCooldown = mHitCooldown;
            mHitCooldown = 10;
            if (hitCooldown > 0)
                return false;
            if (isGot)
                rs::setTouchCheckpointFlagToWatcher(this);
            if (airBubble && !al::isAlive(airBubble) && mAirBubbleTimer < 0) {
                al::calcTransLocalOffset(al::getTransPtr(airBubble), this, gTransIn);
                airBubble->appear();
                mAirBubbleTimer = 180;
            }
            al::setNerve(this, &NrvCheckpointFlag.Shake);
            return al::isMsgPlayerObjTouch(message);
        }
    } else {
        bool isAcquired = false;
        if (rs::isMsgItemGet(message)) {
            if (rs::isPlayerHack(this) || rs::isPlayerBinding(this))
                isAcquired = true;
        } else if (rs::isMsgCapReflect(message) || al::isMsgPlayerKick(message) ||
                   al::isMsgPlayerItemGet(message) || rs::isMsgYoshiTongueAttack(message))
            isAcquired = true;
        if (isAcquired) {
            al::startHitReaction(this, "取得");
            rs::setTouchCheckpointFlagToWatcher(this);
            AirBubble* airBubble = mAirBubble;
            if (airBubble && !al::isAlive(airBubble) && mAirBubbleTimer < 0) {
                al::calcTransLocalOffset(al::getTransPtr(airBubble), this, gTransIn);
                airBubble->appear();
                mAirBubbleTimer = 180;
            }
            GameDataFunction::recoveryPlayerMax(this);
            rs::sendMsgAckCheckpoint(other, self);
            al::setNerve(this, &NrvCheckpointFlag.Get);
            return al::isMsgPlayerKick(message);
        }
    }
    if (rs::isMsgCapReflect(message) || rs::isMsgHosuiAttack(message) ||
        rs::isMsgHammerBrosHammerHackAttack(message) || al::isMsgPlayerFireBallAttack(message) ||
        rs::isMsgTankBullet(message)) {
        bool isGot = mIsGot;
        AirBubble* airBubble = mAirBubble;
        s32 hitCooldown = mHitCooldown;
        mHitCooldown = 10;
        if (hitCooldown > 0)
            return false;
        if (isGot)
            rs::setTouchCheckpointFlagToWatcher(this);
        if (airBubble && !al::isAlive(airBubble) && mAirBubbleTimer < 0) {
            sead::Vector3f* trans = al::getTransPtr(airBubble);
            al::calcTransLocalOffset(trans, this, gTransIn);
            airBubble->appear();
            mAirBubbleTimer = 180;
        }
        al::setNerve(this, &NrvCheckpointFlag.Shake);
        return al::isMsgPlayerObjTouch(message);
    }
    return false;
}

const al::PlacementInfo* CheckpointFlag::getPlayerRestartInfo() const {
    return &al::getPlacementInfo(*mInfo);
}

void CheckpointFlag::setAfter() {
    bool isZeroGravity = mIsZeroGravity;
    al::startAction(this, "After");
    if (isZeroGravity)
        al::setActionFrameRate(this, 0.2f);
    al::setNerve(this, &NrvCheckpointFlag.After);
}

void CheckpointFlag::startArriveByWarp() {
    if (mBirdMtxGlideCtrl)
        mBirdMtxGlideCtrl->makeActorDeadBirdIfAlive();
}

void CheckpointFlag::initHomeFlag(const al::ActorInitInfo& info) {
    mIsHome = true;
    init(info);
}

void CheckpointFlag::exeBefore() {
    if (al::isFirstStep(this)) {
        bool isZeroGravity = mIsZeroGravity;
        al::startAction(this, "Before");
        if (isZeroGravity)
            al::setActionFrameRate(this, 0.2f);
        if (mBirdMtxGlideCtrl)
            mBirdMtxGlideCtrl->validateGlide();
    }
    if (!mIsHome) {
        if (mBirdMtxGlideCtrl && mBirdMtxGlideCtrl->isWaitBird())
            rs::requestShowCheckpointFlagBalloon(this, gBalloonTrans);
        else
            rs::requestShowCheckpointFlagBalloon(this, sead::Vector3f::zero);
    }
}

void CheckpointFlag::exeGet() {
    if (al::isFirstStep(this)) {
        bool isZeroGravity = mIsZeroGravity;
        al::startAction(this, "Get");
        if (isZeroGravity)
            al::setActionFrameRate(this, 0.2f);
        rs::requestShowCheckpointFlagNameLayout(this, mFlagName);
        rs::requestHideCheckpointFlagBalloon(this);
        if (mBirdMtxGlideCtrl)
            mBirdMtxGlideCtrl->invalidateGlide();
        mIsGot = true;
    }
    al::setNerveAtActionEnd(this, &NrvCheckpointFlag.After);
}

void CheckpointFlag::exeAfter() {
    if (al::isFirstStep(this)) {
        bool isZeroGravity = mIsZeroGravity;
        mIsGot = true;
        if (!al::isActionPlaying(this, "After")) {
            al::startAction(this, "After");
            if (isZeroGravity)
                al::setActionFrameRate(this, 0.2f);
        }
        if (mBirdMtxGlideCtrl)
            mBirdMtxGlideCtrl->validateGlide();
    }
}

void CheckpointFlag::exeShake() {
    if (al::isFirstStep(this)) {
        bool isZeroGravity = mIsZeroGravity;
        al::startAction(this, "Shake");
        if (isZeroGravity)
            al::setActionFrameRate(this, 0.2f);
        if (mBirdMtxGlideCtrl)
            mBirdMtxGlideCtrl->invalidateGlide();
        if (mIsGot)
            rs::requestShowCheckpointFlagNameLayout(this, mFlagName);
    }
    if (al::isActionEnd(this)) {
        if (mIsGot)
            al::setNerve(this, &NrvCheckpointFlag.After);
        else
            al::setNerve(this, &NrvCheckpointFlag.Before);
    }
}
