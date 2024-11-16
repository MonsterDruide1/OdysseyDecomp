#pragma once
#include "Library/LiveActor/LiveActor.h"

class ElectricWire;

class ElectricWireRailKeeper : public al::LiveActor {
public:
    void appear() override;
    void appearBySwitch();
    void exeStandby();
    void exeWait();
private:
    ElectricWire *mElectricWire;
};
