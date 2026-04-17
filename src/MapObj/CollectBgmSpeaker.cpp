#include "MapObj/CollectBgmSpeaker.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Enemy/DisregardReceiver.h"
#include "Util/NpcEventFlowUtil.h"

namespace {
const sead::Vector3f sRequestIconOffset = {0.0f, 0.0f, 0.0f};

NERVE_IMPL(CollectBgmSpeaker, Wait);
NERVE_IMPL(CollectBgmSpeaker, Open);
NERVE_IMPL(CollectBgmSpeaker, Close);
NERVES_MAKE_NOSTRUCT(CollectBgmSpeaker, Wait, Open, Close);
}  // namespace

CollectBgmSpeaker::CollectBgmSpeaker(const char* name) : al::LiveActor(name) {}

void CollectBgmSpeaker::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "CollectBgmSpeaker", nullptr);
    al::initNerve(this, &Wait, 0);
    mDisregardReceiver = new DisregardReceiver(this, nullptr);
    al::startAction(this, "Wait");
    makeActorAlive();
}

void CollectBgmSpeaker::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isSensorEye(self) && al::isSensorPlayer(other) &&
        rs::checkTriggerDecideWithRequestIcon(this, sRequestIconOffset, -1.0f))
        al::setNerve(this, &Open);
}

bool CollectBgmSpeaker::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                   al::HitSensor* self) {
    return mDisregardReceiver->receiveMsg(message, other, self);
}

void CollectBgmSpeaker::exeWait() {}

void CollectBgmSpeaker::exeOpen() {
    if (al::isFirstStep(this))
        al::invalidateClipping(this);
}

void CollectBgmSpeaker::exeClose() {
    if (al::isGreaterEqualStep(this, 30)) {
        al::validateClipping(this);
        al::setNerve(this, &Wait);
    }
}

void CollectBgmSpeaker::exePlayBgm() {}

bool CollectBgmSpeaker::isOpen() const {
    return al::isNerve(this, &Open);
}

void CollectBgmSpeaker::close() {
    al::setNerve(this, &Close);
}
