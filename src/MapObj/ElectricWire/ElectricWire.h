#pragma once

#include "Library/LiveActor/LiveActor.h"

class ElectricWireRailKeeper;

class ElectricWire : public al::LiveActor {
public:
    ElectricWire(const char*);

    void tryUpdateDisplayModel();

    bool isElectricWireRadio() { return mIsElectricWireRadio; }

    void addDemoActorElectricWirePartsAll();

    s32 get_110() const { return _110; }

    ElectricWireRailKeeper** getElectricWireRailKeepers() { return mElectricWireRailKeepers; }

private:
    void* pad;
    s32 _110;
    char pad2[4];
    ElectricWireRailKeeper** mElectricWireRailKeepers;
    char pad3[0x1e0];
    bool mIsElectricWireRadio;
};
