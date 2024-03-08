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
    char mUnkChar = '\0';
};
}  // namespace al
