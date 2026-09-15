#include "MapObj/StatueSnapMark.h"

#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Enemy/DisregardReceiver.h"
#include "Util/ItemUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_HOST_TYPE_IMPL(StatueSnapMark, Wait);
NERVE_HOST_TYPE_IMPL(StatueSnapMark, AppearShine);
NERVE_HOST_TYPE_IMPL(StatueSnapMark, Done);

NERVES_MAKE_STRUCT(HostType, Wait, AppearShine);
NERVES_MAKE_NOSTRUCT(HostType, Done);
}  // namespace

StatueSnapMark::StatueSnapMark(const char* name) : al::LiveActor(name) {}

void StatueSnapMark::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    al::initNerve(this, &NrvHostType.Wait, 0);
    mShineActor = rs::initLinkShine(info, "ShineActor", 0);
    mDisregardReceiver = new DisregardReceiver(this, nullptr);
    makeActorAlive();
}

bool StatueSnapMark::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                al::HitSensor* self) {
    if (mDisregardReceiver->receiveMsg(message, other, self))
        return true;

    if (rs::isMsgStatueSnap(message) && al::isNerve(this, &NrvHostType.Wait)) {
        mSnapTarget = al::getSensorHost(other);
        al::setNerve(this, &NrvHostType.AppearShine);
        return true;
    }
    return false;
}

void StatueSnapMark::exeWait() {}

void StatueSnapMark::exeAppearShine() {
    if (al::isFirstStep(this))
        rs::appearPopupShine(mShineActor);

    if (rs::isEndAppearShine(mShineActor))
        al::setNerve(this, &Done);
}

void StatueSnapMark::exeDone() {
    if (al::isFirstStep(this) && mSnapTarget) {
        al::HitSensor* self = al::getHitSensor(this, nullptr);
        rs::sendMsgCancelHack(al::getHitSensor(mSnapTarget, "Body"), self);
    }
}
