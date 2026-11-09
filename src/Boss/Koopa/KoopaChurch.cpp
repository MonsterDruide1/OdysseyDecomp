#include "Boss/Koopa/KoopaChurch.h"

#include "Boss/BossUtil/BossUtil.h"
#include "Boss/Koopa/KoopaCameraCtrl.h"
#include "Boss/Koopa/KoopaDemoExecutor.h"
#include "Boss/Koopa/KoopaFunction.h"
#include "Library/Demo/DemoFunction.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "MapObj/ChangeStageInfo.h"
#include "MapObj/ChurchDoor.h"
#include "Npc/Peach.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"
#include "Util/NpcAnimUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(KoopaChurch, BeforeEnter);
NERVE_IMPL(KoopaChurch, Wait);
NERVE_IMPL(KoopaChurch, DemoEnter);
NERVE_IMPL(KoopaChurch, DemoChurch);
NERVE_IMPL(KoopaChurch, End);
NERVE_IMPL(KoopaChurch, ChangeStage);

NERVES_MAKE_STRUCT(KoopaChurch, BeforeEnter, Wait, DemoEnter, DemoChurch, End, ChangeStage);

void syncPeachAndRingToChurch(KoopaChurch* church, Peach* peach, al::LiveActor* ring) {
    if (al::isDead(ring))
        ring->appear();
    al::copyPose(peach, church);
    al::copyPose(ring, church);
    al::resetPosition(peach);
    al::resetPosition(ring);
    peach->startWaitChurch();
    al::startAction(church, "WaitChurch");
    al::startAction(ring, "WaitChurch");
}
}  // namespace

KoopaChurch::KoopaChurch(const char* name) : al::LiveActor(name) {}

void KoopaChurch::init(const al::ActorInitInfo& info) {
    KoopaFunction::initActorKoopaDemoModel(this, info);
    al::initNerve(this, &NrvKoopaChurch.BeforeEnter, 0);
    al::invalidateClipping(this);
    mChangeStageInfo = rs::createChangeStageInfo(this, al::getPlacementInfo(info));
    mChurchDoor = new ChurchDoor("教会ドア");
    al::initLinksActor(mChurchDoor, info, "DemoChurchDoor", 0);
    al::invalidateClipping(mChurchDoor);
    mPeach = KoopaFunction::initAndCreateLinksPeach(info);
    al::invalidateClipping(mPeach);
    mDemoPeachRing = KoopaFunction::initAndCreateDemoPeachRing(info);
    al::invalidateClipping(mDemoPeachRing);
    KoopaDemoExecutor* executor = new KoopaDemoExecutor();
    mDemoExecutor = executor;
    al::EventFlowExecutor* demoEvent = KoopaFunction::initAndCreateDemoEventChurch(this, info);
    KoopaCameraCtrl* cameraCtrl = new KoopaCameraCtrl(this, info);
    executor->init(this, info, demoEvent, cameraCtrl, mPeach);
    mDemoExecutor->initMoonChurch(info, mChurchDoor, mDemoPeachRing);
    mAddDemoInfo = al::registDemoRequesterToAddDemoInfo(this, info, 0);
    syncPeachAndRingToChurch(this, mPeach, mDemoPeachRing);
    if (mChurchDoor->isOpenWait()) {
        mDemoPeachRing->makeActorAlive();
        al::setNerve(this, &NrvKoopaChurch.Wait);
    }
    makeActorAlive();
}

void KoopaChurch::control() {
    rs::trySwitchDepthToSelfShadow(this);
}

bool KoopaChurch::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                             al::HitSensor* self) {
    return rs::isMsgCapReflect(message);
}

bool KoopaChurch::isFirstDemo() const {
    return !rs::isAlreadyShowDemoBossBattleStart(this, 6, 2);
}

bool KoopaChurch::isEnableSkipDemo() const {
    return true;
}

void KoopaChurch::skipDemo() {
    mDemoExecutor->skip();
    if (al::isNerve(this, &NrvKoopaChurch.DemoEnter)) {
        syncPeachAndRingToChurch(this, mPeach, mDemoPeachRing);
        al::setNerve(this, &NrvKoopaChurch.Wait);
    } else if (al::isNerve(this, &NrvKoopaChurch.DemoChurch)) {
        if (GameDataFunction::tryChangeNextStage(this, mChangeStageInfo))
            al::setNerve(this, &NrvKoopaChurch.End);
        else
            al::setNerve(this, &NrvKoopaChurch.ChangeStage);
    }
}

void KoopaChurch::exeBeforeEnter() {
    if (mChurchDoor->isOpenWait()) {
        if (mDemoExecutor->tryStartChurchEnterDemo(this, mAddDemoInfo))
            al::setNerve(this, &NrvKoopaChurch.DemoEnter);
    }
}

void KoopaChurch::exeDemoEnter() {
    if (mDemoExecutor->update()) {
        syncPeachAndRingToChurch(this, mPeach, mDemoPeachRing);
        al::setNerve(this, &NrvKoopaChurch.Wait);
    }
}

void KoopaChurch::exeWait() {
    if (al::isOnStageSwitch(this, "SwitchDemoChurchStart")) {
        if (mDemoExecutor->tryStartChurchStartDemo(this, mAddDemoInfo))
            al::setNerve(this, &NrvKoopaChurch.DemoChurch);
    }
}

void KoopaChurch::exeDemoChurch() {
    if (mDemoExecutor->update()) {
        if (GameDataFunction::tryChangeNextStage(this, mChangeStageInfo))
            al::setNerve(this, &NrvKoopaChurch.End);
        else
            al::setNerve(this, &NrvKoopaChurch.ChangeStage);
    }
}

void KoopaChurch::exeChangeStage() {
    if (GameDataFunction::tryChangeNextStage(this, mChangeStageInfo))
        al::setNerve(this, &NrvKoopaChurch.End);
}

void KoopaChurch::exeEnd() {}
