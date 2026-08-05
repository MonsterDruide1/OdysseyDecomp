#include "MapObj/EchoEmitter.h"

#include <cmath>

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(EchoEmitter, Wait)
NERVE_IMPL(EchoEmitter, Keep)
NERVE_IMPL(EchoEmitter, Stop)
NERVES_MAKE_NOSTRUCT(EchoEmitter, Wait, Keep, Stop)
}  // namespace

EchoEmitter::EchoEmitter(const char* name) : al::LiveActor(name) {}

void EchoEmitter::init(const al::ActorInitInfo& info) {
    al::initActorSceneInfo(this, info);
    al::initExecutorUpdate(this, info, "敵[Movement]");
    al::initActorPoseTQSV(this);
    al::initActorClipping(this, info);
    al::invalidateClipping(this);
    al::initNerve(this, &Wait, 0);
    makeActorDead();
}

void EchoEmitter::start(const sead::Vector3f& trans, f32 radius, s32 life) {
    al::resetPosition(this, trans);
    al::setNerve(this, &Wait);

    f32 halfRadius = radius * 0.5f;

    mAlpha = 1.0f;
    mLife = life;
    mOffsetStart = 0.0f;
    mOffsetEnd = 0.0f;
    mRadiusStart = halfRadius;
    mRadiusEnd = radius + radius;
    mExpansionRate = 0.0f;
    mRadius = halfRadius;
    appear();
}

void EchoEmitter::startKeep(const sead::Vector3f& trans, f32 radius, s32 life) {
    al::resetPosition(this, trans);
    al::setNerve(this, &Keep);

    mAlpha = 1.0f;
    mLife = life;
    mExpansionRate = 0.0f;
    mKeepRadius = fmaxf(radius, 0.0f);
    appear();
}

void EchoEmitter::exeWait() {
    mExpansionRate = al::calcNerveEaseOutValue(this, mLife / 3, mOffsetStart, mOffsetEnd);
    mRadius = al::calcNerveEaseOutValue(this, mLife / 3, mRadiusStart, mRadiusEnd);
    mAlpha = al::calcNerveValue(this, mLife / 2, mLife, 1.0f, 0.0f);

    if (al::isGreaterEqualStep(this, mLife)) {
        al::setNerve(this, &Stop);
        mRadius = 10.0f;
        kill();
    }
}

void EchoEmitter::exeKeep() {
    mAlpha = al::calcNerveEaseInOutValue(this, mLife, 1.0f, 0.0f);
    mRadius = al::lerpValue(mRadius, mKeepRadius, 0.1f);

    if (al::isGreaterEqualStep(this, mLife)) {
        al::setNerve(this, &Stop);
        mRadius = 10.0f;
        kill();
    }
}

void EchoEmitter::exeStop() {
    mExpansionRate = 0.0f;
    mRadius = 10.0f;
    mAlpha = 0.0f;
}

s32 EchoEmitter::getLife() const {
    if (!al::isNerve(this, &Wait))
        return 0;

    return mLife - al::getNerveStep(this);
}
