#include "Player/PlayerInputFunction.h"

#include "Library/LiveActor/LiveActor.h"
#include "Library/Controller/InputFunction.h"

bool PlayerInputFunction::isTriggerAction(const al::LiveActor* actor, s32 port) {
    if (rs::isSeparatePlay(actor) && al::isPadTypeJoySingle(port)) {
        return true;
    }
    return al::isPadTriggerX(port) || al::isPadTriggerY(port);
}
