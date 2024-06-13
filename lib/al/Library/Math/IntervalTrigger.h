#pragma once

#include <basis/seadTypes.h>

namespace al {
class IntervalTrigger {
public:
    IntervalTrigger(f32);
    void update(f32);
};
}  // namespace al
