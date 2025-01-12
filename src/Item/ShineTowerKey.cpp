#include "Item/ShineTowerKey.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/Nerve/NerveSetupUtil.h"

#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"
#include "Util/Sensor.h"

namespace {
NERVE_IMPL(ShineTowerKey, Wait)
NERVES_MAKE_NOSTRUCT(ShineTowerKey, Wait)
}  // namespace

ShineTowerKey::ShineTowerKey(const char* name) : al::LiveActor(name) {}

void ShineTowerKey::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "ShineTowerKey", nullptr);
    al::initNerve(this, &Wait, 0);
    makeActorAlive();
}

bool ShineTowerKey::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                               al::HitSensor* self) {
    if (rs::isMsgItemGetAll(message)) {
        GameDataFunction::addKey(this, 1);
        al::startHitReaction(this, "取得");
        kill();
        return true;
    }
    return false;
}

void ShineTowerKey::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
}
