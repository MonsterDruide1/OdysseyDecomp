#include "Library/MapObj/SupportFreezeSyncGroupHolder.h"

#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Project/LiveActor/SupportFreezeSyncGroup.h"

namespace al {
SupportFreezeSyncGroupHolder::SupportFreezeSyncGroupHolder() : LiveActor("DRC拘束グループ監視") {
    mSupportFreezeSyncGroups = new SupportFreezeSyncGroup*[mSupportFreezeSyncGroupMaxCount];

    for (s32 i = 0; i < mSupportFreezeSyncGroupMaxCount; i++)
        mSupportFreezeSyncGroups[i] = nullptr;
}

void SupportFreezeSyncGroupHolder::initAfterPlacementSceneObj(const ActorInitInfo& info) {
    initActorSceneInfo(this, info);
    initExecutorUpdate(this, info, "DRCアシスト同期グループ");
    initActorPoseTRSV(this);

    initHitSensor(1);

    HitSensor* bodySensor = addHitSensorMapObj(this, info, "Body", 0.0f, 0, {0.0f, 0.0f, 0.0f});
    for (s32 i = 0; i < mSupportFreezeSyncGroupCount; i++)
        mSupportFreezeSyncGroups[i]->setHostSensor(bodySensor);

    makeActorAlive();
}

void SupportFreezeSyncGroupHolder::movement() {
    for (s32 i = 0; i < mSupportFreezeSyncGroupCount; i++)
        mSupportFreezeSyncGroups[i]->movement();
}

void SupportFreezeSyncGroupHolder::regist(LiveActor* actor, const ActorInitInfo& info) {
    SupportFreezeSyncGroup* supportFreezeSyncGroup = tryFindGroup(info);
    if (supportFreezeSyncGroup == nullptr) {
        supportFreezeSyncGroup = new SupportFreezeSyncGroup();
        supportFreezeSyncGroup->init(info);

        mSupportFreezeSyncGroups[mSupportFreezeSyncGroupCount] = supportFreezeSyncGroup;
        mSupportFreezeSyncGroupCount++;
    }

    supportFreezeSyncGroup->regist(actor);
}

SupportFreezeSyncGroup* SupportFreezeSyncGroupHolder::tryFindGroup(const ActorInitInfo& info) {
    for (s32 i = 0; i < mSupportFreezeSyncGroupCount; i++)
        if (mSupportFreezeSyncGroups[i]->isEqualGroupId(info))
            return mSupportFreezeSyncGroups[i];

    return nullptr;
}
}  // namespace al
