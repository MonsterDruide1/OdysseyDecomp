#include "Enemy/SenobiGeneratePoint.h"

#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "Enemy/Senobi.h"

namespace {
NERVE_HOST_TYPE_IMPL(SenobiGeneratePoint, Standby)
NERVE_HOST_TYPE_IMPL(SenobiGeneratePoint, Wait)
NERVE_HOST_TYPE_IMPL(SenobiGeneratePoint, End)
NERVE_HOST_TYPE_IMPL(SenobiGeneratePoint, Generate)

NERVES_MAKE_NOSTRUCT(HostType, End, Generate)
NERVES_MAKE_STRUCT(HostType, Standby, Wait)
}  // namespace

void SenobiGeneratePoint::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    const al::Nerve* standby = &NrvHostType.Standby;
    const al::Nerve* wait = &NrvHostType.Wait;
    al::initNerve(this, al::isValidSwitchStart(this) ? standby : wait, 0);

    mSenobi = new Senobi("セノビー");
    mSenobi->setUseResetState(false);
    al::initCreateActorWithPlacementInfo(mSenobi, info);
    mSenobi->makeActorDead();
    makeActorAlive();
}

void SenobiGeneratePoint::forceKill() {
    if (al::isAlive(mSenobi))
        mSenobi->kill();
    al::setNerve(this, &End);
}

bool SenobiGeneratePoint::tryGenerate() {
    if (al::isAlive(mSenobi))
        return false;
    al::setNerve(this, &Generate);
    return true;
}

void SenobiGeneratePoint::exeStandby() {
    if (al::isOnSwitchStart(this))
        al::setNerve(this, &NrvHostType.Wait);
}

void SenobiGeneratePoint::exeWait() {
    if (!al::isAlive(mSenobi))
        al::setNerve(this, &Generate);
}

void SenobiGeneratePoint::exeEnd() {}

void SenobiGeneratePoint::exeGenerate() {
    if (al::isLessStep(this, 180))
        return;

    Senobi* senobi = mSenobi;
    const sead::Vector3f& trans = al::getTrans(this);
    const sead::Vector3f& rotate = al::getRotate(this);
    senobi->rebirth(trans, rotate);
    al::setNerve(this, &NrvHostType.Wait);
}
