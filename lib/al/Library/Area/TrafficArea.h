#pragma once

#include "Library/Area/AreaObj.h"

namespace al {
class TrafficArea : public AreaObj {
public:
    TrafficArea(const char* name);

    bool tryPermitEnterCar();
    bool tryPermitEnterNpc();

private:
    bool mIsCarFull = false;
    bool mIsNpcFull = false;
    bool mIsNpcUnavailable = false;
    bool mIsCarUnavailable = false;
};
}  // namespace al
