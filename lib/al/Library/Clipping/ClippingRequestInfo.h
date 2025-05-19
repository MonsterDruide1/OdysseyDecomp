#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;

enum class ClippingRequestType : s32 { isClipped, isNotClipped };

struct ClippingRequestInfo {
    ClippingRequestInfo();

    ClippingRequestInfo(LiveActor* actor, ClippingRequestType type) {
        liveActor = actor;
        requestType = type;
    }

    LiveActor* liveActor = nullptr;
    ClippingRequestType requestType = ClippingRequestType::isClipped;
};

}  // namespace al
