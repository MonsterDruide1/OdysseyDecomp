#pragma once

#include "Library/LiveActor/LiveActor.h"
#include "Library/Scene/ISceneObj.h"

namespace al {
class SupportFreezeSyncGroup;

class SupportFreezeSyncGroupHolder : public LiveActor, public ISceneObj {
public:
    SupportFreezeSyncGroupHolder();

    void initAfterPlacementSceneObj(const ActorInitInfo& info) override;
    void movement() override;

    void regist(LiveActor* actor, const ActorInitInfo& info);
    SupportFreezeSyncGroup* tryFindGroup(const ActorInitInfo& info);

private:
    SupportFreezeSyncGroup** mSupportFreezeSyncGroups;
    s32 mSupportFreezeSyncGroupCount = 0;
    s32 mSupportFreezeSyncGroupMaxCount = 64;
};

static_assert(sizeof(SupportFreezeSyncGroupHolder) == 0x120);
}  // namespace al
