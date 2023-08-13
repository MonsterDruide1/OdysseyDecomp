#include "game/MapObj/CapTargetParts.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"

CapTargetParts::CapTargetParts(al::LiveActor* actor, const al::ActorInitInfo& initInfo)
    : al::PartsModel("帽子の的パーツ") {
    initPartsFixFileNoRegister(actor, initInfo, "CapTarget", "Parts", nullptr);
}

void CapTargetParts::startHack() {
    al::startAction(this, "Normal");
}

void CapTargetParts::startNormal() {
    al::startAction(this, "Normal");
    al::showModelIfHide(this);
}

void CapTargetParts::startSwoon() {
    al::startAction(this, "Swoon");
    al::hideModel(this);
}
