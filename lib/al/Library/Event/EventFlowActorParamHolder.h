#pragma once

#include <basis/seadTypes.h>

namespace al {
class ByamlIter;

struct EventFlowActorParam {
    const char* actionName = nullptr;
    f32 moveSpeed = 2.0f;
    f32 turnSpeedDegree = 3.5f;
};

class EventFlowActorParamHolder {
public:
    EventFlowActorParamHolder();

    void load(const ByamlIter&);
    const EventFlowActorParam* findSuffixParam(const char*) const;

    const EventFlowActorParam* getDefaultParam() const { return mDefaultParam; }

private:
    EventFlowActorParam* mDefaultParam = nullptr;
    EventFlowActorParam* mSubActorParams = nullptr;
    s32 mSubActorParamCount = 0;
};

static_assert(sizeof(EventFlowActorParam) == 0x10);
static_assert(sizeof(EventFlowActorParamHolder) == 0x18);
}  // namespace al
