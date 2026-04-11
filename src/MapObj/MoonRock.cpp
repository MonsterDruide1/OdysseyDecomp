#include "MapObj/MoonRock.h"

#include <prim/seadSafeString.h>

#include "Library/Camera/CameraUtil.h"
#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Demo/DemoFunction.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nature/NatureUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "MapObj/CapMessageShowInfo.h"
#include "MapObj/ChangeStageInfo.h"
#include "MapObj/GoalMark.h"
#include "Scene/WipeHolderRequester.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"
#include "System/GameDataHolderWriter.h"
#include "Util/DemoUtil.h"
#include "Util/MoonRockUtil.h"
#include "Util/PlayerDemoUtil.h"
#include "Util/SensorMsgFunction.h"
#include "Util/SpecialBuildUtil.h"

namespace {
NERVE_IMPL_(MoonRock, Sleep, ChangeScene)
NERVE_IMPL(MoonRock, Break)
NERVE_IMPL(MoonRock, Wait)
NERVE_IMPL(MoonRock, Reaction)
NERVE_IMPL(MoonRock, ReadyRequestStartDemoMoonSetStart)
NERVE_IMPL(MoonRock, RequestStartDemoMoonSetStart)
NERVE_IMPL(MoonRock, DemoMoonSetStart)
NERVE_IMPL(MoonRock, EndDemoMoonSetStart)
NERVE_IMPL(MoonRock, ChangeScene)

NERVES_MAKE_NOSTRUCT(MoonRock, Sleep, Break, Wait, Reaction, ReadyRequestStartDemoMoonSetStart,
                     RequestStartDemoMoonSetStart, DemoMoonSetStart, EndDemoMoonSetStart,
                     ChangeScene)

union WreckageArg {
    s32 scenarioNo;
    sead::SafeString archiveName;

    WreckageArg() : scenarioNo(-1) {}

    ~WreckageArg() {}
};
}  // namespace

void MoonRock::init(const al::ActorInitInfo& info) {
    al::initActorSceneInfo(this, info);
    if (GameDataFunction::isEnableOpenMoonRock(this))
        al::initActorSuffix(this, info, "Standby");
    else
        al::initActor(this, info);

    s32 scenarioNo = -1;
    al::tryGetArg(&scenarioNo, info, "ScenarioNo");

    GameDataHolderAccessor accessor(this);
    mChangeStageInfo =
        rs::createChangeStageInfo(this, "MoonRock", GameDataFunction::getCurrentStageName(accessor),
                                  false, scenarioNo, ChangeStageInfo::NO_SUB_SCENARIO);
    mChangeStageInfo->setWipeType("FadeWhite");
    mPlacementId = al::createPlacementId(info);

    if (al::isExistLinkChild(info, "DemoCameraPosition", 0)) {
        mHasDemoCameraPosition = true;
        al::getLinksMatrix(&mDemoCameraPosition, info, "DemoCameraPosition");
    }

    const al::Nerve* initialNerve = &Sleep;
    if (GameDataFunction::isEnableOpenMoonRock(this)) {
        mAddDemoInfo = al::registDemoRequesterToAddDemoInfo(this, info, 0);
        mDemoCamera = al::initDemoAnimCamera(this, info, "Anim");

        mGoalMark = new GoalMark("ゴールマーク");
        al::initCreateActorWithPlacementInfo(mGoalMark, info);
        al::copyPose(mGoalMark, this);
        al::setTrans(mGoalMark, al::getTrans(this));

        initialNerve = &Wait;
    } else {
        WreckageArg wreckageArg;
        al::tryGetArg(&wreckageArg.scenarioNo, info, "ScenarioNo");
        if (GameDataFunction::getScenarioNo(this) == wreckageArg.scenarioNo) {
            al::tryGetArg(&mWreckagePoseType, info, "WreckagePoseType");

            mWreckageActor = new al::LiveActor("残骸");
            new (&wreckageArg.archiveName) sead::SafeString("MoonRockPiece");
            al::initChildActorWithArchiveNameNoPlacementInfo(mWreckageActor, info,
                                                             wreckageArg.archiveName, nullptr);
            mWreckageActor->makeActorAlive();
            GameDataFunction::openMoonRock(this, mPlacementId);
            mIsOpenedOnInit = true;
            initialNerve = &ReadyRequestStartDemoMoonSetStart;
        }
    }

    rs::createCapMessageEnableChecker(&mCapMessageEnableChecker, this, info);
    al::initNerve(this, initialNerve, 0);

    if (initialNerve == &ReadyRequestStartDemoMoonSetStart)
        makeActorDead();
    else
        makeActorAlive();

    GameDataFunction::setMoonRockTrans(GameDataHolderWriter(this), al::getTrans(this));
}

void MoonRock::initAfterPlacement() {
    if (mWreckageActor == nullptr)
        return;

    if (mWreckagePoseType == 1) {
        al::updatePoseMtx(mWreckageActor, &mDemoCameraPosition);
        return;
    }

    al::copyPose(mWreckageActor, this);

    sead::Vector3f hitPos = sead::Vector3f::zero;
    sead::Vector3f hitNormal = sead::Vector3f::ey;
    const al::IUseCollision* useCollision = static_cast<const al::IUseCollision*>(this);
    const sead::Vector3f& trans = al::getTrans(this);
    sead::Vector3f start = trans + (sead::Vector3f::ey * 100.0);
    sead::Vector3f arrow = sead::Vector3f::ey * -500.0;

    if (alCollisionUtil::getHitPosAndNormalOnArrow(useCollision, &hitPos, &hitNormal, start, arrow,
                                                   nullptr, nullptr)) {
        al::setTrans(mWreckageActor, hitPos);
        al::makeQuatUpNoSupport(al::getQuatPtr(mWreckageActor), hitNormal);
    }
}

