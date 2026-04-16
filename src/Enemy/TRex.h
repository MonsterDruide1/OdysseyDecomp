#pragma once

#include "Library/LiveActor/LiveActor.h"

class TRex : public al::LiveActor {
public:
    bool isEnableBreakPartsForceScroll() const;

    bool isForceScrollEndFallStart() const { return mIsForceScrollEndFallStart; }

private:
    u8 _108[0x224];
    bool mIsForceScrollEndFallStart = false;  // TODO: verify
};
