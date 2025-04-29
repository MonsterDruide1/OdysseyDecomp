#include "Library/Area/TrafficArea.h"

namespace al {
TrafficArea::TrafficArea(const char* name) : AreaObj(name) {}

bool TrafficArea::tryPermitEnterCar() {
    if (mIsNpcFull || mIsCarUnavailable)
        return false;

    mIsCarFull = true;

    return true;
}

bool TrafficArea::tryPermitEnterNpc() {
    if (mIsCarFull || mIsNpcUnavailable)
        return false;

    mIsNpcFull = true;

    return true;
}
}  // namespace al
