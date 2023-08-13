#include "game/Player/PlayerInputFunction.h"

#include "al/Library/LiveActor/LiveActor.h"
#include "al/Library/Controller/InputFunction.h"

bool PlayerInputFunction::isTriggerAction(const al::LiveActor* actor, int port) {
    if (rs::isSeparatePlay(actor) && al::isPadTypeJoySingle(port)) {
        return true;
    }
    return al::isPadTriggerX(port) || al::isPadTriggerY(port);
}