void MoonRock::makeActorAlive() {
    al::LiveActor::makeActorAlive();
    if (rs::isModeE3MovieRom() || rs::isModeE3LiveRom()) {
        makeActorDead();
        return;
    }

    al::invalidateHitSensor(this, "Kill");
}

void MoonRock::kill() {
    if (mGoalMark && al::isAlive(mGoalMark))
        mGoalMark->kill();

    al::LiveActor::kill();
}

bool MoonRock::isFirstDemo() const {
    return rs::isFirstDemoOpenMoonRock(this);
}

bool MoonRock::isEnableSkipDemo() const {
    return true;
}

void MoonRock::skipDemo() {
    rs::requestEndDemoWithPlayerCinemaFrame(this);

    GameDataHolderWriter writer(this);
    const ChangeStageInfo* changeStageInfo = mChangeStageInfo;
    GameDataFunction::tryChangeNextStage(writer, changeStageInfo);
}

void MoonRock::movement() {
    if (!al::isNerve(this, &Sleep)) {
        al::LiveActor::movement();
        return;
    }

    if (!rs::tryCheckShowCapMsgMoonRockLook(this, mCapMessageEnableChecker))
        rs::tryCheckShowCapMsgMoonRockLookLongTime(this, mCapMessageEnableChecker);
}

void MoonRock::calcAnim() {
    if (al::isNerve(this, &Sleep)) {
        al::calcViewModel(this);
        return;
    }

    al::LiveActor::calcAnim();
}

GoalMark* MoonRock::getGoalMarkForCapTalk() const {
    return mGoalMark;
}

void MoonRock::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isNerve(this, &Break))
        rs::sendMsgKillByMoonRockDemo(other, self);
}

bool MoonRock::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    if (al::isMsgAskSafetyPoint(message) ||
        (!al::isNerve(this, &Sleep) && !al::isNerve(this, &Wait) &&
         al::isMsgPlayerDisregard(message)) ||
        rs::isMsgPlayerDisregardHomingAttack(message) ||
        rs::isMsgPlayerDisregardTargetMarker(message)) {
        return true;
    }

    if (al::isNerve(this, &Wait) &&
        (rs::isMsgCapTouchWall(message) || rs::isMsgPlayerAndCapHipDropAll(message) ||
         rs::isMsgCapReflectCollide(message))) {
        al::invalidateClipping(this);
        al::setNerve(this, &Reaction);
        return true;
    }

    return false;
}

void MoonRock::exeWait() {
    if (!al::isFirstStep(this))
        return;

    al::validateHitSensor(this, "Kill");
    mGoalMark->activate();
    al::startAction(this, "Standby");
}

void MoonRock::exeReaction() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "ReactionCap");
        al::startHitReaction(this, "Hit");
    }

    if (!al::isActionEnd(this))
        return;

    al::killForceBeforeDemo(mGoalMark);
    al::tryAddRipple(this, al::getTrans(this), 1.0f, 2000.0f);
    al::setNerve(this, &Break);
}

void MoonRock::exeBreak() {
    kill();
}

void MoonRock::exeReadyRequestStartDemoMoonSetStart() {
    al::setNerve(this, &RequestStartDemoMoonSetStart);
}

void MoonRock::exeRequestStartDemoMoonSetStart() {
    if (!rs::requestStartDemoWithPlayerCinemaFrame(this, false))
        return;

    rs::disableOpenWipeForSkipDemo(this);
    al::addDemoActorFromAddDemoInfo(this, mAddDemoInfo);
    if (mHasDemoCameraPosition)
        al::resetMtxPosition(this, mDemoCameraPosition);

    al::tryOnStageSwitch(this, "DemoStartOn");
    rs::forcePutOnDemoCap(this);
    rs::hideDemoPlayer(this);
    al::invalidateClipping(this);
    al::startAction(this, "DemoStartMoonSet");
    al::startAnimCamera(this, mDemoCamera, "DemoStartMoonSet", 0);
    rs::requestValidateDemoSkip(this, this);
    rs::setDemoInfoDemoName(this, "月の石開放デモ");
    al::setNerve(this, &DemoMoonSetStart);
}

void MoonRock::exeDemoMoonSetStart() {
    if (!al::isActionEnd(this))
        return;

    rs::showDemoPlayer(this);
    al::setNerve(this, &EndDemoMoonSetStart);
}

void MoonRock::exeEndDemoMoonSetStart() {
    if (al::isFirstStep(this)) {
        rs::requestEndDemoWithPlayerCinemaFrame(this);
        al::endCamera(this, mDemoCamera, -1, false);
    }

    if (rs::isActiveDemo(this))
        return;

    GameDataHolderWriter writer(this);
    const ChangeStageInfo* changeStageInfo = mChangeStageInfo;
    GameDataFunction::tryChangeNextStage(writer, changeStageInfo);
    al::setNerve(this, &ChangeScene);
}

void MoonRock::exeChangeScene() {}
