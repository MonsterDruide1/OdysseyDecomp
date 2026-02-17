#include "Item/ItemAppearanceController.h"

#include "Library/Item/ItemUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/ItemUtil.h"

namespace {
NERVE_HOST_TYPE_IMPL(ItemAppearanceController, Wait)
NERVE_HOST_TYPE_IMPL(ItemAppearanceController, Appear)
NERVE_HOST_TYPE_IMPL(ItemAppearanceController, Count)
NERVE_HOST_TYPE_IMPL(ItemAppearanceController, Done)

NERVES_MAKE_STRUCT(HostType, Wait, Count)
NERVES_MAKE_NOSTRUCT(HostType, Appear, Done)
}  // namespace

ItemAppearanceController::ItemAppearanceController(al::LiveActor* actor, s32 appearTime)
    : al::NerveExecutor("アイテム出現制御"), mActor(actor), mAppearTime(appearTime) {
    initNerve(&NrvHostType.Wait, 0);
}

bool ItemAppearanceController::requestAppear(const al::SensorMsg* message,
                                             const al::HitSensor* hitSensor) {
    if (al::isNerve(this, &NrvHostType.Wait)) {
        rs::setAppearItemFactorAndOffsetByMsg(mActor, message, hitSensor);
        al::setNerve(this, &NrvHostType.Count);
        return true;
    }
    return false;
}

void ItemAppearanceController::exeWait() {}

void ItemAppearanceController::exeCount() {
    if (al::isGreaterEqualStep(this, mAppearTime))
        al::setNerve(this, &Appear);
}

void ItemAppearanceController::exeAppear() {
    al::appearItem(mActor);
    al::setNerve(this, &Done);
}

void ItemAppearanceController::exeDone() {}
