#pragma once

#include <basis/seadTypes.h>

namespace al {
class EventFlowExecutor;

class EventFlowExecutorHolder {
public:
    EventFlowExecutorHolder(s32 capacity);

    void registerExecutor(EventFlowExecutor* executor);
    void initAfterPlacement();

private:
    s32 mCapacity;
    s32 mCount;
    EventFlowExecutor** mExecutors;
};

static_assert(sizeof(EventFlowExecutorHolder) == 0x10);
}  // namespace al
