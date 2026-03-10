#include "MapObj/RiseMapPartsHolder.h"

#include <math/seadVector.h>

#include "Library/Camera/CameraUtil.h"
#include "Library/Demo/DemoFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActorGroup.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

#include "Boss/BossUtil/BossUtil.h"
#include "MapObj/RiseMapParts.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"
#include "System/GameDataUtil.h"
#include "System/SaveObjInfo.h"
#include "Util/DemoUtil.h"
#include "Util/PlayerDemoUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(RiseMapPartsHolder, Wait);
NERVE_IMPL(RiseMapPartsHolder, Move);
NERVE_IMPL(RiseMapPartsHolder, MoveEnd);
NERVE_IMPL(RiseMapPartsHolder, Stop);
NERVE_IMPL(RiseMapPartsHolder, RequestDemo);
NERVE_IMPL(RiseMapPartsHolder, Delay);

NERVES_MAKE_STRUCT(RiseMapPartsHolder, Wait, Stop, Move, MoveEnd, RequestDemo, Delay);
}  // namespace

RiseMapPartsHolder::RiseMapPartsHolder(const char* name) : al::LiveActor(name) {}

// NON_MATCHING: https://decomp.me/scratch/lCb5J
void RiseMapPartsHolder::init(const al::ActorInitInfo& info) {
    using RiseMapPartsHolderFunctor =
        al::FunctorV0M<RiseMapPartsHolder*, void (RiseMapPartsHolder::*)()>;

    al::initActorSceneInfo(this, info);
    al::initActorPoseTRSV(this);
    al::initActorClipping(this, info);
    al::initExecutorUpdate(this, info, "コリジョン地形[Movement]");
    al::initStageSwitch(this, info);
    initHitSensor(1);
    al::addHitSensorMapObj(this, info, "Body", 0.0f, 64, {0.0f, 0.0f, 0.0f});
    mAddDemoInfo = al::registDemoRequesterToAddDemoInfo(this, info, 0);
    if (al::isObjectNameSubStr(info, "Switch"))
        mIsSwitch = true;

    mLinkedShineIndex = GameDataFunction::tryFindLinkedShineIndex(this, info);
    al::initNerve(this, &NrvRiseMapPartsHolder.Wait, 0);
    _108 = rs::createSaveObjInfoWriteSaveData(info);
    _110 = rs::createSaveObjInfoNoWriteSaveDataInSameWorld(info);

    s32 childNum = al::calcLinkChildNum(info, "RiseParts");
    mRiseMapPartsGroup = new al::DeriveActorGroup<RiseMapParts>("地形隆起パーツ", childNum);

    for (s32 i = 0; i < childNum; i++) {
        RiseMapParts* mapParts =
            new RiseMapParts(al::getLinksActorDisplayName(info, "RiseParts", i));
        al::initLinksActor(mapParts, info, "RiseParts", i);
        mRiseMapPartsGroup->registerActor(mapParts);
    }

    if (al::isExistLinkChild(info, "PlayerRestartPos", 0)) {
        al::getLinksMatrix(&mPlayerRestartPosMtx, info, "PlayerRestartPos");
        mHasRestartPos = true;
    }

    bool isValidObjectCamera = false;
    al::tryGetArg(&isValidObjectCamera, info, "IsValidObjectCamera");
    if (isValidObjectCamera)
        mDemoCamera = al::initDemoObjectCamera(this, info, nullptr, "固定");

    al::tryGetArg(&mIsResetFirstPosInMiniGame, info, "ResetFirstPosInMiniGame");
    al::tryGetArg(&mRelatedBoss, info, "RelatedBoss");
    al::tryGetArg(&mDelayFrame, info, "DelayFrame");

    if (mDelayFrame > -1)
        mDelayFrame = -1;

    if (mIsSwitch) {
        makeActorAlive();
        return;
    }

    bool isStageSwitchListening = al::listenStageSwitchOnStart(
        this, RiseMapPartsHolderFunctor(this, &RiseMapPartsHolder::startRise));

    bool isDead = false;
    if (rs::isSequenceTimeBalloonOrRace(this))
        isDead = true;
    else if (mLinkedShineIndex < 0)
        isDead = rs::isOnSaveObjInfo(_108);
    else if (GameDataFunction::isGotShine(GameDataHolderAccessor(this), mLinkedShineIndex))
        isDead = true;
    else if (mLinkedShineIndex < 0)
        isDead = rs::isOnSaveObjInfo(_108);

    if (mRelatedBoss > -1)
        isDead = rs::isAlreadyDeadGK(this, mRelatedBoss, 3);

    if (isDead) {
        if (rs::isSequenceTimeBalloonOrRace(this) && mIsResetFirstPosInMiniGame) {
            makeActorDead();
            al::setNerve(this, &NrvRiseMapPartsHolder.Wait);
            return;
        }

        al::invalidateClipping(this);
        al::setNerve(this, &NrvRiseMapPartsHolder.Stop);
        makeActorAlive();
        return;
    }

    if (al::listenStageSwitchOnAppear(
            this, RiseMapPartsHolderFunctor(this, &RiseMapPartsHolder::startRise))) {
        for (s32 i = 0; i < childNum; i++)
            mRiseMapPartsGroup->getDeriveActor(i)->kill();
        makeActorDead();
        return;
    }

    if (isStageSwitchListening) {
        makeActorAlive();
        return;
    }

    s32 scenarioNo = -1;
    if (!al::tryGetArg(&scenarioNo, info, "ScenarioNo")) {
        makeActorDead();
        return;
    }

    makeActorAlive();
}

