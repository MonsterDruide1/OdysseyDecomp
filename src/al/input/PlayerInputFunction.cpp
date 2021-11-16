#include "al/input/PlayerInputFunction.h"

#include "al/LiveActor/LiveActor.h"
#include "al/input/rsSeparatePlay.h"
#include "al/util/InputUtil.h"

bool PlayerInputFunction::isTriggerAction(const al::LiveActor* actor, int port) {
    if (rs::isSeparatePlay(actor) && al::isPadTypeJoySingle(port)) {
        return true;
    }
    return al::isPadTriggerX(port) || al::isPadTriggerY(port);
}
