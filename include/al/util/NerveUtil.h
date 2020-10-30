#pragma once

#include "al/nerve/Nerve.h"
#include "al/nerve/NerveStateBase.h"

namespace al
{
    void setNerve(al::IUseNerve *, const al::Nerve *);
    void setNerveAtStep(al::IUseNerve *, const al::Nerve *, int);
    bool isStep(const al::IUseNerve *, int);
    void setNerveAtGreaterEqualStep(al::IUseNerve *, const al::Nerve *, int);
    bool isGreaterEqualStep(const al::IUseNerve *, int);
    bool isNerve(const al::IUseNerve *, const al::Nerve *);
    int getNerveStep(const al::IUseNerve *);
    const al::Nerve* getCurrentNerve(const al::IUseNerve *);
    bool isFirstStep(const al::IUseNerve *);
    bool isLessStep(const al::IUseNerve *, int);
    bool isGreaterStep(const al::IUseNerve *, int);
    bool isInRangeStep(const al::IUseNerve *, int, int);
    bool isIntervalStep(const al::IUseNerve *, int, int);
    bool isIntervalOnOffStep(const al::IUseNerve *, int, int);
    bool isNewNerve(const al::IUseNerve *);
    int calcNerveInterval(const al::IUseNerve *, int, int);
    float calcNerveRate(const al::IUseNerve *, int);
    float calcNerveRate(const al::IUseNerve *, int, int);

    void initNerveState(al::IUseNerve *, al::NerveStateBase *, const al::Nerve *, const char *);
    void addNerveState(al::IUseNerve *, al::NerveStateBase *, const al::Nerve *, const char *);
    void updateNerveState(al::IUseNerve *);
    bool updateNerveStateAndNextNerve(al::IUseNerve *, const al::Nerve *);
    bool isStateEnd(const al::IUseNerve *);
};