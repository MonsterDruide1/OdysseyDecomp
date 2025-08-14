#pragma once

#include "Library/Area/AreaObj.h"

namespace al {
class TrafficArea : public AreaObj {
public:
    TrafficArea(const char* name);

    bool tryPermitEnterCar();
    bool tryPermitEnterNpc();

    void reset() {
        *(u16*)&mIsNpcUnavailable = *(u16*)&mIsCarFull;
        mIsCarFull = false;
        mIsNpcFull = false;
    }

private:
    bool mIsCarFull = false;
    bool mIsNpcFull = false;
    bool mIsNpcUnavailable = false;
    bool mIsCarUnavailable = false;
};
}  // namespace al
