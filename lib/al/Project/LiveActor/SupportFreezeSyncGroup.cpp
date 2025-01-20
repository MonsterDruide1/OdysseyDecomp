#include "Project/LiveActor/SupportFreezeSyncGroup.h"

#include "Library/HitSensor/HitSensorKeeper.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementId.h"

namespace al {
SupportFreezeSyncGroup::SupportFreezeSyncGroup() : mGroupId(new PlacementId()) {}

void SupportFreezeSyncGroup::init(const ActorInitInfo& info) {
    alPlacementFunction::getLinkGroupId(mGroupId, info, "SupportFreezeSyncGroup");

    mActors = new LiveActor*[mMaxActorCount];
    for (s32 i = 0; i < mMaxActorCount; i++)
        mActors[i] = nullptr;
}

void SupportFreezeSyncGroup::regist(LiveActor* actor) {
    u64 uVar1 = *(u64*)&mActorCount;
    if (mActorCount >= (s32)(uVar1 >> 32))  // check for integer overflow
        return;

    mActors[mActorCount] = actor;
    mActorCount++;
}

void SupportFreezeSyncGroup::setHostSensor(HitSensor* hostSensor) {
    mHostSensor = hostSensor;
}

bool SupportFreezeSyncGroup::isEqualGroupId(const ActorInitInfo& info) const {
    if (!mGroupId->isValid())
        return false;

    PlacementId groupId;
    if (!alPlacementFunction::getLinkGroupId(&groupId, info, "SupportFreezeSyncGroup"))
        return false;

    return mGroupId->isEqual(groupId);
}

void SupportFreezeSyncGroup::movement() {
    bool bVar3 = false;
    for (s32 i = 0; i < mActorCount; i++)
        bVar3 |= sendMsgIsNerveSupportFreeze(mActors[i]->getHitSensorKeeper()->getSensor(0),
                                             mHostSensor);

    for (s32 i = 0; i < mActorCount; i++)
        if (bVar3)
            sendMsgOffSyncSupportFreeze(mActors[i]->getHitSensorKeeper()->getSensor(0),
                                        mHostSensor);
        else
            sendMsgOnSyncSupportFreeze(mActors[i]->getHitSensorKeeper()->getSensor(0), mHostSensor);
}
}  // namespace al
