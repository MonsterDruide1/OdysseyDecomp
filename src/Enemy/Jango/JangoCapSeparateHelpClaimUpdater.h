#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
}  // namespace al

class JangoCapSeparateHelpClaimUpdater {
public:
    JangoCapSeparateHelpClaimUpdater(al::LiveActor* actor);

    void update();

private:
    s32 mHelpReactionCoolTime = 0;
    al::LiveActor* mActor = nullptr;
    s32 mSinglePlayReactionTimer = 0;
};

static_assert(sizeof(JangoCapSeparateHelpClaimUpdater) == 0x18);
