#include "MapObj/TreasureBoxKey.h"

#include <math/seadVector.h>

#include "Library/Camera/CameraUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSceneFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "Item/Shine.h"
#include "MapObj/TreasureBoxKeyOpener.h"
#include "Util/DemoUtil.h"
#include "Util/ItemUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(TreasureBoxKey, CloseWait);
NERVE_IMPL(TreasureBoxKey, WaitStartDemo);
NERVE_IMPL(TreasureBoxKey, WaitCameraInterpole);
NERVE_IMPL(TreasureBoxKey, Open);
NERVE_IMPL(TreasureBoxKey, WaitEndCameraInterpole);
NERVE_IMPL(TreasureBoxKey, OpenWait);

NERVES_MAKE_NOSTRUCT(TreasureBoxKey, WaitCameraInterpole);
NERVES_MAKE_STRUCT(TreasureBoxKey, CloseWait, OpenWait, Open, WaitStartDemo,
                   WaitEndCameraInterpole);
}  // namespace

TreasureBoxKey::TreasureBoxKey(const char* name) : al::LiveActor(name) {}

void TreasureBoxKey::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    al::initNerve(this, &NrvTreasureBoxKey.CloseWait, 0);
    if (al::isExistLinkChild(info, "BoxOpener", 0)) {
        mTreasureBoxKeyOpener = new TreasureBoxKeyOpener("鍵付宝箱の鍵");
        al::initLinksActor(mTreasureBoxKeyOpener, info, "BoxOpener", 0);
        mTreasureBoxKeyOpener->setHostForClipping(this);
    }

    mShine = rs::initLinkShine(info, "ShineActor", 0);
    if (mShine ) {
        al::updatePoseRotate(mShine, sead::Vector3f::zero);
        sead::Vector3f trans = al::getTrans(this);
        trans.y = al::getTrans(mShine).y;
        al::setTrans(mShine, trans);

        if (mShine )
            rs::updateHintTrans(mShine, al::getTrans(mTreasureBoxKeyOpener));
    }

    bool isValidObjectCamera = false;
    al::tryGetArg(&isValidObjectCamera, info, "IsValidObjectCamera");
    if (isValidObjectCamera)
        mCameraTicket = al::initDemoObjectCamera(this, info, nullptr, "固定");

    al::tryGetArg(&mIsStopUpdateWhileCameraMoving, info, "IsStopUpdateWhileCameraMoving");
    al::tryGetArg(&mCameraStartInterpFrame, info, "CameraStartInterpFrame");
    al::tryOffStageSwitch(this, "GetOpenerOn");
    makeActorAlive();
}

void TreasureBoxKey::makeActorDead() {
    al::LiveActor::makeActorDead();
    if (mTreasureBoxKeyOpener )
        mTreasureBoxKeyOpener->makeActorDead();

    if (mShine )
        mShine->makeActorDead();

    if (mCameraTicket  && al::isActiveCamera(mCameraTicket))
        al::endCamera(this, mCameraTicket, -1, false);
}

void TreasureBoxKey::makeActorAlive() {
    al::LiveActor::makeActorAlive();
    al::validateClipping(this);
    if (mShine )
        al::validateClipping(mShine);
}

bool TreasureBoxKey::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                al::HitSensor* self) {
    if (al::isSensorName(self, "PlayerRegard")) {
        if (al::isMsgPlayerDisregard(message) && al::isNerve(this, &NrvTreasureBoxKey.OpenWait))
            return true;
        return rs::isMsgPlayerDisregardHomingAttack(message);
    }

    if (al::isMsgRestart(message)) {
        restart();
        return true;
    }

    if (al::isMsgChangeAlpha(message)) {
        f32 alpha = al::getChangeAlphaValue(message);
        al::setModelAlphaMask(this, alpha);
        al::setModelAlphaMask(mTreasureBoxKeyOpener, alpha);
        if (!al::sendMsgChangeAlpha(mShine, alpha))
            al::setModelAlphaMask(mShine, alpha);
        return true;
    }

    if (rs::isMsgTimerAthleticDemoStart(message)) {
        al::addDemoActor(this);
        al::addDemoActor(mTreasureBoxKeyOpener);
        return true;
    }

    return false;
}

