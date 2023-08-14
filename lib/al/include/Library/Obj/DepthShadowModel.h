#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {

class DepthShadowModel : public LiveActor {
public:
    DepthShadowModel(LiveActor*, const ActorInitInfo&, const char*, bool);
    DepthShadowModel(LiveActor*, const ActorInitInfo&, const char*, const char*, bool);
    void initAfterPlacement() override;

private:
    void* field_108;
    s32 field_110;
    bool field_114;
};
static_assert(sizeof(DepthShadowModel) == 0x118, "DepthShadowModel Size");

}  // namespace al
