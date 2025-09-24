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
    void* mPad;
    s32 _110;
    ElectricWireRailKeeper** mElectricWireRailKeepers;
    char mPad2[0x189];
    bool mIsElectricWireRadio;
    char mPad3[0x58];
};