void RiseMapPartsHolder::startRise() {
    if (mRelatedBoss <= -1 && !mIsSwitch && rs::isOnSaveObjInfo(_108))
        return;

    if (al::isDead(this))
        appear();

    if (al::isNerve(this, &NrvRiseMapPartsHolder.Stop) ||
        al::isNerve(this, &NrvRiseMapPartsHolder.Move) ||
        al::isNerve(this, &NrvRiseMapPartsHolder.RequestDemo)) {
        return;
    }

    if (!mIsSwitch)
        rs::onSaveObjInfo(_110);

    al::invalidateClipping(this);
    rs::addDemoRiseMapPartsToList(this);
    al::setNerve(this, &NrvRiseMapPartsHolder.RequestDemo);
}

bool RiseMapPartsHolder::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                    al::HitSensor* self) {
    if (rs::isMsgPlayerDisregardTargetMarker(message) ||
        rs::isMsgPlayerDisregardHomingAttack(message) || al::isMsgPlayerDisregard(message) ||
        al::isMsgSwitchOffInit(message)) {
        return true;
    }

    if (rs::isMsgSwitchOnWithSaveRequest(message, &mSwitchOnSaveInfo)) {
        startRise();
        return true;
    }

    if (al::isMsgSwitchOnInit(message)) {
        al::invalidateClipping(this);
        al::setNerve(this, &NrvRiseMapPartsHolder.Stop);
        return true;
    }

    return false;
}

void RiseMapPartsHolder::exeWait() {}

void RiseMapPartsHolder::exeMove() {
    if (al::isFirstStep(this)) {
        if (al::isActiveCameraInterpole(this, 0)) {
            al::setNerve(this, &NrvRiseMapPartsHolder.Move);
            return;
        }

        startRiseAllChild();
    }

    s32 actorCount = mRiseMapPartsGroup->getActorCount();
    bool allStopped = true;
    for (s32 i = 0; i < actorCount; i++) {
        if (!mRiseMapPartsGroup->getDeriveActor(i)->isStop()) {
            allStopped = false;
            break;
        }
    }

    if (allStopped) {
        al::tryOnStageSwitch(this, "SwitchStopOn");
        al::setNerve(this, &NrvRiseMapPartsHolder.MoveEnd);
    }
}

void RiseMapPartsHolder::startRiseAllChild() {
    if (!mIsSwitch && mRelatedBoss == -1)
        rs::onSaveObjInfo(_108);

    s32 actorCount = mRiseMapPartsGroup->getActorCount();
    for (s32 i = 0; i < actorCount; i++)
        mRiseMapPartsGroup->getDeriveActor(i)->startRise();
}

void RiseMapPartsHolder::exeMoveEnd() {
    if (!al::isGreaterEqualStep(this, 90))
        return;

    if (mDemoCamera)
        al::endCamera(this, mDemoCamera, -1, false);

    if (mHasRestartPos) {
        sead::Vector3f trans;
        mPlayerRestartPosMtx.getTranslation(trans);
        sead::Quatf quat = sead::Quatf::unit;
        mPlayerRestartPosMtx.toQuat(quat);
        rs::replaceDemoPlayer(this, trans, quat);
    }

    if (_188 && rs::isActiveDemo(this)) {
        rs::requestEndDemoRiseMapParts(this);
        if (mSwitchOnSaveInfo)
            rs::onSaveObjInfo(mSwitchOnSaveInfo);
    }

    kill();
}

void RiseMapPartsHolder::exeStop() {
    if (al::isFirstStep(this)) {
        s32 actorCount = mRiseMapPartsGroup->getActorCount();
        for (s32 i = 0; i < actorCount; i++)
            mRiseMapPartsGroup->getDeriveActor(i)->setStop();

        al::tryOnStageSwitch(this, "SwitchStopOn");
        kill();
    }
}

void RiseMapPartsHolder::exeRequestDemo() {}

void RiseMapPartsHolder::exeDelay() {
    if (al::isGreaterEqualStep(this, mDelayFrame)) {
        startRiseAllChild();
        al::setNerve(this, &NrvRiseMapPartsHolder.Move);
    }
}

// TODO: Rename `unk`
void RiseMapPartsHolder::startDemo(bool unk) {
    al::invalidateClipping(this);
    if (unk)
        _188 = false;

    if (mDemoCamera)
        al::startCamera(this, mDemoCamera, -1);

    if (mDelayFrame >= 0) {
        al::setNerve(this, &NrvRiseMapPartsHolder.Delay);
        return;
    }

    if (unk)
        rs::addDemoActor(this, false);
    else if (mSwitchActor)
        rs::addDemoActor(mSwitchActor, false);

    if (mRiseMapPartsArray)
        for (s32 i = 0; i < mRiseMapPartsNum; i++)
            rs::addDemoActor(mRiseMapPartsArray[i], false);

    al::addDemoActorFromAddDemoInfo(this, mAddDemoInfo);
    al::setNerve(this, &NrvRiseMapPartsHolder.Move);
}
