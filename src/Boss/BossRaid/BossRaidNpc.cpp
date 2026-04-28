#include "Boss/BossRaid/BossRaidNpc.h"

#include <math/seadVector.h>

#include "Library/Demo/DemoFunction.h"
#include "Library/Joint/JointAimInfo.h"
#include "Library/Joint/JointControllerKeeper.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/NpcEventFlowUtil.h"
#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(BossRaidNpc, Event);
NERVE_IMPL(BossRaidNpc, Reaction);
NERVES_MAKE_NOSTRUCT(BossRaidNpc, Event, Reaction);
}  // namespace

BossRaidNpc::BossRaidNpc(const char* name) : al::LiveActor(name) {}

void BossRaidNpc::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "BossRaid", "Npc");
    al::initNerve(this, &Event, 0);
    mEventFlowExecutor = rs::initEventFlow(this, info, "BossRaid", "Talk");
    if (rs::isDefinedEventCamera(mEventFlowExecutor, "Default"))
        rs::initEventCameraObject(mEventFlowExecutor, info, "Default");
    rs::startEventFlow(mEventFlowExecutor, "Init");
    al::initJointControllerKeeper(this, 2);

    mJointAimInfoL = new al::JointAimInfo();
    mJointAimInfoL->setPowerRate(1.0f);
    mJointAimInfoL->setInterpoleRate(0.1f);
    mJointAimInfoL->setBaseAimLocalDir(sead::Vector3f::ex);
    mJointAimInfoL->setBaseUpLocalDir(sead::Vector3f::ey);
    mJointAimInfoL->setBaseSideLocalDir(sead::Vector3f::ez);
    mJointAimInfoL->setLimitDegreeRect(43.0f, 43.0f, 25.0f, 20.0f);
    mJointAimInfoL->setEnableBackAim(true);
    al::initJointAimController(this, mJointAimInfoL, "EyeL");

    mJointAimInfoR = new al::JointAimInfo();
    mJointAimInfoR->setPowerRate(1.0f);
    mJointAimInfoR->setInterpoleRate(0.1f);
    mJointAimInfoR->setBaseAimLocalDir(-sead::Vector3f::ex);
    mJointAimInfoR->setBaseUpLocalDir(-sead::Vector3f::ey);
    mJointAimInfoR->setBaseSideLocalDir(-sead::Vector3f::ez);
    mJointAimInfoR->setLimitDegreeRect(43.0f, 43.0f, 25.0f, 20.0f);
    mJointAimInfoR->setEnableBackAim(true);
    al::initJointAimController(this, mJointAimInfoR, "EyeR");

    al::registActorToDemoInfo(this, info);
    makeActorAlive();
}

bool BossRaidNpc::receiveMsg(const al::SensorMsg* msg, al::HitSensor* other, al::HitSensor* self) {
    if (rs::isMsgPlayerDisregardTargetMarker(msg))
        return true;
    if (rs::isMsgNpcCapReactionAll(msg) && al::isSensorEnemyBody(self) && isEnableReaction()) {
        rs::requestHitReactionToAttacker(msg, self, other);
        al::setNerve(this, &Reaction);
        return true;
    }
    return false;
}

bool BossRaidNpc::isEnableReaction() const {
    if (al::isNerve(this, &Reaction))
        return al::isGreaterEqualStep(this, 20);
    return true;
}

void BossRaidNpc::exeEvent() {
    rs::updateEventFlow(mEventFlowExecutor);
    mJointAimInfoL->setTargetPos(rs::getPlayerBodyPos(this));
    mJointAimInfoR->setTargetPos(rs::getPlayerBodyPos(this));
}

void BossRaidNpc::exeReaction() {
    if (al::isFirstStep(this))
        al::startAction(this, "Reaction");
    mJointAimInfoL->setTargetPos(rs::getPlayerBodyPos(this));
    mJointAimInfoR->setTargetPos(rs::getPlayerBodyPos(this));
    if (al::isActionEnd(this)) {
        al::startAction(this, "Wait");
        al::setNerve(this, &Event);
    }
}
