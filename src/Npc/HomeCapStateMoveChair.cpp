#include "Npc/HomeCapStateMoveChair.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Npc/HomeCap.h"
#include "Util/PlayerUtil.h"

namespace {
NERVE_IMPL(HomeCapStateMoveChair, Move);

NERVES_MAKE_STRUCT(HomeCapStateMoveChair, Move);
}  // namespace

HomeCapStateMoveChair::HomeCapStateMoveChair(al::LiveActor* actor)
    : al::ActorStateBase("椅子に移動", actor), mParabolicPath(new al::ParabolicPath()) {}

void HomeCapStateMoveChair::init() {
    initNerve(&NrvHomeCapStateMoveChair.Move, 0);
}

void HomeCapStateMoveChair::appear() {
    al::NerveStateBase::appear();
}

void HomeCapStateMoveChair::appearMoveOtherChair(al::LiveActor* actor) {
    mTargetActor = actor;
    al::LiveActor* actorBase = mActor;
    al::resetPosition(actorBase,
                      rs::getPlayerPos(actorBase) + sead::Vector3f(0.0f, 60.0f, 0.0f));
    sead::Vector3f frontDir = sead::Vector3f::zero;
    sead::Vector3f upDir = sead::Vector3f::zero;
    rs::calcPlayerFrontDir(&frontDir, actorBase);
    rs::calcPlayerUpDir(&upDir, actorBase);
    sead::Quatf quat = sead::Quatf::unit;
    al::makeQuatFrontUp(&quat, frontDir, upDir);
    al::setQuat(actorBase, quat);
    al::setNerve(this, &NrvHomeCapStateMoveChair.Move);
}

void HomeCapStateMoveChair::getCapAppearPos(sead::Vector3f* out) {
    *out = rs::getPlayerPos(mActor) + sead::Vector3f(0.0f, 60.0f, 0.0f);
}

void HomeCapStateMoveChair::exeMove() {
    al::LiveActor* actorBase = mActor;
    al::ParabolicPath* path = mParabolicPath;
    if (al::isFirstStep(this)) {
        sead::Vector3f offset = *HomeCapFunction::getAimChairOffset();
        sead::Quatf quat = sead::Quatf::unit;
        al::calcQuat(&quat, mTargetActor);
        offset.rotate(quat);
        path->initFromUpVectorAddHeight(
            al::getTrans(actorBase), al::getTrans(mTargetActor) + offset, sead::Vector3f::ey,
            50.0f);
        al::startAction(actorBase, "HomeCapManHeroAppear");
    }
    sead::Vector3f* pos = al::getTransPtr(actorBase);
    f32 rate = al::calcNerveRate(this, 40);
    path->calcPosition(pos, rate);
    sead::Vector3f frontDir = sead::Vector3f::zero;
    al::calcFrontDir(&frontDir, mTargetActor);
    al::turnToDirection(actorBase, frontDir, 2.5f);
    if (al::isGreaterEqualStep(this, 40)) {
        sead::Quatf quat = sead::Quatf::unit;
        al::makeQuatFrontUp(&quat, frontDir, sead::Vector3f::ey);
        al::setQuat(actorBase, quat);
        update();
    }
}
