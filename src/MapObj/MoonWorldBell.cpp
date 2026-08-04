#include "MapObj/MoonWorldBell.h"

#include "Library/Demo/DemoFunction.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "System/GameDataFunction.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(MoonWorldBell, Wait)
NERVE_IMPL(MoonWorldBell, CapReaction)
NERVE_IMPL(MoonWorldBell, HipDropReaction)

NERVES_MAKE_NOSTRUCT(MoonWorldBell, Wait, CapReaction, HipDropReaction)
}  // namespace

MoonWorldBell::MoonWorldBell(const char* name) : al::LiveActor(name) {}

void MoonWorldBell::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "MoonWorldHomeBell", nullptr);
    al::initNerve(this, &Wait, 0);
    al::invalidateClipping(this);
    al::registActorToDemoInfo(this, info);

    if (GameDataFunction::isGameClear(GameDataHolderAccessor(this)))
        al::startAction(this, "WaitNoSound");

    makeActorAlive();
}

bool MoonWorldBell::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                               al::HitSensor* self) {
    if (rs::isMsgCapAttackCollide(message) && !al::isNerve(this, &CapReaction)) {
        al::startHitReaction(this, "帽子ヒット");
        al::setNerve(this, &CapReaction);
        return true;
    }

    if (rs::isMsgPlayerAndCapHipDropAll(message) && !al::isNerve(this, &HipDropReaction)) {
        al::startHitReaction(this, "ヒップドロップ");
        al::setNerve(this, &HipDropReaction);
        return true;
    }

    if (al::isMsgPlayerUpperPunch(message)) {
        al::startHitReaction(this, "アッパーパンチ");
        return true;
    }

    return false;
}

void MoonWorldBell::exeWait() {}

void MoonWorldBell::exeCapReaction() {
    if (al::isGreaterEqualStep(this, 60))
        al::setNerve(this, &Wait);
}

void MoonWorldBell::exeHipDropReaction() {
    if (al::isGreaterEqualStep(this, 60))
        al::setNerve(this, &Wait);
}
