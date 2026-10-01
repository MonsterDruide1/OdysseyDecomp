#include "Npc/Poetter.h"

#include <math/seadVector.h>

#include "Library/Joint/JointSpringControllerHolder.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/MapObj/FixMapParts.h"
#include "Library/Math/MathUtil.h"
#include "Library/Message/MessageHolder.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"

#include "Npc/NpcEventStateScare.h"
#include "Npc/NpcStateReaction.h"
#include "Npc/NpcStateReactionParam.h"
#include "System/GameDataFunction.h"
#include "System/GameDataUtil.h"
#include "Util/NpcActionUtil.h"
#include "Util/NpcAnimUtil.h"
#include "Util/NpcEventFlowUtil.h"
#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(Poetter, Wait);
NERVE_IMPL(Poetter, EventScare);
NERVE_IMPL(Poetter, Reaction);
NERVE_IMPL(Poetter, Event);

NERVES_MAKE_NOSTRUCT(Poetter, Wait, EventScare, Reaction, Event);
}  // namespace

static const NpcStateReactionParam sReactionParam("Reaction", "ReactionCap");
static const NpcEventStateScareActionParam sScareParam("Scared");

Poetter::Poetter(const char* name) : al::LiveActor(name) {}

void Poetter::init(const al::ActorInitInfo& initInfo) {
    al::initActor(this, initInfo);
    al::initNerve(this, &Wait, 4);
    makeActorAlive();

    mTalkParam = rs::initTalkNpcParam(this, nullptr);

    bool isPlaceWithHome = true;
    al::tryGetArg(&isPlaceWithHome, initInfo, "IsPlaceWithHome");
    if (isPlaceWithHome) {
        mHome = new al::FixMapParts("ホーム");
        al::initChildActorWithArchiveNameWithPlacementInfo(mHome, initInfo, "PoetterHome", nullptr);
        mHome->appear();

        al::resetPosition(this, al::getTrans(mHome) + sead::Vector3f::ey * 82.0f);
    }

    mScareState = new NpcEventStateScare(this, &sScareParam);
    mReactionState = NpcStateReaction::createForHuman(this, &sReactionParam);
    al::initNerveState(this, mScareState, &EventScare, "イベント中の怖がり");
    al::initNerveState(this, mReactionState, &Reaction, "リアクション");

    mMessageSystem = initInfo.layoutInitInfo->getMessageSystem();
    mEventFlowExecutor = rs::initEventFlow(this, initInfo, nullptr, nullptr);
    rs::initEventCharacterName(mEventFlowExecutor, initInfo, "Hint_Bird");
    rs::initEventParam(mEventFlowExecutor, mTalkParam, nullptr);
    rs::initEventCameraObject(mEventFlowExecutor, initInfo, "Default");
    rs::initEventMovementTurnSeparate(mEventFlowExecutor, initInfo);
    rs::startEventFlow(mEventFlowExecutor, "Wait");

    al::MessageTagDataHolder* tagHolder = al::initMessageTagDataHolder(1);
    al::registerMessageTagDataString(tagHolder, "MoonName", &mHintMessage);
    rs::initEventMessageTagDataHolder(mEventFlowExecutor, tagHolder);

    if (GameDataFunction::isMainStage(this))
        GameDataFunction::setPoetterTrans(this, al::getTrans(this));

    mJointSpringHolder = al::JointSpringControllerHolder::tryCreateAndInitJointControllerKeeper(
        this, "InitJointSpringCtrl");
    mJointSpringHolder->onControlAll();
}

void Poetter::control() {}

void Poetter::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (!al::isSensorEye(self)) {
        rs::attackSensorNpcCommon(self, other);
        return;
    }

    if (mEventFlowExecutor)
        rs::sendMsgEventFlowScareCheck(other, self, mEventFlowExecutor);
}

bool Poetter::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    if (rs::isMsgPlayerDisregardHomingAttack(message) ||
        rs::isMsgPlayerDisregardTargetMarker(message))
        return true;

    if (mReactionState->receiveMsg(message, other, self)) {
        if (!al::isNerve(this, &Reaction))
            al::setNerve(this, &Reaction);
        return true;
    }

    return mReactionState->receiveMsgNoReaction(message, other, self);
}

