#include "Enemy/EnemyStateReviveInsideScreen.h"

#include "Library/Action/ActorActionKeeper.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Se/SeFunction.h"

namespace {
NERVE_IMPL(EnemyStateReviveInsideScreen, Hide);
NERVE_IMPL(EnemyStateReviveInsideScreen, Delay);
NERVE_IMPL(EnemyStateReviveInsideScreen, AppearSign);

NERVES_MAKE_NOSTRUCT(EnemyStateReviveInsideScreen, AppearSign);
NERVES_MAKE_STRUCT(EnemyStateReviveInsideScreen, Hide, Delay);
}  // namespace

EnemyStateReviveInsideScreen::EnemyStateReviveInsideScreen(al::LiveActor* actor)
    : al::ActorStateBase("強制復活状態", actor) {
    initNerve(&NrvEnemyStateReviveInsideScreen.Hide, 0);
    al::calcQuat(&mReviveQuat, actor);
    mRevivePos.set(al::getTrans(actor));
}

void EnemyStateReviveInsideScreen::appear() {
    setDead(false);

    if (al::isHideModel(mActor)) {
        mIsModelHidden = true;
    } else {
        al::hideModel(mActor);
        mIsModelHidden = false;
    }

    if (al::isInvalidClipping(mActor)) {
        mIsInvalidClipping = true;
    } else {
        al::invalidateClipping(mActor);
        mIsInvalidClipping = false;
    }

    al::setVelocityZero(mActor);
    mActor->getActorActionKeeper()->tryStartActionNoAnim("ReviveInsideScreen");
    if (mIsHidden)
        al::setNerve(this, &NrvEnemyStateReviveInsideScreen.Hide);
    else
        al::setNerve(this, &NrvEnemyStateReviveInsideScreen.Delay);
}

void EnemyStateReviveInsideScreen::kill() {
    al::stopSe(mActor, "RevivalSign", 0xFFFFFFFF, 0);
    setDead(true);
    if (!mIsModelHidden)
        al::showModel(mActor);
    if (!mIsInvalidClipping)
        al::validateClipping(mActor);
}

void EnemyStateReviveInsideScreen::startRevive() {
    al::setNerve(this, &AppearSign);
}

bool EnemyStateReviveInsideScreen::isDisappear() const {
    return al::isNerve(this, &NrvEnemyStateReviveInsideScreen.Hide) ||
           al::isNerve(this, &NrvEnemyStateReviveInsideScreen.Delay);
}

void EnemyStateReviveInsideScreen::exeHide() {}

void EnemyStateReviveInsideScreen::exeDelay() {
    al::setNerveAtGreaterEqualStep(this, &AppearSign, 150);
}

void EnemyStateReviveInsideScreen::exeAppearSign() {
    if (al::isFirstStep(this)) {
        al::resetQuatPosition(mActor, mReviveQuat, mRevivePos);
        al::startAction(mActor, "AppearSign");
    }
    if (al::isGreaterEqualStep(this, 150)) {
        al::startHitReaction(mActor, "出現");
        kill();
    }
}
