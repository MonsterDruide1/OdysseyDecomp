#include "ElectricWireRailKeeper.h"

#include "ElectricWire.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/LiveActorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"

namespace {
NERVE_IMPL(ElectricWireRailKeeper, Standby)
NERVE_IMPL(ElectricWireRailKeeper, Wait)

NERVES_MAKE_NOSTRUCT(ElectricWireRailKeeper, Standby, Wait)
}

void ElectricWireRailKeeper::appear() {
    LiveActor::appear();
    this->mElectricWire->tryUpdateDisplayModel();
    al::setNerve(this, &Wait);
}

void ElectricWireRailKeeper::appearBySwitch() {
    if (al::isAlive(this))
        return;

    this->appear();
    al::invalidateClipping(this);
    al::setNerve(this, &Standby);
}

void ElectricWireRailKeeper::exeStandby() {
    if (al::isGreaterStep(this, 10)) {
        al::validateClipping(this);
        al::setNerve(this, &Wait);
    }
}

void ElectricWireRailKeeper::exeWait() {}