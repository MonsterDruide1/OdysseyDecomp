#include "Boss/GolemClimb/GolemClimbFootPrint.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"

GolemClimbFootPrint::GolemClimbFootPrint() : al::LiveActor("ゴーレムのぼり足跡") {}

void GolemClimbFootPrint::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "GolemClimbFootPrint", nullptr);
}

void GolemClimbFootPrint::appear() {
    al::startAction(this, "Disappear");
    al::LiveActor::makeActorAlive();
}

void GolemClimbFootPrint::control() {
    if (al::isActionEnd(this))
        kill();
}
