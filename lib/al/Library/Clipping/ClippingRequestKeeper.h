#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
struct ClippingRequestInfo;

enum class ClippingRequestType : s32 { isClipped, isNotClipped };

struct ClippingRequestInfo {
    ClippingRequestInfo();

    ClippingRequestInfo(LiveActor* actor, ClippingRequestType type)
        : liveActor(actor), requestType(type) {}

    LiveActor* liveActor = nullptr;
    ClippingRequestType requestType = ClippingRequestType::isClipped;
};

struct ClippingRequestTable {
    ClippingRequestTable(s32 capacity);
    s32 tableCapacity = 0;
    s32 tableSize = 0;
    ClippingRequestInfo* tableInfo = nullptr;
};

class ClippingRequestKeeper {
public:
    ClippingRequestKeeper(s32 capacity);
    void executeRequest();
    void request(LiveActor* actor, ClippingRequestType requestType);

private:
    ClippingRequestTable* mRequestTable;
};

}  // namespace al
