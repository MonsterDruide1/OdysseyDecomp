#include "Util/PlayerUtil.h"

#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Player/PlayerUtil.h"

const sead::Vector3f& rs::getPlayerPos(const al::LiveActor* actor) {
    const al::LiveActor* player = al::getPlayerActor(actor, 0);
    return al::getTrans(player);
}
