#include "Library/Nerve/NerveUtil.h"
#include <algorithm>
#include "Library/Nerve/IUseNerve.h"
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
    if (currentStep < 0)
        return false;
    return currentStep == (interval != 0 ? currentStep / interval : 0) * interval;
}
bool isIntervalOnOffStep(const IUseNerve* user, s32 interval, s32 offset) {
    if (interval == 0)
        return false;
    return ((user->getNerveKeeper()->getCurrentStep() - offset) / interval) == 0;
}
bool isNerve(const IUseNerve* user, const Nerve* nerve) {
    return user->getNerveKeeper()->getCurrentNerve() == nerve;
}
bool isNewNerve(const IUseNerve* user) {
    return user->getNerveKeeper()->isNewNerve();
}

s32 calcNerveInterval(const IUseNerve* pKeeper, s32 start, s32 end) {
    NerveKeeper* keeper = pKeeper->getNerveKeeper();

    s32 dist = keeper->getCurrentStep() - end;

    if (start < 1 || dist < 1)
        return 0;

    return dist / start;
}

f32 calcNerveRate(const IUseNerve* user, s32 step) {
    if (step < 1)
        return 1.0f;

    f32 curStep = user->getNerveKeeper()->getCurrentStep();
    f32 ret = curStep / step;

    if (ret < 0.0f)
        ret = 0.0f;
    else if (ret > 1.0f)
        ret = 1.0f;

    return ret;
}

void initNerveState(IUseNerve* user, NerveStateBase* state, const Nerve* nerve, const char* name) {
    state->init();
    user->getNerveKeeper()->getStateCtrl()->addState(state, nerve, name);
}

void addNerveState(const IUseNerve* user, NerveStateBase* state, const Nerve* nerve,
                   const char* name) {
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
