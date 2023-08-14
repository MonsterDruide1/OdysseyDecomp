#include "Player/PlayerInputFunction.h"

#include "Library/Controller/InputFunction.h"
#include "Library/LiveActor/LiveActor.h"

bool PlayerInputFunction::isTriggerAction(const al::LiveActor* actor, s32 port) {
    if (rs::isSeparatePlay(actor) && al::isPadTypeJoySingle(port)) {
        return true;
    }
    return al::isPadTriggerX(port) || al::isPadTriggerY(port);
}
