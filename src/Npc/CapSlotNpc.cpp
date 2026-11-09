#include "Npc/CapSlotNpc.h"

#include "Library/Base/StringUtil.h"
#include "Library/Event/EventFlowFunction.h"
#include "Library/Event/EventFlowUtil.h"
#include "Library/Joint/JointControllerKeeper.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Npc/NpcStateReaction.h"
#include "Util/IItemExistanceJudge.h"
#include "Util/ItemUtil.h"
#include "Util/NpcActionUtil.h"
#include "Util/NpcAnimUtil.h"
#include "Util/NpcEventFlowUtil.h"

namespace {
NERVE_IMPL(CapSlotNpc, Wait);
NERVE_IMPL(CapSlotNpc, Reaction);

NERVES_MAKE_STRUCT(CapSlotNpc, Wait, Reaction);
}  // namespace

void CapSlotNpc::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "DesertMan", "Slot");
    mTalkNpcParam = rs::initTalkNpcParam(this, nullptr);
    al::initJointControllerKeeper(this, 8);
    mNpcJointLookAtController =
        rs::tryCreateAndAppendNpcJointLookAtController(this, mTalkNpcParam);
    al::initNerve(this, &NrvCapSlotNpc.Wait, 8);
    mNpcStateReaction = NpcStateReaction::create(this, nullptr);
    al::initNerveState(this, mNpcStateReaction, &NrvCapSlotNpc.Reaction, "リアクション");
    mEventFlowExecutor = rs::initEventFlow(this, info, "CapSlotNpc", "CapSlotEvent");
    rs::initEventCharacterName(mEventFlowExecutor, info, "CapSlotNpc");
    al::initEventReceiver(mEventFlowExecutor, this);
    rs::initEventQueryJudge(mEventFlowExecutor, this);
    rs::startEventFlow(mEventFlowExecutor, "Init");
    al::startVisAnim(this, "Off");
    makeActorAlive();
}

void CapSlotNpc::movement() {
    if (mNpcJointLookAtController) {
        rs::updateNpcJointLookAtController(mNpcJointLookAtController);
    }
    al::LiveActor::movement();
}

bool CapSlotNpc::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                            al::HitSensor* self) {
    if (al::isMsgPlayerDisregard(message)) {
        return !al::isSensorName(self, "Head");
    }
    if (mNpcStateReaction->receiveMsg(message, other, self)) {
        if (!al::isNerve(this, &NrvCapSlotNpc.Reaction)) {
            al::setNerve(this, &NrvCapSlotNpc.Reaction);
        }
        return true;
    }
    return mNpcStateReaction->receiveMsgNoReaction(message, other, self);
}

void CapSlotNpc::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    rs::attackSensorNpcCommon(self, other);
}

void CapSlotNpc::exeWait() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Wait");
        al::validateClipping(this);
    }
    rs::updateEventFlow(mEventFlowExecutor);
}

void CapSlotNpc::exeReaction() {
    al::updateNerveStateAndNextNerve(this, &NrvCapSlotNpc.Wait);
}

bool CapSlotNpc::receiveEvent(const al::EventFlowEventData* event) {
    if (al::isEventName(event, "Start")) {
        _118 = 1;
        return true;
    }
    if (al::isEventName(event, "Reset")) {
        _118 = 4;
        return true;
    }
    if (al::isEventName(event, "Quit")) {
        _118 = 5;
        return true;
    }
    return false;
}

const char* CapSlotNpc::judgeQuery(const char* query) const {
    if (al::isEqualString(query, "JudgeSuccess")) {
        if (_118 == 2) {
            return "Success";
        }
        if (_118 == 3) {
            return "Miss";
        }
        return "Playing";
    }
    if (al::isEqualString(query, "JudgeItemExistance")) {
        return mItemExistanceJudge->isExist() ? "Exist" : "NoExist";
    }
    if (al::isEqualString(query, "JudgeIsGotShine")) {
        return rs::isGotShine(mShine) ? "Yes" : "No";
    }
    return nullptr;
}
