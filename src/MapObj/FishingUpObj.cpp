#include "MapObj/FishingUpObj.h"

#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/Base/StringUtil.h"
#include "Library/Event/EventFlowFunction.h"
#include "Library/Event/EventFlowUtil.h"
#include "Library/Joint/JointControllerKeeper.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorResourceFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Yaml/ByamlUtil.h"

#include "Npc/TalkNpcActionAnimInfo.h"
#include "Npc/TalkNpcStateEvent.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"
#include "Util/DemoUtil.h"
#include "Util/ItemGenerator.h"
#include "Util/NpcAnimUtil.h"
#include "Util/NpcEventFlowUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(FishingUpObj, PopUp);
NERVE_IMPL(FishingUpObj, Event);
NERVE_IMPL(FishingUpObj, FishingUp);
NERVE_IMPL(FishingUpObj, Land);

NERVES_MAKE_NOSTRUCT(FishingUpObj, PopUp, Event, FishingUp, Land);
}  // namespace

FishingUpObj::FishingUpObj(const char* name) : al::LiveActor(name) {}

void FishingUpObj::init(const al::ActorInitInfo& info) {
    const char* archiveName = "Pukupuku";
    const char* suffix = "FishingUpObj";
    al::tryGetStringArg(&archiveName, info, "FishingUpObj");
    if (!al::isEqualString("Pukupuku", archiveName))
        suffix = nullptr;

    al::initActorWithArchiveName(this, info, archiveName, suffix);
    al::initNerve(this, &PopUp, 1);

    al::ByamlIter initFishingIter;
    if (al::tryGetActorInitFileIter(&initFishingIter, this, "InitFishing", suffix))
        al::tryGetByamlString(&mItemGeneratePositionJointName, initFishingIter,
                              "ItemGeneratePositionJointName");

    if (al::isEqualString(archiveName, "KinopioBrigade")) {
        mTalkNpcParam = rs::initTalkNpcParam(this, suffix);
        mTalkNpcActionAnimInfo = new TalkNpcActionAnimInfo();
        mTalkNpcActionAnimInfo->setWaitActionName("Wait");
        mTalkNpcActionAnimInfo->init(this, info, mTalkNpcParam, suffix);

        al::EventFlowExecutor* eventFlowExecutor =
            rs::initEventFlow(this, info, "FishingKinopioBrigade", nullptr);
        rs::initEventMovementTurnSeparate(eventFlowExecutor, info);
        rs::initEventActionNameConverter(eventFlowExecutor, mTalkNpcActionAnimInfo);
        al::initEventReceiver(eventFlowExecutor, this);
        rs::initEventCameraTalk(eventFlowExecutor, info, "Default", 800.0f);
        rs::initEventCharacterName(eventFlowExecutor, info, "KinopioBrigade");

        al::initJointControllerKeeper(this, 1);
        mJointLookAtController =
            rs::tryCreateAndAppendNpcJointLookAtController(this, mTalkNpcParam);

        mTalkNpcStateEvent =
            new TalkNpcStateEvent("イベント", this, mTalkNpcParam, eventFlowExecutor, -1);
        mTalkNpcStateEvent->initStateReaction(rs::createNpcStateReaction(
            this, mTalkNpcParam, mTalkNpcActionAnimInfo->getReactionParam()));
        al::initNerveState(this, mTalkNpcStateEvent, &Event, "イベント");

        if (al::isExistLinkChild(info, "ShineActor", 0)) {
            mLinkedShineIndex =
                GameDataFunction::tryFindLinkedShineIndexByLinkName(this, info, "ShineActor");
            if (GameDataFunction::isGotShine(this, mLinkedShineIndex)) {
                mTalkNpcStateEvent->trySwitchEventEntry("WaitAfter");
                al::setNerve(this, &Event);
                makeActorAlive();
                return;
            }
        }
    } else {
        mItemGenerator = new ItemGenerator();
        mItemGenerator->initNoLinkShine(this, info, false);
    }

    makeActorDead();
}

void FishingUpObj::appear() {
    al::LiveActor::appear();

    ItemGenerator* itemGenerator = mItemGenerator;
    if (isKinopioBrigade()) {
        al::setScaleAll(this, 1.0f);
    } else {
        bool isPochiActor = isPochi();
        if (itemGenerator && !isPochiActor && itemGenerator->isShine())
            al::setScaleAll(this, 1.75f);
        else
            al::setScaleAll(this, 1.0f);
    }

    if (mTalkNpcStateEvent)
        al::setNerve(this, &PopUp);
    else
        al::setNerve(this, &FishingUp);
}

void FishingUpObj::kill() {
    ItemGenerator* itemGenerator = mItemGenerator;
    const char* hitReaction;

    if (isKinopioBrigade()) {
        hitReaction = "消滅[釣り]";
    } else {
        hitReaction = "消滅[釣り]";
        bool isPochiActor = isPochi();
        if (itemGenerator && !isPochiActor && itemGenerator->isShine())
            hitReaction = "消滅(大)[釣り]";
    }

    al::startHitReaction(this, hitReaction);
    al::LiveActor::kill();
}

