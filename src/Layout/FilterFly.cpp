#include "Layout/FilterFly.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathLengthUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(FilterFly, Wait);
NERVE_IMPL(FilterFly, WaitEnd);
NERVE_IMPL(FilterFly, Move);

NERVE_MAKE(FilterFly, Wait);
NERVE_MAKE(FilterFly, WaitEnd);
NERVE_MAKE(FilterFly, Move);
}  // namespace

const sead::Vector2f localTrans = {600.0f, -300.0f};

FilterFly::FilterFly(const char* name, const al::LayoutInitInfo& info, const char* suffix)
    : al::LayoutActor(name) {
    al::initLayoutActor(this, info, "FilterFly", suffix);
    initNerve(&Wait, 0);
    al::setPaneLocalTrans(this, "RootPane", localTrans);
    kill();
}

void FilterFly::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait", 0);
    if (al::isGreaterStep(this, 180))
        al::setNerve(this, &WaitEnd);
}

void FilterFly::exeWaitEnd() {}

void FilterFly::exeMove() {
    if (al::isFirstStep(this))
        al::startAction(this, "Loop", 0);

    sead::Vector2f currentPos = {al::getLocalTrans(this).x, al::getLocalTrans(this).y};
    sead::Vector2f direction = mTargetPos - currentPos;

    f32 len = direction.length();
    if (!(len > 15.0f)) {
        al::setNerve(this, &Wait);
        return;
    }

    if (al::tryNormalizeOrZero(&direction)) {
        mTargetPos2 += direction * 0.9f;
        currentPos += mTargetPos2;
        al::setPaneLocalTrans(this, "RootPane", currentPos);

        f32 angleY = al::calcAngleDegree(direction, sead::Vector2f::ey);

        al::setPaneLocalRotate(
            this, "RootPane",
            {0, 0, al::calcAngleDegree(direction, sead::Vector2f::ex) < 90.0f ? -angleY : angleY});
    }
    mTargetPos2 *= 0.98f;
}

void FilterFly::move(const sead::Vector2f& target) {
    mTargetPos.set(target);
    if (!al::isNerve(this, &Move))
        return al::setNerve(this, &Move);
}

bool FilterFly::isWaitEnd() const {
    return al::isNerve(this, &WaitEnd);
}
