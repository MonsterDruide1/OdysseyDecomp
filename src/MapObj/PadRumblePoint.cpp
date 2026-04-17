#include "MapObj/PadRumblePoint.h"

#include "Library/Controller/PadRumbleFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Placement/PlacementFunction.h"

namespace {
NERVE_ACTION_IMPL(PadRumblePoint, Wait)

NERVE_ACTIONS_MAKE_STRUCT(PadRumblePoint, Wait)
}  // namespace

PadRumblePoint::PadRumblePoint(const char* actorName) : al::LiveActor(actorName) {}

void PadRumblePoint::init(const al::ActorInitInfo& info) {
    al::initNerveAction(this, "Wait", &NrvPadRumblePoint.collector, 0);
    al::initActor(this, info);
    al::tryGetStringArg(&mRumbleName, info, "RumbleName");
    al::tryGetArg(&mRumbleNear, info, "RumbleNear");
    al::tryGetArg(&mRumbleFar, info, "RumbleFar");
    al::trySyncStageSwitchAppearAndKill(this);
}

void PadRumblePoint::exeWait() {
    if (alPadRumbleFunction::checkIsAlivePadRumbleLoop(this, mRumbleName, al::getTransPtr(this),
                                                       -1)) {
        return;
    }

    alPadRumbleFunction::startPadRumbleLoop(this, mRumbleName, al::getTransPtr(this), mRumbleNear,
                                            mRumbleFar, -1);
}
