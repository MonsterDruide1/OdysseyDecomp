#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class PlacementInfo;
}  // namespace al

class TalkMessageInfoMoon : public al::LiveActor {
public:
    TalkMessageInfoMoon(const char* displayName);

    void initPlacement(const al::ActorInitInfo& info, const al::PlacementInfo& placementInfo);
    void update();
    bool isRequest() const;
    void accepted();

    const char* getLabelName() const { return mLabelName; }

    s32 getPriority() const { return mPriority; }

private:
    const char* mLabelName = nullptr;
    s32 mPriority = -1;
    s32 _114 = -1;
    u16 _118 = 0;
    u8 _11a[6] = {};
    void* _120 = nullptr;
    void* _128 = nullptr;
};

static_assert(sizeof(TalkMessageInfoMoon) == 0x130);
