#include "Player/YoshiStateEgg.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nature/NatureUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Player/PlayerConst.h"
#include "Player/PlayerModelChangerYoshi.h"
#include "Player/YoshiEgg.h"
#include "Util/ObjUtil.h"
#include "Util/PlayerCollisionUtil.h"
#include "Util/PlayerHackFunction.h"

namespace {
NERVE_IMPL(YoshiStateEgg, Appear)
NERVE_IMPL(YoshiStateEgg, Wait)

NERVES_MAKE_STRUCT(YoshiStateEgg, Wait, Appear)
}  // namespace

YoshiStateEgg::YoshiStateEgg(const al::ActorInitInfo& info, al::LiveActor* actor,
                             const IUsePlayerCollision* playerCollision,
                             const PlayerConst* playerConst,
                             const al::WaterSurfaceFinder* waterSurfaceFinder,
                             PlayerModelChangerYoshi* playerModelChanger)
    : al::ActorStateBase("卵", actor), mPlayerCollision(playerCollision), mPlayerConst(playerConst),
      mWaterSurfaceFinder(waterSurfaceFinder), mPlayerModelChangerYoshi(playerModelChanger) {
    mYoshiEgg = new YoshiEgg(actor, playerCollision);
    mYoshiEgg->init(info);
    initNerve(&NrvYoshiStateEgg.Wait, 0);
}

void YoshiStateEgg::appear() {
    al::ActorStateBase::appear();
    mYoshiEgg->kill();
    al::invalidateHitSensors(mActor);
    if (_48) {
        _48 = false;
        mYoshiEgg->initPlacementEgg();
        al::setNerve(this, &NrvYoshiStateEgg.Wait);
        return;
    }

    al::setNerve(this, &NrvYoshiStateEgg.Appear);
}

void YoshiStateEgg::kill() {
    al::ActorStateBase::kill();
    al::validateHitSensors(mActor);
}

bool YoshiStateEgg::reactionCollidedCollisionCode() {
    if (isDead())
        return false;

    al::LiveActor* actor = mActor;

    if (al::isInDeathArea(actor)) {
        al::startHitReaction(actor, "[ヨッシー]死亡");
        return true;
    }

    if (al::isNerve(this, &NrvYoshiStateEgg.Wait) &&
        (rs::isTouchHackCancelCollisionCode(actor, mPlayerCollision) || al::isInWater(actor))) {
        al::startHitReaction(actor, "[ヨッシー]死亡");
        return true;
    }

    return false;
}

static void updateFallVelocity(al::LiveActor* actor, const IUsePlayerCollision* playerCollision,
                               const PlayerConst* playerConst) {
    if (rs::isCollidedGround(playerCollision)) {
        rs::waitGround(actor, playerCollision, playerConst->getGravity(),
                       playerConst->getFallSpeedMax(), playerConst->getSlerpQuatGrav(), 0.0f);
        return;
    }

    al::tryAddVelocityLimit(actor, al::getGravity(actor) * playerConst->getGravity(),
                            playerConst->getFallSpeedMax());
}

void YoshiStateEgg::exeAppear() {
    if (al::isFirstStep(this))
        mYoshiEgg->appearEgg();

    updateFallVelocity(mActor, mPlayerCollision, mPlayerConst);

    if (mYoshiEgg->isEndAppear()) {
        al::setNerve(this, &NrvYoshiStateEgg.Wait);
        return;
    }
}

void YoshiStateEgg::exeWait() {
    if (al::isFirstStep(this))
        al::validateClipping(mActor);

    updateFallVelocity(mActor, mPlayerCollision, mPlayerConst);
    if (mYoshiEgg->isBreak()) {
        mPlayerModelChangerYoshi->appearModel();
        kill();
    }
}
