#pragma once

#include <basis/seadTypes.h>

namespace al {
class ScreenPointTarget;

class ScreenPointCheckGroup {
public:
    ScreenPointCheckGroup(s32 size);
    void setValid(ScreenPointTarget* target);
    void setInvalid(ScreenPointTarget* target);
    ScreenPointTarget* getTarget(s32 index) const;
    void setTarget(ScreenPointTarget* target);

private:
    s32 mSize = 0;
    s32 mCount = 0;
    s32 mValidCount = 0;
    ScreenPointTarget** mScreenPointTargets = nullptr;
};

static_assert(sizeof(ScreenPointCheckGroup) == 0x18);
}  // namespace al
