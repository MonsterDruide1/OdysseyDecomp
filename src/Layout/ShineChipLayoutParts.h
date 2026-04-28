#pragma once

#include <basis/seadTypes.h>

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
class Nerve;
}  // namespace al

class ShineChipLayoutParts : public al::LayoutActor {
public:
    ShineChipLayoutParts(const al::LayoutInitInfo& info, const char* layoutName);

    void appear() override;

    bool isInArea() const;
    bool tryUpdateCount(s32 currentCount, s32 prevCount);
    bool isEndCompleteAnim() const;

    void exeAppear();
    void exeWait();
    void exeAdd();
    void exeAddNoAnim();
    void exeComplete();
    void exeEnd();

private:
    s32 mCurrentCount = -1;
    s32 mPrevCount = -1;
    const al::Nerve* mNextNerve = nullptr;
};

static_assert(sizeof(ShineChipLayoutParts) == 0x140);

namespace rs {
bool tryUpdateShineChipLayoutCount(ShineChipLayoutParts* parts);
}  // namespace rs
