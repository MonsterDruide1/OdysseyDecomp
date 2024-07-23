#include "Library/MapObj/ChildStep.h"

#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Placement/PlacementFunction.h"

namespace {
using namespace al;

NERVE_IMPL(ChildStep, Wait)

NERVES_MAKE_STRUCT(ChildStep, Wait)
}  // namespace

namespace al {
ChildStep::ChildStep(const char* name, LiveActor* parent) : LiveActor(name), mParent(parent) {}

void ChildStep::init(const ActorInitInfo& info) {
    initMapPartsActor(this, info, nullptr);
    initNerve(this, &NrvChildStep.Wait, 0);

    multVecInvQuat(&mPos, mParent, getTrans(this));

    makeActorAlive();
}

bool ChildStep::receiveMsg(const SensorMsg* message, HitSensor* source, HitSensor* target) {
    return mParent->receiveMsg(message, source, target);
}

void ChildStep::exeWait() {
    multVecPose(getTransPtr(this), mParent, mPos);
}

s32 calcChildStepCount(const ActorInitInfo& info) {
    return calcLinkChildNum(info, "ChildStep");
}
}  // namespace al
