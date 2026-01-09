#include "Amiibo/ItemAmiiboKoopa.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(ItemAmiiboKoopa, Expand);
NERVE_IMPL(ItemAmiiboKoopa, Wait);

NERVES_MAKE_NOSTRUCT(ItemAmiiboKoopa, Expand, Wait);
}  // namespace

ItemAmiiboKoopa::ItemAmiiboKoopa(const char* actorName) : al::LiveActor(actorName) {}

void ItemAmiiboKoopa::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "ItemAmiiboKoopa", nullptr);
    al::initNerve(this, &Expand, 0);
    makeActorDead();
}

void ItemAmiiboKoopa::appear() {
    al::LiveActor::appear();
    al::setNerve(this, &Expand);
    al::startHitReaction(this, "出現");
}

void ItemAmiiboKoopa::exeExpand() {
    al::setSensorRadius(this, al::getNerveStep(this) / 30.0f * 1000.0f);
    al::setTrans(this, rs::getPlayerPos(this));
    if (al::isGreaterEqualStep(this, 30))
        al::setNerve(this, &Wait);
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
