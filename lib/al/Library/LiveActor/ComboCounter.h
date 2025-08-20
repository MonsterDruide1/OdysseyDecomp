#pragma once

#include <basis/seadTypes.h>

namespace al {
class ComboCounter {
public:
    ComboCounter() = default;

    virtual void increment() { mCount++; }

    s32 getCount() const { return mCount; }

private:
    s32 mCount = 0;
};
}  // namespace al
