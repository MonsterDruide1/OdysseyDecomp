#include "Library/Clipping/ClippingRequestKeeper.h"

#include "Library/Clipping/ClippingRequestInfo.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/LiveActor.h"

namespace al {

ClippingRequestTable::ClippingRequestTable(s32 capacity) {
    tableCapacity = capacity;
    tableSize = 0;
    tableInfo = nullptr;
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
        const ClippingRequestInfo* clippingRequestInfo = mRequestTable->tableInfo;
        if (isDead(mRequestTable->tableInfo[i].liveActor))
            continue;

        switch (clippingRequestInfo[i].requestType) {
        case ClippingRequestType::isClipped:
            if (!isClipped(clippingRequestInfo[i].liveActor))
                clippingRequestInfo[i].liveActor->startClipped();
            break;
        case ClippingRequestType::isNotClipped:
            if (isClipped(clippingRequestInfo[i].liveActor))
                clippingRequestInfo[i].liveActor->endClipped();
            break;
        }
    }
    mRequestTable->tableSize = 0;
}
}  //  namespace al