void FishingUpObj::updateCollider() {
    al::LiveActor::updateCollider();
    if (al::isNerve(this, &PopUp) && al::isOnGround(this, 0)) {
        al::setVelocityZero(this);

        sead::Vector3f groundNormal = al::getCollidedGroundNormal(this);
        sead::Vector3f front = {0.0f, 0.0f, 0.0f};
        al::calcFrontDir(&front, this);
        al::verticalizeVec(&front, groundNormal, front);
        if (!al::tryNormalizeOrZero(&front))
            al::calcFrontDir(&front, this);

        sead::Quatf quat = sead::Quatf::unit;
        al::makeQuatFrontUp(&quat, front, groundNormal);
        al::updatePoseQuat(this, quat);
        al::startAction(this, "Land");
        al::setNerve(this, &Land);
    }
}

void FishingUpObj::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    rs::attackSensorNpcCommon(self, other);
    if (mTalkNpcStateEvent)
        rs::sendMsgEventFlowScareCheck(other, self, mTalkNpcStateEvent->getEventFlowExecutor());
}

bool FishingUpObj::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                              al::HitSensor* self) {
    if (al::isMsgPlayerDisregard(message))
        return !isKinopioBrigade();

    if (rs::isMsgPlayerDisregardHomingAttack(message))
        return isKinopioBrigade();

    if (al::isNerve(this, &PopUp))
        return false;

    if (rs::isInvalidTrampleSensor(self, mTalkNpcParam)) {
        if (mTalkNpcStateEvent->tryStartReactionWithoutTrample(message, other, self))
            return true;
        return rs::receiveMsgNpcCommonNoReaction(message, other, self);
    } else {
        if (mTalkNpcStateEvent->tryStartReaction(message, other, self))
            return true;
        return rs::receiveMsgNpcCommonNoReaction(message, other, self);
    }
}

bool FishingUpObj::isKinopioBrigade() const {
    return al::isEqualString(al::getModelName(this), "KinopioBrigade");
}

bool FishingUpObj::receiveEvent(const al::EventFlowEventData* event) {
    if (TalkNpcFunction::receiveEventChangeWaitAction(mTalkNpcActionAnimInfo, event, mTalkNpcParam))
        return true;

    if (al::isEventName(event, "TakeOut")) {
        if (mItemGenerator)
            mItemGenerator->generate(al::getTrans(this), al::getFront(this));

        return true;
    }

    return false;
}

bool FishingUpObj::isPochi() const {
    return al::isEqualString(al::getModelName(this), "Pochi");
}

bool FishingUpObj::isExistShine() const {
    return mItemGenerator && mItemGenerator->isShine();
}

bool FishingUpObj::isAfterGiveShine() const {
    if (mLinkedShineIndex < 0)
        return false;

    return GameDataFunction::isGotShine(this, mLinkedShineIndex);
}

bool FishingUpObj::isValidAfterAppearDemoEvent() const {
    if (!mTalkNpcStateEvent)
        return false;

    if (mLinkedShineIndex >= 0)
        return true;

    return !GameDataFunction::isGotShine(this, mLinkedShineIndex);
}

bool FishingUpObj::tryStartAppearAndStartDemoEvent() {
    return rs::tryStartEventCutSceneKeepHackDemo(this);
}

bool FishingUpObj::tryUpdateShineHintTrans(const sead::Vector3f& trans) {
    if (mLinkedShineIndex >= 0) {
        if (!GameDataFunction::isGotShine(this, mLinkedShineIndex)) {
            GameDataFunction::setHintTrans(this, mLinkedShineIndex, trans);
            return true;
        }
    }

    if (mItemGenerator)
        return mItemGenerator->tryUpdateHintTransIfExistShine(trans);

    return false;
}

void FishingUpObj::exeFishingUp() {
    if (al::isFirstStep(this)) {
        ItemGenerator* itemGenerator = mItemGenerator;
        const char* actionName;
        if (isKinopioBrigade()) {
            actionName = "FishingUp";
        } else {
            actionName = "FishingUp";
            bool isPochiActor = isPochi();
            if (itemGenerator && !isPochiActor && itemGenerator->isShine())
                actionName = "FishingUpGold";
        }

        al::startAction(this, actionName);
    }

    if (al::isActionEnd(this)) {
        sead::Vector3f itemGeneratePosition = al::getTrans(this);
        if (mItemGeneratePositionJointName)
            al::calcJointPos(&itemGeneratePosition, this, mItemGeneratePositionJointName);

        if (mItemGenerator)
            mItemGenerator->generate(itemGeneratePosition, al::getQuat(this));

        kill();
    }
}

void FishingUpObj::exePopUp() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Appear");
        al::setVelocityJump(this, 45.0f);
    }

    al::addVelocityToGravity(this, 1.5f);
    al::scaleVelocity(this, 0.975f);
}

void FishingUpObj::exeLand() {
    if (al::isActionEnd(this)) {
        if (rs::isActiveEventDemo(this) && rs::isEqualEventDemoStartActor(this))
            rs::setDemoInfoDemoName(this, "キノピオ隊長会話デモ");

        al::setNerve(this, &Event);
    }
}

void FishingUpObj::exeEvent() {
    al::updateNerveState(this);
    if (mJointLookAtController)
        rs::updateNpcJointLookAtController(mJointLookAtController);
}
