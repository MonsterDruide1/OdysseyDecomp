#include "Library/Area/TrafficArea.h"

namespace al {
TrafficArea::TrafficArea(const char* name) : AreaObj(name) {}

bool TrafficArea::tryPermitEnterCar() {
    if (mCurState.isNpc || mPrevState.isNpc)
        return false;

    mCurState.isCar = true;

    return true;
}

bool TrafficArea::tryPermitEnterNpc() {
    if (mCurState.isCar || mPrevState.isCar)
        return false;

    mCurState.isNpc = true;

    return true;
}
}  // namespace al
