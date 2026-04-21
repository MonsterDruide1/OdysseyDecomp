#pragma once

#include "Library/Yaml/ByamlIter.h"

namespace al {
class EventFlowNodeInitInfo;

class EventFlowEventData {
public:
    EventFlowEventData(const char* name, const EventFlowNodeInitInfo& info);

    const char* getName() const { return mName; }

    const ByamlIter& getByamlIter() const { return *mByamlIter; }

private:
    const char* mName = nullptr;
    const ByamlIter* mByamlIter = nullptr;
    void* _10 = nullptr;
};

static_assert(sizeof(EventFlowEventData) == 0x18);
}  // namespace al
