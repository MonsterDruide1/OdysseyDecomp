#include "Project/LiveActor/SupportFreezeSyncGroup.h"

#include "Library/HitSensor/HitSensorKeeper.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Placement/PlacementFunction.h"

namespace al {
SupportFreezeSyncGroup::SupportFreezeSyncGroup() {}

void SupportFreezeSyncGroup::init(const ActorInitInfo& info) {
    alPlacementFunction::getLinkGroupId(mSupportFreezeSyncGroupId, info, "SupportFreezeSyncGroup");

    mActors = new LiveActor*[mMaxActorCount];
    for (s32 i = 0; i < mMaxActorCount; i++)
        mActors[i] = nullptr;
}

void SupportFreezeSyncGroup::regist(LiveActor* actor) {
    if (mActorCount >= mMaxActorCount)
        return;

    mActors[mActorCount] = actor;
    mActorCount++;
}

void SupportFreezeSyncGroup::setHostSensor(HitSensor* hostSensor) {
    mHostSensor = hostSensor;
}

bool SupportFreezeSyncGroup::isEqualGroupId(const ActorInitInfo& info) const {
    if (!mSupportFreezeSyncGroupId->isValid())
        return false;

    PlacementId groupId;
    if (!alPlacementFunction::getLinkGroupId(&groupId, info, "SupportFreezeSyncGroup"))
        return false;

    return mSupportFreezeSyncGroupId->isEqual(groupId);
}

void SupportFreezeSyncGroup::movement() {
    bool isAnyNerveSupportFreeze = false;
    for (s32 i = 0; i < mActorCount; i++)
        isAnyNerveSupportFreeze |= sendMsgIsNerveSupportFreeze(
            mActors[i]->getHitSensorKeeper()->getSensor(0), mHostSensor);

    for (s32 i = 0; i < mActorCount; i++)
        if (isAnyNerveSupportFreeze)
            sendMsgOnSyncSupportFreeze(mActors[i]->getHitSensorKeeper()->getSensor(0), mHostSensor);
        else
            sendMsgOffSyncSupportFreeze(mActors[i]->getHitSensorKeeper()->getSensor(0),
                                        mHostSensor);
}
}  // namespace al
