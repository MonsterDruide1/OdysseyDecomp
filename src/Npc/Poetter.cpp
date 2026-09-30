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

static NpcStateReactionParam sReactionParam("Reaction", "ReactionCap");
static NpcEventStateScareActionParam sScareParam("Scared");

Poetter::Poetter(const char* name) : al::LiveActor(name) {}

// NON_MATCHING: regalloc caches "PoetterHome" in x23; vtable offset 0x20 vs 0x18
void Poetter::init(const al::ActorInitInfo& initInfo) {
    al::initActor(this, initInfo);
    al::initNerve(this, &Wait, 4);
    makeActorAlive();

    mTalkParam = rs::initTalkNpcParam(this, nullptr);

    bool isPlaceWithHome = true;
    al::tryGetArg(&isPlaceWithHome, initInfo, "IsPlaceWithHome");
    if (isPlaceWithHome) {
        auto* home = new al::FixMapParts("PoetterHome");
        mHome = home;
        al::initChildActorWithArchiveNameWithPlacementInfo(home, initInfo, "PoetterHome", nullptr);
        mHome->makeActorAlive();
        sead::Vector3f pos = al::getTrans(mHome) + sead::Vector3f::ey * 82.0f;
        al::resetPosition(this, pos);
    }

    auto* scareState = new NpcEventStateScare(this, &sScareParam);
    mScareState = scareState;
    mReactionState = NpcStateReaction::createForHuman(this, &sReactionParam);
    al::initNerveState(this, mScareState, &EventScare, u8"イベント中の怖がり");
    al::initNerveState(this, mReactionState, &Reaction, u8"リアクション");

    mMessageSystem = initInfo.layoutInitInfo->getMessageSystem();
    mEventFlowExecutor = rs::initEventFlow(this, initInfo, nullptr, nullptr);
    rs::initEventCharacterName(mEventFlowExecutor, initInfo, "Hint_Bird");
    rs::initEventParam(mEventFlowExecutor, mTalkParam, nullptr);
    rs::initEventCameraObject(mEventFlowExecutor, initInfo, "Default");
    rs::initEventMovementTurnSeparate(mEventFlowExecutor, initInfo);
    rs::startEventFlow(mEventFlowExecutor, "Wait");

    auto* tagHolder = al::initMessageTagDataHolder(1);
    al::registerMessageTagDataString(tagHolder, "MoonName", &mHintMessage);
    rs::initEventMessageTagDataHolder(mEventFlowExecutor, tagHolder);

    {
        GameDataHolderAccessor accessor(this);
        if (GameDataFunction::isMainStage(accessor)) {
            GameDataHolderAccessor accessor2(this);
            GameDataFunction::setPoetterTrans(accessor2, al::getTrans(this));
        }
    }

    mJointSpringHolder = al::JointSpringControllerHolder::tryCreateAndInitJointControllerKeeper(
        this, "InitJointSpringCtrl");
    mJointSpringHolder->onControlAll();
}

void Poetter::control() {}

void Poetter::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isSensorEye(self)) {
        if (mEventFlowExecutor)
            rs::sendMsgEventFlowScareCheck(other, self, mEventFlowExecutor);
    } else {
        rs::attackSensorNpcCommon(self, other);
    }
}

bool Poetter::receiveMsg(const al::SensorMsg* msg, al::HitSensor* other, al::HitSensor* self) {
    if (rs::isMsgPlayerDisregardHomingAttack(msg))
        return true;
    if (rs::isMsgPlayerDisregardTargetMarker(msg))
        return true;
    if (mReactionState->receiveMsg(msg, other, self)) {
        if (!al::isNerve(this, &Reaction))
            al::setNerve(this, &Reaction);
        return true;
    }
    return mReactionState->receiveMsgNoReaction(msg, other, self);
}

// NON_MATCHING: stack layout (0x20 vs 0x30 locals), count variable placement, b.ge vs b.pl
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
        GameDataHolderAccessor accessor(this);
        s32 worldId = GameDataFunction::getCurrentWorldId(accessor);
        if (worldId < 0) {
            rs::startEventFlow(mEventFlowExecutor, "TalkNoMore");
            al::setNerve(this, &Event);
            return;
        }

        s32 indices[1024];
        s32 unlockableCount;
        rs::calcShineIndexTableNameUnlockable(indices, &unlockableCount, this);

        s32 unlockedIndex;
        if (unlockableCount < 1) {
            unlockedIndex = -1;
        } else {
            unlockedIndex = indices[al::getRandom(0, unlockableCount)];
            if (!rs::tryUnlockShineName(this, unlockedIndex))
                unlockedIndex = -1;
        }

        s32 availableCount;
        rs::calcShineIndexTableNameAvailable(indices, &availableCount, this);

        if (availableCount < 0) {
            makeActorDead();
            return;
        }

        if (availableCount == 0) {
            rs::startEventFlow(mEventFlowExecutor, "TalkNoMore");
            al::setNerve(this, &Event);
            return;
        }

        s32 index = mHintIndex;
        if (index >= availableCount || index >= 3) {
            index = 0;
            mHintIndex = 0;
        }

        if (unlockedIndex >= 0) {
            for (s32 i = 0; i < availableCount; i++) {
                if (indices[i] == unlockedIndex) {
                    mHintIndex = i;
                    index = i;
                    break;
                }
            }
        }

        mHintMessage = GameDataFunction::tryFindShineMessage(this, this, worldId, indices[index]);
        rs::startEventFlow(mEventFlowExecutor, "TalkShow");
        al::setNerve(this, &Event);
        return;
    }

    sead::Vector3f capPos;
    if (!rs::tryGetFlyingCapPos(&capPos, this)) {
        mCapWatchCount = 0;
        goto checkRolling;
    }

    {
        const sead::Vector3f& trans = al::getTrans(this);
        sead::Vector3f diff = {capPos.x - trans.x, capPos.y - trans.y, capPos.z - trans.z};
        if (diff.x * diff.x + diff.y * diff.y + diff.z * diff.z >= 422500.0f) {
            mCapWatchCount = 0;
            goto checkRolling;
        }

        sead::Vector3f front = al::getFront(this);
        if (al::calcAngleDegree(front, diff) < 80.0f) {
            if (mCapWatchCount++ >= 49) {
                if (al::isActionPlaying(this, "Wait") || al::isActionPlaying(this, "RollingEnd"))
                    al::startAction(this, "RollingStart");
                else if (al::isActionPlaying(this, "RollingStart") && al::isActionEnd(this))
                    al::startAction(this, "Rolling");
                goto checkYawn;
            }
        } else {
            mCapWatchCount = 0;
        }
    }

checkRolling:
    if (al::isActionPlaying(this, "Rolling") || al::isActionPlaying(this, "RollingStart"))
        al::startAction(this, "RollingEnd");
    else if (al::isActionPlaying(this, "RollingEnd") && al::isActionEnd(this))
        al::startAction(this, "Wait");

checkYawn:
    if (al::isActionPlaying(this, "Yawn") && al::isActionEnd(this)) {
        al::startAction(this, "Wait");
    } else if (al::isActionPlaying(this, "Wait")) {
        if (--mYawnWait <= 0)
            al::startAction(this, "Yawn");
    } else {
        mYawnWait = al::getRandom(600, 6000);
    }
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

const al::MessageSystem* Poetter::getMessageSystem() const {
    return mMessageSystem;
}
