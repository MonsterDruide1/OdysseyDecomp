#include "Library/Clipping/ClippingRequestKeeper.h"

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/LiveActor.h"

namespace al {

ClippingRequestInfo::ClippingRequestInfo() = default;

ClippingRequestTable::ClippingRequestTable(s32 capacity) : capacity(capacity) {
    entries = new ClippingRequestInfo[capacity];
}

ClippingRequestKeeper::ClippingRequestKeeper(s32 capacity) {
    mRequestTable = new ClippingRequestTable(capacity);
}

void ClippingRequestKeeper::request(LiveActor* actor, ClippingRequestType clippingRequestType) {
    mRequestTable->numEntries++;
    mRequestTable->entries[mRequestTable->numEntries - 1] = {actor, clippingRequestType};
}

void ClippingRequestKeeper::executeRequest() {
    for (s32 i = 0; i < mRequestTable->numEntries; i++) {
        const ClippingRequestInfo& clippingRequestInfo = mRequestTable->entries[i];
        if (isDead(mRequestTable->entries[i].liveActor))
            continue;

        switch (clippingRequestInfo.requestType) {
        case ClippingRequestType::IsClipped:
            if (!isClipped(clippingRequestInfo.liveActor))
                clippingRequestInfo.liveActor->startClipped();
            break;
        case ClippingRequestType::IsNotClipped:
            if (isClipped(clippingRequestInfo.liveActor))
                clippingRequestInfo.liveActor->endClipped();
            break;
        }
    }
    mRequestTable->numEntries = 0;
}
}  //  namespace al
