#include "Boss/BossForest/BossForestWander.h"

#include "Library/KeyPose/KeyPoseKeeper.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/SubActorKeeper.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Se/SeFunction.h"
#include "Library/Stage/StageSwitchUtil.h"

namespace {
NERVE_HOST_TYPE_IMPL(BossForestWander, WaitSwitchStart)
NERVE_HOST_TYPE_IMPL(BossForestWander, Wait)
NERVE_HOST_TYPE_IMPL(BossForestWander, Move)
NERVE_HOST_TYPE_IMPL(BossForestWander, End)

NERVES_MAKE_NOSTRUCT(HostType, WaitSwitchStart, Move)
NERVES_MAKE_STRUCT(HostType, Wait, End)
}  // namespace

BossForestWander::BossForestWander(const char* name) : LiveActor(name) {}

void BossForestWander::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "BossForest", "Wander");
    al::initNerve(this, &WaitSwitchStart, 0);

    al::tryGetQuatPtr(this);

    mKeyPoseKeeper = al::createKeyPoseKeeper(info);
    mKeyPoseKeeper->setMoveTypeStop();
    al::setKeyMoveClippingInfo(this, &mClippingRadius, mKeyPoseKeeper);

    al::startActionSubActor(this, "ライフパーツ00", "WaitOn");
    al::startActionSubActor(this, "ライフパーツ01", "WaitOn");
    al::startActionSubActor(this, "ライフパーツ02", "WaitOn");

    al::trySyncStageSwitchAppear(this);
}

void BossForestWander::exeWaitSwitchStart() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
    if (!al::isValidSwitchStart(this) || al::isOnSwitchStart(this))
        al::setNerve(this, &NrvHostType.Wait);
}

void BossForestWander::exeWait() {
    if (al::isFirstStep(this)) {
        s32 waitTime = al::calcKeyMoveWaitTime(mKeyPoseKeeper);
        if (waitTime >= 0)
            mWaitTime = waitTime;
    }
    if (al::isGreaterEqualStep(this, mWaitTime)) {
        if (!al::checkIsPlayingSe(this, "PgSenario2StartMove", nullptr))
            al::tryStartSe(this, "PgSenario2StartMove");
        al::setNerve(this, &Move);
    }
}

void BossForestWander::exeMove() {
    if (al::isFirstStep(this))
        mMoveTime = al::calcKeyMoveMoveTime(mKeyPoseKeeper);

    f32 rate = al::calcNerveRate(this, mMoveTime);
    al::calcLerpKeyTrans(al::getTransPtr(this), mKeyPoseKeeper, rate);
    al::calcSlerpKeyQuat(al::getQuatPtr(this), mKeyPoseKeeper, rate);

    if (al::isGreaterEqualStep(this, mMoveTime)) {
        al::nextKeyPose(mKeyPoseKeeper);
        if (al::isStop(mKeyPoseKeeper))
            al::setNerve(this, &NrvHostType.End);
        else
            al::setNerve(this, &NrvHostType.Wait);
    }
}

void BossForestWander::exeEnd() {
    kill();
}
