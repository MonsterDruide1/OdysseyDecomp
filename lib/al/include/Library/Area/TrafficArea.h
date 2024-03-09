#pragma once

#include "Library/Area/AreaObj.h"

namespace al {
class TrafficArea : public AreaObj {
public:
    TrafficArea(const char* name);

    bool tryPermitEnterCar();
    bool tryPermitEnterNpc();

private:
    bool mCarFull = false;
    bool mNpcFull = false;
    bool mNpcUnavailable = false;
    bool mCarUnavailable = false;
};
}  // namespace al
