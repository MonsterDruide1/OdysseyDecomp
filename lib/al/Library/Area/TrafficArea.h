#pragma once

#include "Library/Area/AreaObj.h"

namespace al {
struct TrafficAreaState {
    bool isCar = false;
    bool isNpc = false;
};

class TrafficArea : public AreaObj {
public:
    TrafficArea(const char* name);

    bool tryPermitEnterCar();
    bool tryPermitEnterNpc();

    void reset() {
        mPrevState = mCurState;
        mCurState = {};
    }

private:
    TrafficAreaState mCurState;
    TrafficAreaState mPrevState;
};
}  // namespace al
