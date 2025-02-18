#include "Amiibo/ItemAmiiboKoopa.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/LiveActor/LiveActorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(ItemAmiiboKoopa, Wait);
NERVE_IMPL(ItemAmiiboKoopa, Expand);

NERVES_MAKE_STRUCT(ItemAmiiboKoopa, Wait, Expand);
}  // namespace

ItemAmiiboKoopa::ItemAmiiboKoopa(const char* actorName) : al::LiveActor(actorName) {}

void ItemAmiiboKoopa::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "ItemAmiiboKoopa", nullptr);
    al::initNerve(this, &NrvItemAmiiboKoopa.Wait, 0);
    makeActorDead();
}

void ItemAmiiboKoopa::appear() {
    al::LiveActor::appear();
    al::setNerve(this, &NrvItemAmiiboKoopa.Wait);
    al::startHitReaction(this, "出現");
}

void ItemAmiiboKoopa::exeExpand() {
    al::setSensorRadius(this, al::getNerveStep(this) / 30.0f * 1000.0f);
    al::setTrans(this, rs::getPlayerPos(this));
    if (al::isGreaterEqualStep(this, 30))
        al::setNerve(this, &NrvItemAmiiboKoopa.Wait);
}

void ItemAmiiboKoopa::exeWait() {
    if (al::isGreaterEqualStep(this, 300)) {
        kill();
        return;
    }

    al::setTrans(this, rs::getPlayerPos(this));
}

void ItemAmiiboKoopa::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    rs::sendMsgItemAmiiboKoopa(other, self);
}
