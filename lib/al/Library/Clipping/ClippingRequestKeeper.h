#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
struct ClippingRequestInfo;

enum class ClippingRequestType : s32 { IsClipped, IsNotClipped };

struct ClippingRequestInfo {
    ClippingRequestInfo();

    ClippingRequestInfo(LiveActor* actor, ClippingRequestType type)
        : liveActor(actor), requestType(type) {}

    LiveActor* liveActor = nullptr;
    ClippingRequestType requestType = ClippingRequestType::IsClipped;
};

struct ClippingRequestTable {
    ClippingRequestTable(s32 capacity);
    s32 capacity = 0;
    s32 numEntries = 0;
    ClippingRequestInfo* entries = nullptr;
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