void Poetter::exeWait() {
    if (al::isFirstStep(this)) {
        mCapWatchCount = 0;
        al::startAction(this, "Wait");
        rs::restartEventFlow(mEventFlowExecutor);
    }

    if (mScareState->tryStart(mEventFlowExecutor)) {
        rs::stopEventFlow(mEventFlowExecutor);
        al::setNerve(this, &EventScare);
        return;
    }

    if (rs::updateEventFlow(mEventFlowExecutor)) {
        s32 worldId = GameDataFunction::getCurrentWorldId(GameDataHolderAccessor(this));
        if (worldId < 0) {
            startTalkNoMore();
            return;
        }

        s32 indices[1024];
        s32 unlockableCount;
        rs::calcShineIndexTableNameUnlockable(indices, &unlockableCount, this);

        s32 unlockedIndex = -1;
        if (unlockableCount > 0) {
            unlockedIndex = indices[al::getRandom(0, unlockableCount)];
            if (!rs::tryUnlockShineName(this, unlockedIndex))
                unlockedIndex = -1;
        }

        s32 availableNameCount;
        rs::calcShineIndexTableNameAvailable(indices, &availableNameCount, this);

        if (availableNameCount < 0) {
            makeActorDead();
            return;
        }

        if (availableNameCount == 0) {
            startTalkNoMore();
            return;
        }

        if (mHintIndex >= availableNameCount || mHintIndex >= 3)
            mHintIndex = 0;

        if (unlockedIndex >= 0) {
            for (s32 i = 0; i < availableNameCount; i++) {
                if (indices[i] == unlockedIndex) {
                    mHintIndex = i;
                    break;
                }
            }
        }

        mHintMessage =
            GameDataFunction::tryFindShineMessage(this, this, worldId, indices[mHintIndex]);
        rs::startEventFlow(mEventFlowExecutor, "TalkShow");
        al::setNerve(this, &Event);
        return;
    }

    bool isCapInSight = false;
    sead::Vector3f capPos;
    if (rs::tryGetFlyingCapPos(&capPos, this)) {
        sead::Vector3f capDiff = capPos - al::getTrans(this);
        if (capDiff.squaredLength() < 422500.0f) {
            sead::Vector3f front = al::getFront(this);
            isCapInSight = al::calcAngleDegree(front, capDiff) < 80.0f;
            if (!isCapInSight)
                mCapWatchCount = 0;
        } else {
            mCapWatchCount = 0;
        }
    } else {
        mCapWatchCount = 0;
    }

    if (isCapInSight && mCapWatchCount++ >= 49) {
        if (al::isActionPlaying(this, "Wait") || al::isActionPlaying(this, "RollingEnd"))
            al::startAction(this, "RollingStart");
        else if (al::isActionPlaying(this, "RollingStart") && al::isActionEnd(this))
            al::startAction(this, "Rolling");
    } else {
        if (al::isActionPlaying(this, "Rolling") || al::isActionPlaying(this, "RollingStart"))
            al::startAction(this, "RollingEnd");
        else if (al::isActionPlaying(this, "RollingEnd") && al::isActionEnd(this))
            al::startAction(this, "Wait");
    }

    if (al::isActionPlaying(this, "Yawn") && al::isActionEnd(this)) {
        al::startAction(this, "Wait");
        return;
    }

    if (!al::isActionPlaying(this, "Wait")) {
        resetYawnWait();
        return;
    }

    if (--mYawnWait <= 0)
        al::startAction(this, "Yawn");
}

void Poetter::startTalkNoMore() {
    rs::startEventFlow(mEventFlowExecutor, "TalkNoMore");
    al::setNerve(this, &Event);
}

void Poetter::resetYawnWait() {
    mYawnWait = al::getRandom(600, 6000);
}

void Poetter::exeEvent() {
    if (rs::updateEventFlow(mEventFlowExecutor)) {
        rs::startEventFlow(mEventFlowExecutor, "Wait");
        al::setNerve(this, &Wait);
        mHintIndex++;
    }
}

void Poetter::exeEventScare() {
    rs::updateEventFlow(mEventFlowExecutor);
    al::updateNerveStateAndNextNerve(this, &Wait);
}

void Poetter::exeReaction() {
    al::updateNerveStateAndNextNerve(this, &Wait);
}
