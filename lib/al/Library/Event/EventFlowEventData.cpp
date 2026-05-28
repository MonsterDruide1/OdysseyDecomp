#include "Library/Event/EventFlowEventData.h"

#include "Library/Event/EventFlowFunction.h"
#include "Library/Yaml/ByamlIter.h"

namespace al {

EventFlowEventData::EventFlowEventData(const char* name, const EventFlowNodeInitInfo& info)
    : mName(name) {
    mParamIter = new ByamlIter();
    tryGetParamIter(mParamIter, info);
}

}  // namespace al
