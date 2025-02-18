#include "Library/MapObj/ChildStep.h"

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/SubActorFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Placement/PlacementFunction.h"

namespace al {
namespace {
NERVE_IMPL(ChildStep, Wait)

NERVES_MAKE_STRUCT(ChildStep, Wait)
}  // namespace

ChildStep::ChildStep(const char* name, LiveActor* parent) : LiveActor(name), mParent(parent) {}

void ChildStep::init(const ActorInitInfo& info) {
    initMapPartsActor(this, info, nullptr);
    initNerve(this, &NrvChildStep.Wait, 0);

    multVecInvQuat(&mPos, mParent, getTrans(this));

    makeActorAlive();
}

bool ChildStep::receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) {
    return mParent->receiveMsg(message, other, self);
}

void ChildStep::exeWait() {
    multVecPose(getTransPtr(this), mParent, mPos);
}

s32 calcChildStepCount(const ActorInitInfo& info) {
    return calcLinkChildNum(info, "ChildStep");
}

void tryInitSubActorKeeperChildStep(LiveActor* actor, const ActorInitInfo& info) {
    s32 childStepCount = calcChildStepCount(info);

    if (childStepCount <= 0)
        return;

    initSubActorKeeperNoFile(actor, info, childStepCount);
}

void createChildStep(const ActorInitInfo& info, LiveActor* parent, bool isSyncClipping) {
    s32 childStepCount = calcChildStepCount(info);

    for (s32 i = 0; i < childStepCount; i++) {
        ChildStep* childStep = new ChildStep("子供足場", parent);

        initLinksActor(childStep, info, "ChildStep", i);

        if (isSyncClipping) {
            invalidateClipping(childStep);
            registerSubActorSyncClipping(parent, childStep);
        }
    }
}
}  // namespace al
