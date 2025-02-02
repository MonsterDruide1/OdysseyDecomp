#include "Library/Nerve/NerveUtil.h"

#include <math/seadMathCalcCommon.h>

#include "Library/Math/MathUtil.h"
#include "Library/Nerve/IUseNerve.h"
#include "Library/Nerve/NerveAction.h"
#include "Library/Nerve/NerveActionCtrl.h"
#include "Library/Nerve/NerveKeeper.h"
#include "Library/Nerve/NerveStateCtrl.h"

namespace al {
void setNerve(IUseNerve* user, const Nerve* nerve) {
    user->getNerveKeeper()->setNerve(nerve);
}

void setNerveAtStep(IUseNerve* user, const Nerve* nerve, s32 step) {
    if (user->getNerveKeeper()->getCurrentStep() == step)
        user->getNerveKeeper()->setNerve(nerve);
}

bool isStep(const IUseNerve* user, s32 step) {
    return user->getNerveKeeper()->getCurrentStep() == step;
}

void setNerveAtGreaterEqualStep(IUseNerve* user, const Nerve* nerve, s32 step) {
    if (user->getNerveKeeper()->getCurrentStep() >= step)
        user->getNerveKeeper()->setNerve(nerve);
}

bool isNerve(const IUseNerve* user, const Nerve* nerve) {
    return user->getNerveKeeper()->getCurrentNerve() == nerve;
}

s32 getNerveStep(const IUseNerve* user) {
    return user->getNerveKeeper()->getCurrentStep();
}

const Nerve* getCurrentNerve(const IUseNerve* user) {
    return user->getNerveKeeper()->getCurrentNerve();
}

bool isFirstStep(const IUseNerve* user) {
    return isStep(user, 0);
}

bool isGreaterStep(const IUseNerve* user, s32 step) {
    return user->getNerveKeeper()->getCurrentStep() > step;
}

bool isGreaterEqualStep(const IUseNerve* user, s32 step) {
    return user->getNerveKeeper()->getCurrentStep() >= step;
}

bool isLessStep(const IUseNerve* user, s32 step) {
    return user->getNerveKeeper()->getCurrentStep() < step;
}

bool isLessEqualStep(const IUseNerve* user, s32 step) {
    return user->getNerveKeeper()->getCurrentStep() <= step;
}

bool isInRangeStep(const IUseNerve* user, s32 startStep, s32 endStep) {
    NerveKeeper* nerveKeeper = user->getNerveKeeper();
    return startStep <= nerveKeeper->getCurrentStep() && nerveKeeper->getCurrentStep() <= endStep;
}

bool isIntervalStep(const IUseNerve* user, s32 interval, s32 offset) {
    s32 currentStep = user->getNerveKeeper()->getCurrentStep() - offset;
    return currentStep >= 0 && currentStep % interval == 0;
}

bool isIntervalOnOffStep(const IUseNerve* user, s32 interval, s32 offset) {
    return ((user->getNerveKeeper()->getCurrentStep() - offset) / interval) % 2 == 0;
}

bool isNewNerve(const IUseNerve* user) {
    return isLessStep(user, 0);
}

s32 calcNerveInterval(const IUseNerve* user, s32 interval, s32 offset) {
    s32 remain = getNerveStep(user) - offset;

    if (interval < 1 || remain < 1)
        return 0;

    return remain / interval;
}

f32 calcNerveRate(const IUseNerve* user, s32 max) {
    if (max < 1)
        return 1.0f;

    f32 curStep = getNerveStep(user);
    return sead::Mathf::clamp(curStep / max, 0.0f, 1.0f);
}

f32 calcNerveRate(const IUseNerve* user, s32 min, s32 max) {
    f32 rate = normalize((f32)getNerveStep(user), (f32)min, (f32)max);
    return sead::Mathf::clamp(rate, 0.0f, 1.0f);
}

f32 calcNerveEaseInRate(const IUseNerve* user, s32 max) {
    return easeIn(calcNerveRate(user, max));
}

f32 calcNerveEaseInRate(const IUseNerve* user, s32 min, s32 max) {
    return easeIn(calcNerveRate(user, min, max));
}

f32 calcNerveEaseOutRate(const IUseNerve* user, s32 max) {
    return easeOut(calcNerveRate(user, max));
}

f32 calcNerveEaseOutRate(const IUseNerve* user, s32 min, s32 max) {
    return easeOut(calcNerveRate(user, min, max));
}

f32 calcNerveEaseInOutRate(const IUseNerve* user, s32 max) {
    return easeInOut(calcNerveRate(user, max));
}

f32 calcNerveEaseInOutRate(const IUseNerve* user, s32 min, s32 max) {
    return easeInOut(calcNerveRate(user, min, max));
}

f32 calcNerveSquareInRate(const IUseNerve* user, s32 max) {
    return squareIn(calcNerveRate(user, max));
}

f32 calcNerveSquareInRate(const IUseNerve* user, s32 min, s32 max) {
    return squareIn(calcNerveRate(user, min, max));
}

f32 calcNerveSquareOutRate(const IUseNerve* user, s32 max) {
    return squareOut(calcNerveRate(user, max));
}

f32 calcNerveSquareOutRate(const IUseNerve* user, s32 min, s32 max) {
    return squareOut(calcNerveRate(user, min, max));
}

f32 calcNerveEaseByTypeRate(const IUseNerve* user, s32 max, s32 type) {
    return easeByType(calcNerveRate(user, max), type);
}

f32 calcNerveEaseByTypeRate(const IUseNerve* user, s32 min, s32 max, s32 type) {
    return easeByType(calcNerveRate(user, min, max), type);
}

f32 calcNervePowerInRate(const IUseNerve* user, s32 max, f32 power) {
    return powerIn(calcNerveRate(user, max), power);
}

f32 calcNervePowerInRate(const IUseNerve* user, s32 min, s32 max, f32 power) {
    return powerIn(calcNerveRate(user, min, max), power);
}

f32 calcNervePowerOutRate(const IUseNerve* user, s32 max, f32 power) {
    return powerOut(calcNerveRate(user, max), power);
}

f32 calcNervePowerOutRate(const IUseNerve* user, s32 min, s32 max, f32 power) {
    return powerOut(calcNerveRate(user, min, max), power);
}

f32 calcNerveJumpRate(const IUseNerve* user, s32 inMax, s32 upDuration, s32 release) {
    s32 step = getNerveStep(user);
    if (step <= inMax)
        return calcNerveEaseOutRate(user, inMax);

    s32 startRelease = upDuration + inMax;
    if (step <= startRelease)
        return 1.0f;

    return lerpValue(1.0f, 0.0f, calcNerveEaseInRate(user, startRelease, startRelease + release));
}

f32 calcNerveEaseInValue(const IUseNerve* user, s32 min, s32 max, f32 start, f32 end) {
    return lerpValue(start, end, calcNerveEaseInRate(user, min, max));
}

f32 calcNerveStartEndRate(const IUseNerve* user, s32 inMax, s32 upDuration, s32 release) {
    s32 step = getNerveStep(user);
    if (step <= inMax)
        return calcNerveEaseInOutRate(user, inMax);

    s32 startRelease = upDuration + inMax;
    if (step <= startRelease)
        return 1.0f;

    return lerpValue(1.0f, 0.0f,
                     calcNerveEaseInOutRate(user, startRelease, startRelease + release));
}

f32 calcNerveEaseInOutValue(const IUseNerve* user, s32 min, s32 max, f32 start, f32 end) {
    return lerpValue(start, end, calcNerveEaseInOutRate(user, min, max));
}

f32 calcNerveValue(const IUseNerve* user, s32 max, f32 start, f32 end) {
    return lerpValue(start, end, calcNerveRate(user, max));
}

f32 calcNerveValue(const IUseNerve* user, s32 min, s32 max, f32 start, f32 end) {
    return lerpValue(start, end, calcNerveRate(user, min, max));
}

f32 calcNerveEaseInValue(const IUseNerve* user, s32 max, f32 start, f32 end) {
    return lerpValue(start, end, calcNerveEaseInRate(user, max));
}

f32 calcNerveEaseOutValue(const IUseNerve* user, s32 max, f32 start, f32 end) {
    return lerpValue(start, end, calcNerveEaseOutRate(user, max));
}

f32 calcNerveEaseOutValue(const IUseNerve* user, s32 min, s32 max, f32 start, f32 end) {
    return lerpValue(start, end, calcNerveEaseOutRate(user, min, max));
}

f32 calcNerveEaseInOutValue(const IUseNerve* user, s32 max, f32 start, f32 end) {
    return lerpValue(start, end, calcNerveEaseInOutRate(user, max));
}

f32 calcNerveSquareInValue(const IUseNerve* user, s32 max, f32 start, f32 end) {
    return lerpValue(start, end, calcNerveSquareInRate(user, max));
}

f32 calcNerveSquareInValue(const IUseNerve* user, s32 min, s32 max, f32 start, f32 end) {
    return lerpValue(start, end, calcNerveSquareInRate(user, min, max));
}

f32 calcNerveSquareOutValue(const IUseNerve* user, s32 max, f32 start, f32 end) {
    return lerpValue(start, end, calcNerveSquareOutRate(user, max));
}

f32 calcNerveSquareOutValue(const IUseNerve* user, s32 min, s32 max, f32 start, f32 end) {
    return lerpValue(start, end, calcNerveSquareOutRate(user, min, max));
}

f32 calcNerveEaseByTypeValue(const IUseNerve* user, s32 max, f32 start, f32 end, s32 type) {
    return lerpValue(start, end, calcNerveEaseByTypeRate(user, max, type));
}

f32 calcNerveEaseByTypeValue(const IUseNerve* user, s32 min, s32 max, f32 start, f32 end,
                             s32 type) {
    return lerpValue(start, end, calcNerveEaseByTypeRate(user, min, max, type));
}

f32 calcNerveCosCycle(const IUseNerve* user, s32 max) {
    if (max == 0)
        return 1.0f;
    return sead::Mathf::cos((f32)getNerveStep(user) / max * sead::Mathf::pi2());
}

f32 calcNerveSinCycle(const IUseNerve* user, s32 max) {
    if (max == 0)
        return 1.0f;
    return sead::Mathf::sin((f32)getNerveStep(user) / max * sead::Mathf::pi2());
}

f32 calcNerveRepeatRate(const IUseNerve* user, s32 max) {
    return (getNerveStep(user) % max) / (f32)max;
}

f32 calcNerveRepeatDegree(const IUseNerve* user, s32 max) {
    return calcNerveRepeatRate(user, max) * 360.0f;
}

f32 calcNerveJumpValue(const IUseNerve* user, s32 inMax, s32 upDuration, s32 release, f32 factor) {
    return calcNerveJumpRate(user, inMax, upDuration, release) * factor;
}

f32 calcNerveStartEndValue(const IUseNerve* user, s32 inMax, s32 upDuration, s32 release, f32 start,
                           f32 end) {
    return lerpValue(start, end, calcNerveStartEndRate(user, inMax, upDuration, release));
}

void initNerveState(IUseNerve* user, NerveStateBase* state, const Nerve* nerve, const char* name) {
    state->init();
    user->getNerveKeeper()->getStateCtrl()->addState(state, nerve, name);
}

void addNerveState(IUseNerve* user, NerveStateBase* state, const Nerve* nerve, const char* name) {
    user->getNerveKeeper()->getStateCtrl()->addState(state, nerve, name);
}

bool updateNerveState(IUseNerve* user) {
    return user->getNerveKeeper()->getStateCtrl()->updateCurrentState();
}

bool updateNerveStateAndNextNerve(IUseNerve* user, const Nerve* nerve) {
    if (user->getNerveKeeper()->getStateCtrl()->updateCurrentState()) {
        user->getNerveKeeper()->setNerve(nerve);
        return true;
    }

    return false;
}

bool isStateEnd(const IUseNerve* user) {
    return user->getNerveKeeper()->getStateCtrl()->isCurrentStateEnd();
}

}  // namespace al

namespace alNerveFunction {

void setNerveAction(al::IUseNerve* user, const char* action) {
    al::NerveKeeper* keeper = user->getNerveKeeper();
    keeper->setNerve(keeper->getActionCtrl()->findNerve(action));
}

}  // namespace alNerveFunction
