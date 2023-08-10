#include "game/MapObj/CapTargetParts.h"

#include "al/Library/LiveActor/ActorActionFunction.h"
#include "al/Library/LiveActor/ActorModelFunction.h"

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
