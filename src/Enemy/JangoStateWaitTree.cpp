#include "Enemy/JangoStateWaitTree.h"

#include "Library/Collision/PartsConnectorUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "Util/PlayerUtil.h"

namespace {
NERVE_IMPL(JangoStateWaitTree, WaitOnSwitch)
NERVE_IMPL_(JangoStateWaitTree, EndFind, End)
NERVE_IMPL_(JangoStateWaitTree, EndDemo, End)
NERVE_IMPL(JangoStateWaitTree, WaitPlayerOnGround)

NERVES_MAKE_NOSTRUCT(JangoStateWaitTree, WaitOnSwitch, EndFind, EndDemo, WaitPlayerOnGround);
}  // namespace

JangoStateWaitTree::JangoStateWaitTree(al::LiveActor* actor, const al::ActorInitInfo& initInfo,
                                       JangoWaitTreeType waitTreeType)
    : al::ActorStateBase("木で待機", actor), mWaitTreeType(waitTreeType) {}

void JangoStateWaitTree::init() {
    mMtxConnector = al::createMtxConnector(mActor, mQuat);
    initNerve(&WaitOnSwitch, 0);
}

void JangoStateWaitTree::appear() {
    al::LiveActor* actor = mActor;

    al::NerveStateBase::appear();
    al::setNerve(this, &WaitOnSwitch);
    al::offCollide(actor);
    al::attachMtxConnectorToCollision(mMtxConnector, actor, 0.0f, 200.0f);
    mTrans = al::getTrans(actor);
    al::setVelocityZero(actor);
}

void JangoStateWaitTree::control() {
    al::LiveActor* actor = mActor;
    const sead::Vector3f& trans = al::getTrans(actor);
    const sead::Vector3f& playerPos = rs::getPlayerPos(actor);
    sead::Vector3f dist = playerPos - trans;

    if (!(dist.length() < 600.0f)) {
        al::connectPoseTrans(actor, mMtxConnector, mTrans);
        return;
    }

    al::setNerve(this, &EndFind);
    kill();
}

bool JangoStateWaitTree::isEndDemo() const {
    return al::isNerve(this, &EndDemo);
}

bool JangoStateWaitTree::isEndFind() const {
    return al::isNerve(this, &EndFind);
}

void JangoStateWaitTree::exeWaitOnSwitch() {
    al::LiveActor* actor = mActor;

    if (al::isFirstStep(this))
        al::startAction(actor, "WaitTree");

    if (!al::isOnSwitchStart(actor))
        return;

    s32 waitTreeType = mWaitTreeType;

    switch (waitTreeType) {
    case 0:
        al::setNerve(this, &WaitPlayerOnGround);
        return;
    case 1:
        break;
    default:
        return;
    }

    al::setNerve(this, &EndDemo);
    kill();
}

void JangoStateWaitTree::exeWaitPlayerOnGround() {
    al::LiveActor* actor = mActor;

    if (!rs::isPlayerCollidedGround(actor) || rs::isPlayerSafetyPointRecovery(actor)) {
        al::setNerve(this, &WaitPlayerOnGround);
        return;
    }

    if (al::isGreaterEqualStep(this, 2)) {
        al::onCollide(actor);
        al::setNerve(this, &EndDemo);
        kill();
    }
}

void JangoStateWaitTree::exeEnd() {}