void TreasureBoxKey::restart() {
    al::startVisAnim(this, "CloseWait");
    al::startAction(this, "Wait");
    appear();
    al::sendMsgRestart(mTreasureBoxKeyOpener);
    al::sendMsgRestart(mShine);
    al::stopDitherAnimAutoCtrl(mTreasureBoxKeyOpener);
    mShine->makeActorDead();
    rs::updateHintTrans(mShine, al::getTrans(mTreasureBoxKeyOpener));
    al::setNerve(this, &NrvTreasureBoxKey.CloseWait);
}

void TreasureBoxKey::exeCloseWait() {
    if (al::isFirstStep(this)) {
        al::tryStartVisAnimIfNotPlaying(this, "CloseWait");
        al::tryStartActionIfNotPlaying(this, "Wait");
    }

    if (mTreasureBoxKeyOpener->isGot()) {
        al::invalidateClipping(this);
        if (mCameraTicket)
            al::setNerve(this, &NrvTreasureBoxKey.WaitStartDemo);
        else
            al::setNerve(this, &NrvTreasureBoxKey.Open);
    }
}

void TreasureBoxKey::exeWaitStartDemo() {
    if (rs::requestStartDemoNormal(this, false)) {
        al::startCamera(this, mCameraTicket, mCameraStartInterpFrame);
        al::setNerve(this, &WaitCameraInterpole);
    }
}

void TreasureBoxKey::exeWaitCameraInterpole() {
    if (al::isGreaterEqualStep(this, 5) && !al::isActiveCameraInterpole(this, 0))
        al::setNerve(this, &NrvTreasureBoxKey.Open);
}

void TreasureBoxKey::exeOpen() {
    if (al::isFirstStep(this)) {
        al::invalidateHitSensors(this);
        al::invalidateCollisionParts(this);
        al::startAction(this, "Open");
        al::tryOnStageSwitch(this, "GetOpenerOn");
        rs::updateHintTrans(mShine, al::getTrans(mShine));
    }

    if (al::isStep(this, 3)) {
        al::invalidateClipping(mShine);
        mShine->appearPopupWithoutDemo();
        al::sendMsgShowModel(mShine);
        if (mCameraTicket )
            al::startHitReaction(this, "シャイン出現デモ演出");
    }

    if (al::isStep(this, al::getActionFrameMax(this, "Open"))) {
        al::startHitReaction(this, "DisappearOpenedDoor");
        al::startVisAnim(this, "OpenWait");
        al::startAction(this, "Wait");
    }

    if (mCameraTicket ) {
        if (!al::isGreaterEqualStep(this, 150))
            return;

        al::endCamera(this, mCameraTicket, 30, false);
        if (mIsStopUpdateWhileCameraMoving) {
            al::setNerve(this, &NrvTreasureBoxKey.WaitEndCameraInterpole);
            return;
        }

        rs::requestEndDemoNormal(this);
        al::setNerve(this, &NrvTreasureBoxKey.OpenWait);
        return;
    }

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvTreasureBoxKey.OpenWait);
}

void TreasureBoxKey::exeWaitEndCameraInterpole() {
    if (al::isGreaterEqualStep(this, 5) && !al::isActiveCameraInterpole(this, 0))
        al::setNerve(this, &NrvTreasureBoxKey.OpenWait);
}

void TreasureBoxKey::exeOpenWait() {
    if (al::isFirstStep(this)) {
        al::tryStartVisAnimIfNotPlaying(this, "OpenWait");
        al::tryStartActionIfNotPlaying(this, "Wait");
        if (!mIsStopUpdateWhileCameraMoving)
            al::validateClipping(this);
    }

    if (mIsStopUpdateWhileCameraMoving && al::isStep(this, 30)) {
        rs::requestEndDemoNormal(this);
        al::validateClipping(this);
    }
}
