#include "Layout/AimingCursor.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(AimingCursor, Appear);
NERVE_IMPL(AimingCursor, End);
NERVE_IMPL(AimingCursor, Wait);

NERVES_MAKE_NOSTRUCT(AimingCursor, Appear, End, Wait);
}  // namespace

AimingCursor::AimingCursor(const char* name, const al::LayoutInitInfo& info)
    : al::LayoutActor(name) {
    al::initLayoutActor(this, info, "Aiming");
    initNerve(&Appear);
    kill();
}

void AimingCursor::startAppear() {
    appear();
    al::setNerve(this, &Appear);
}

void AimingCursor::end() {
    al::setNerve(this, &End);
}

void AimingCursor::setTrans(const sead::Vector2f& pos) {
    sead::Vector2f t = al::getLocalTrans(this);
    al::setLocalTrans(this, (t + pos) * 0.5f);
}

void AimingCursor::setScale(f32 scale) {
    al::setLocalScale(this, (al::getLocalScale(this) + scale) * 0.5f);
}

bool AimingCursor::tryLookOn() {
    if (mIsLookOn)
        return false;
    mIsLookOn = true;
    al::startAction(this, "On", "State");
    return true;
}

bool AimingCursor::tryLookOff() {
    if (!mIsLookOn)
        return false;
    mIsLookOn = false;
    al::startAction(this, "Off", "State");
    return true;
}

void AimingCursor::exeAppear() {
    if (al::isFirstStep(this))
        al::startAction(this, "Appear");
    if (al::isActionEnd(this))
        al::setNerve(this, &Wait);
}

void AimingCursor::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
}

void AimingCursor::exeEnd() {
    if (al::isFirstStep(this))
        al::startAction(this, "End");
    if (al::isActionEnd(this))
        kill();
}
