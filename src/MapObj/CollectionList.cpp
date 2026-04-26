#include "MapObj/CollectionList.h"

#include <math/seadVector.h>

#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/NpcEventFlowUtil.h"
#include "Util/PlayerPuppetFunction.h"

namespace {
NERVE_IMPL(CollectionList, Wait);
NERVE_IMPL(CollectionList, Open);
NERVE_IMPL(CollectionList, AfterClose);

NERVES_MAKE_NOSTRUCT(CollectionList, Wait, Open, AfterClose);
}  // namespace

const sead::Vector3f sCollectionListRequestIconOffset = {0.0f, 100.0f, 0.0f};

CollectionList::CollectionList(const char* name) : al::LiveActor(name) {}

void CollectionList::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    al::initNerve(this, &Wait, 0);
    makeActorAlive();
}

bool CollectionList::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                al::HitSensor* self) {
    if (al::isMsgPlayerDisregard(message))
        return true;

    if (al::isSensorPlayer(other) && al::isNerve(this, &Wait)) {
        if (al::isMsgBindStart(message) &&
            rs::checkTriggerDecideWithRequestIcon(this, sCollectionListRequestIconOffset, -1.0f))
            return true;

        if (al::isMsgBindInit(message)) {
            mPlayerPuppet = rs::startPuppet(self, other);
            al::setNerve(this, &Open);
            return true;
        }
    }

    if (al::isMsgBindCancel(message)) {
        al::setNerve(this, &Wait);
        mPlayerPuppet = nullptr;
        return true;
    }

    return false;
}

bool CollectionList::isOpen() const {
    return al::isNerve(this, &Open);
}

void CollectionList::close() {
    if (isOpen())
        al::setNerve(this, &AfterClose);
}

void CollectionList::exeWait() {}

void CollectionList::exeOpen() {
    if (al::isFirstStep(this)) {
    }
}

void CollectionList::exeAfterClose() {
    if (al::isStep(this, 10))
        rs::endBindAndPuppetNull(&mPlayerPuppet);

    if (al::isGreaterEqualStep(this, 30))
        al::setNerve(this, &Wait);
}
