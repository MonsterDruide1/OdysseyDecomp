#pragma once

#include <basis/seadTypes.h>

namespace al {
enum class ClippingRequestType;
class LiveActor;
struct ClippingRequestInfo;

struct ClippingRequestTable {
    inline ClippingRequestTable(s32 capacity);
    s32 tableCapacity;
    s32 tableSize;
    ClippingRequestInfo* tableInfo;
};

class ClippingRequestKeeper {
public:
    ClippingRequestKeeper(s32 capacity);
    void executeRequest();
    void request(LiveActor* actor, ClippingRequestType clippingRequestType);

private:
    ClippingRequestTable* mRequestTable;
};

}  // namespace al
