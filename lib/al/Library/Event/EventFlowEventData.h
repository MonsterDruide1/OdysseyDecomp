#pragma once

namespace al {
class ByamlIter;
class EventFlowNodeInitInfo;

class EventFlowEventData {
public:
    EventFlowEventData(const char* name, const EventFlowNodeInitInfo& info);

private:
    const char* mName;
    ByamlIter* mParamIter = nullptr;
};

static_assert(sizeof(EventFlowEventData) == 0x10);

}  // namespace al
