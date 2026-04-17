#include "MapObj/BlockStateNoItem.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/LiveActorFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Obj/PartsFunction.h"

#include "Util/SensorMsgFunction.h"

namespace {
NERVE_HOST_TYPE_IMPL(BlockStateNoItem, Wait);
NERVE_HOST_TYPE_IMPL(BlockStateNoItem, Reaction);

NERVES_MAKE_STRUCT(HostType, Wait, Reaction);
}  // namespace

BlockStateNoItem::BlockStateNoItem(al::LiveActor* actor)
    : al::ActorStateBase("アイテムなし状態", actor) {}

void BlockStateNoItem::init() {
    // NOTE: initializes storage for one sub-state, but never adds it
    initNerve(&NrvHostType.Wait, 1);
}

void BlockStateNoItem::appear() {
    al::ActorStateBase::appear();
    al::validateCollisionParts(mActor);
    al::startAction(mActor, "Wait");
    al::setNerve(this, &NrvHostType.Wait);
}

bool BlockStateNoItem::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                  al::HitSensor* self) {
    if (al::isNerve(this, &NrvHostType.Wait) && rs::isMsgBlockReactionAll(message)) {
        doReaction();
        return true;
    }
    return false;
}

void BlockStateNoItem::doReaction() {
    al::LiveActor* subActor = al::tryGetSubActor(mActor, "壊れモデル");
    if (subActor) {
        al::appearBreakModelRandomRotateY(subActor);
        al::startHitReaction(mActor, "破壊");
        al::hideModelIfShow(mActor);
        al::invalidateCollisionParts(mActor);
    }

    al::setNerve(this, &NrvHostType.Reaction);
}

bool BlockStateNoItem::isReaction() const {
    return al::isNerve(this, &NrvHostType.Reaction);
}

void BlockStateNoItem::exeWait() {}

void BlockStateNoItem::exeReaction() {
    if (!al::isExistAction(mActor, "Reaction")) {
        if (al::isGreaterEqualStep(this, 5))
            kill();
        return;
    }

    if (al::isFirstStep(this))
        al::startAction(mActor, "Reaction");

    if (al::isActionEnd(mActor))
        kill();
}
