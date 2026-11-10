#include "Util/NpcAnimUtil.h"

#include "Npc/CapWorldNpcTailJointController.h"

namespace rs {

void initCapWorldNpcTail(al::LiveActor* capWorldNpc) {
    initCapWorldNpcTailJointController(capWorldNpc);
}

}  // namespace rs
