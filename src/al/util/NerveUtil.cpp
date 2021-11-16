#include "al/util/NerveUtil.h"
#include <algorithm>
#include "al/nerve/NerveKeeper.h"
#include "al/nerve/NerveStateCtrl.h"

namespace al {
void setNerve(al::IUseNerve* pKeeper, const al::Nerve* pNerve) {
    pKeeper->getNerveKeeper()->setNerve(pNerve);
}

void setNerveAtStep(al::IUseNerve* pKeeper, const al::Nerve* pNerve, int step) {
    if (pKeeper->getNerveKeeper()->mStep == step) {
        pKeeper->getNerveKeeper()->setNerve(pNerve);
    }
}
bool isStep(const al::IUseNerve* pKeeper, int step) {
    return pKeeper->getNerveKeeper()->mStep == step;
}

void setNerveAtGreaterEqualStep(al::IUseNerve* pKeeper, const al::Nerve* pNerve, int step) {
    if (pKeeper->getNerveKeeper()->mStep >= step) {
        pKeeper->getNerveKeeper()->setNerve(pNerve);
    }
}

bool isGreaterEqualStep(const al::IUseNerve* pKeeper, int step) {
    return pKeeper->getNerveKeeper()->mStep >= step;
}

bool isNerve(const al::IUseNerve* pKeeper, const al::Nerve* pNerve) {
    return pKeeper->getNerveKeeper()->getCurrentNerve() == pNerve;
}

int getNerveStep(const al::IUseNerve* pKeeper) {
    return pKeeper->getNerveKeeper()->mStep;
}

const al::Nerve* getCurrentNerve(const al::IUseNerve* pKeeper) {
    return pKeeper->getNerveKeeper()->getCurrentNerve();
}

bool isFirstStep(const al::IUseNerve* pKeeper) {
    return pKeeper->getNerveKeeper()->mStep == 0;
}

bool isLessStep(const al::IUseNerve* pKeeper, int step) {
    return pKeeper->getNerveKeeper()->mStep < step;
}

bool isLessEqualStep(const al::IUseNerve* pKeeper, int step) {
    return pKeeper->getNerveKeeper()->mStep <= step;
}

bool isGreaterStep(const al::IUseNerve* pKeeper, int step) {
    return pKeeper->getNerveKeeper()->mStep > step;
}

bool isInRangeStep(const al::IUseNerve* pKeeper, int low, int high) {
    int step = pKeeper->getNerveKeeper()->mStep;
    return (step <= high) & (step >= low);
}

int calcNerveInterval(const al::IUseNerve* pKeeper, int start, int end) {
    al::NerveKeeper* keeper = pKeeper->getNerveKeeper();

    int dist = keeper->mStep - end;

    if (start < 1 || dist < 1)
        return 0;

    return dist / start;
}

float calcNerveRate(const al::IUseNerve* pKeeper, int step) {
    if (step < 1)
        return 1.0f;

    float curStep = pKeeper->getNerveKeeper()->mStep;
    float ret = curStep / step;

    if (ret < 0.0f)
        ret = 0.0f;
    else if (ret > 1.0f)
        ret = 1.0f;

    return ret;
}

void initNerveState(al::IUseNerve* pKeeper, al::NerveStateBase* pStateBase, const al::Nerve* pNerve,
                    const char* pName) {
    pStateBase->init();
    pKeeper->getNerveKeeper()->mStateCtrl->addState(pStateBase, pNerve, pName);
}

void addNerveState(const al::IUseNerve* pKeeper, al::NerveStateBase* pStateBase,
                   const al::Nerve* pNerve, const char* pName) {
    pKeeper->getNerveKeeper()->mStateCtrl->addState(pStateBase, pNerve, pName);
}

void updateNerveState(al::IUseNerve* pKeeper) {
    pKeeper->getNerveKeeper()->mStateCtrl->updateCurrentState();
}

bool updateNerveStateAndNextNerve(al::IUseNerve* pKeeper, const al::Nerve* pNerve) {
    if (pKeeper->getNerveKeeper()->mStateCtrl->updateCurrentState()) {
        pKeeper->getNerveKeeper()->setNerve(pNerve);
        return true;
    }

    return false;
}

bool isStateEnd(const al::IUseNerve* pKeeper) {
    return pKeeper->getNerveKeeper()->mStateCtrl->isCurrentStateEnd();
}
}  // namespace al