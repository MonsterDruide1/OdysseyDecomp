#include "MapObj/VolleyballNet.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(VolleyballNet, Wait);
NERVE_IMPL(VolleyballNet, Reaction);

NERVES_MAKE_NOSTRUCT(VolleyballNet, Wait, Reaction);
}  // namespace

VolleyballNet::VolleyballNet(const char* name) : al::LiveActor(name) {}

void VolleyballNet::init(const al::ActorInitInfo& initInfo) {
    al::initActor(this, initInfo);
    al::initNerve(this, &Wait, 0);
    makeActorAlive();
}

bool VolleyballNet::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                               al::HitSensor* self) {
    if (rs::isMsgCapAttackCollide(message) || al::isMsgPlayerTouch(message) ||
        rs::isMsgSeedTouch(message)) {
        bool isTouchingPlayerOrCap =
            (rs::isMsgCapAttackCollide(message) || al::isMsgPlayerTouch(message));

        if (al::isNerve(this, &Reaction) && mReactionFrames <= 5) {
            mReactionFrames = 0;
            return isTouchingPlayerOrCap;
        }

        mReactionFrames = 0;
        al::setNerve(this, &Reaction);
        return isTouchingPlayerOrCap;
    }
    return false;
}

void VolleyballNet::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
}

void VolleyballNet::exeReaction() {
    if (al::isFirstStep(this))
        al::startAction(this, "Reaction");

    if (al::isActionEnd(this) && mReactionFrames > 5) {
        al::setNerve(this, &Wait);
        return;
    }
    mReactionFrames++;
}
