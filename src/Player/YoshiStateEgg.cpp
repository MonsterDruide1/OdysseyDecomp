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
#include "Util/Hack.h"
#include "Util/ObjUtil.h"
#include "Util/PlayerCollisionUtil.h"

namespace {
void updateEggMove(al::LiveActor* actor, const IUsePlayerCollision* collision,
                   const PlayerConst* playerConst);

NERVE_IMPL(YoshiStateEgg, Wait);
NERVE_IMPL(YoshiStateEgg, Appear);

NERVES_MAKE_NOSTRUCT(YoshiStateEgg, Wait, Appear);
}  // namespace

YoshiStateEgg::YoshiStateEgg(const al::ActorInitInfo& info, al::LiveActor* player,
                             const IUsePlayerCollision* collision, const PlayerConst* playerConst,
                             const al::WaterSurfaceFinder* waterSurfaceFinder,
                             PlayerModelChangerYoshi* modelChanger)
    : al::ActorStateBase("卵", player), mCollision(collision), mConst(playerConst),
      mWaterSurfaceFinder(waterSurfaceFinder), mModelChanger(modelChanger) {
    mEgg = new YoshiEgg(player, collision);
    mEgg->init(info);
    initNerve(&Wait, 0);
}

void YoshiStateEgg::appear() {
    al::ActorStateBase::appear();
    mEgg->kill();
    al::invalidateHitSensors(mActor);

    if (mIsFirstPlacement) {
        mIsFirstPlacement = false;
        mEgg->initPlacementEgg();
        al::setNerve(this, &Wait);
    } else {
        al::setNerve(this, &Appear);
    }
}

void YoshiStateEgg::kill() {
    al::ActorStateBase::kill();
    al::validateHitSensors(mActor);
}

void YoshiStateEgg::exeAppear() {
    if (al::isFirstStep(this))
        mEgg->appearEgg();

    updateEggMove(mActor, mCollision, mConst);

    if (mEgg->isEndAppear())
        al::setNerve(this, &Wait);
}

namespace {
void updateEggMove(al::LiveActor* actor, const IUsePlayerCollision* collision,
                   const PlayerConst* playerConst) {
    if (rs::isCollidedGround(collision)) {
        rs::waitGround(actor, collision, playerConst->getGravity(), playerConst->getFallSpeedMax(),
                       playerConst->getSlerpQuatGrav(), 0.0f);
        return;
    }

    const sead::Vector3f& gravity = al::getGravity(actor);
    f32 gravityValue = playerConst->getGravity();
    sead::Vector3f velocity = {gravityValue * gravity.x, gravityValue * gravity.y,
                               gravityValue * gravity.z};
    al::tryAddVelocityLimit(actor, velocity, playerConst->getFallSpeedMax());
}
}  // namespace

void YoshiStateEgg::exeWait() {
    if (al::isFirstStep(this))
        al::validateClipping(mActor);

    updateEggMove(mActor, mCollision, mConst);

    if (mEgg->isBreak()) {
        mModelChanger->appearModel();
        kill();
    }
}

bool YoshiStateEgg::reactionCollidedCollisionCode() {
    if (isDead())
        return false;

    al::LiveActor* player = mActor;
    if (al::isInDeathArea(player) ||
        (al::isNerve(this, &Wait) &&
         (rs::isTouchHackCancelCollisionCode(player, mCollision) || al::isInWater(player)))) {
        al::startHitReaction(player, "[ヨッシー]死亡");
        return true;
    }

    return false;
}
