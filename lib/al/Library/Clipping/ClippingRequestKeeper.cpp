#include "Library/Clipping/ClippingRequestKeeper.h"

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/LiveActor.h"

namespace al {

ClippingRequestInfo::ClippingRequestInfo() = default;

ClippingRequestTable::ClippingRequestTable(s32 capacity) : tableCapacity(capacity) {
    tableInfo = new ClippingRequestInfo[capacity];
}

ClippingRequestKeeper::ClippingRequestKeeper(s32 capacity) {
    mRequestTable = new ClippingRequestTable(capacity);
}

void ClippingRequestKeeper::request(LiveActor* actor, ClippingRequestType clippingRequestType) {
    mRequestTable->tableSize++;
    mRequestTable->tableInfo[mRequestTable->tableSize - 1] = {actor, clippingRequestType};
}

void ClippingRequestKeeper::executeRequest() {
    for (s32 i = 0; i < mRequestTable->tableSize; i++) {
        const ClippingRequestInfo& clippingRequestInfo = mRequestTable->tableInfo[i];
        if (isDead(mRequestTable->tableInfo[i].liveActor))
            continue;

        switch (clippingRequestInfo.requestType) {
        case ClippingRequestType::isClipped:
            if (!isClipped(clippingRequestInfo.liveActor))
                clippingRequestInfo.liveActor->startClipped();
            break;
        case ClippingRequestType::isNotClipped:
            if (isClipped(clippingRequestInfo.liveActor))
                clippingRequestInfo.liveActor->endClipped();
            break;
        }
    }
    mRequestTable->tableSize = 0;
}
}  //  namespace al
