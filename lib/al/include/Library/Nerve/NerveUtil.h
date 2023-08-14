#pragma once

#include <basis/seadTypes.h>

namespace al {
class IUseNerve;
class Nerve;
class NerveAction;
class NerveStateBase;
class LiveActor;

void setNerve(IUseNerve* user, const Nerve* nerve);
void setNerveAtStep(IUseNerve* user, const Nerve* nerve, s32 step);
void setNerveAtGreaterEqualStep(IUseNerve* user, const Nerve* nerve, s32 step);

s32 getNerveStep(const IUseNerve* user);
const Nerve* getCurrentNerve(const IUseNerve* user);

bool isStep(const IUseNerve* user, s32 step);
bool isFirstStep(const IUseNerve* user);
bool isGreaterStep(const IUseNerve* user, s32 step);
bool isGreaterEqualStep(const IUseNerve* user, s32 step);
bool isLessStep(const IUseNerve* user, s32 step);
bool isLessEqualStep(const IUseNerve* user, s32 step);
bool isInRangeStep(const IUseNerve* user, s32 startStep, s32 endStep);
bool isIntervalStep(const IUseNerve* user, s32 interval, s32 offset);
bool isIntervalOnOffStep(const IUseNerve* user, s32 interval, s32 offset);

bool isNerve(const IUseNerve* user, const Nerve* nerve);
bool isNewNerve(const IUseNerve* user);

s32 calcNerveInterval(const IUseNerve* user, s32, s32);
f32 calcNerveRate(const IUseNerve* user, s32);
f32 calcNerveRate(const IUseNerve* user, s32, s32);
f32 calcNerveEaseInRate(const IUseNerve* user, s32);
f32 calcNerveEaseInRate(const IUseNerve* user, s32, s32);
f32 calcNerveEaseOutRate(const IUseNerve* user, s32);
f32 calcNerveEaseOutRate(const IUseNerve* user, s32, s32);
f32 calcNerveEaseInOutRate(const IUseNerve* user, s32);
f32 calcNerveEaseInOutRate(const IUseNerve* user, s32, s32);
f32 calcNerveSquareInRate(const IUseNerve* user, s32);
f32 calcNerveSquareInRate(const IUseNerve* user, s32, s32);
f32 calcNerveSquareOutRate(const IUseNerve* user, s32);
f32 calcNerveSquareOutRate(const IUseNerve* user, s32, s32);
f32 calcNerveEaseByTypeRate(const IUseNerve* user, s32, s32);
f32 calcNerveEaseByTypeRate(const IUseNerve* user, s32, s32, s32);
f32 calcNervePowerInRate(const IUseNerve* user, s32, f32);
f32 calcNervePowerInRate(const IUseNerve* user, s32, s32, f32);
f32 calcNervePowerOutRate(const IUseNerve* user, s32, f32);
f32 calcNervePowerOutRate(const IUseNerve* user, s32, s32, f32);
f32 calcNerveJumpRate(const IUseNerve* user, s32, s32, s32);
f32 calcNerveEaseInValue(const IUseNerve* user, s32, s32, f32, f32);
f32 calcNerveStartEndRate(const IUseNerve* user, s32, s32, s32);
f32 calcNerveEaseInOutValue(const IUseNerve* user, s32, s32, f32, f32);
f32 calcNerveValue(const IUseNerve* user, s32, f32, f32);
f32 calcNerveValue(const IUseNerve* user, s32, s32, f32, f32);
f32 calcNerveEaseInValue(const IUseNerve* user, s32, f32, f32);
f32 calcNerveEaseOutValue(const IUseNerve* user, s32, f32, f32);
f32 calcNerveEaseOutValue(const IUseNerve* user, s32, s32, f32, f32);
f32 calcNerveEaseInOutValue(const IUseNerve* user, s32, f32, f32);
f32 calcNerveSquareInValue(const IUseNerve* user, s32, f32, f32);
f32 calcNerveSquareInValue(const IUseNerve* user, s32, s32, f32, f32);
f32 calcNerveSquareOutValue(const IUseNerve* user, s32, f32, f32);
f32 calcNerveSquareOutValue(const IUseNerve* user, s32, s32, f32, f32);
f32 calcNerveEaseByTypeValue(const IUseNerve* user, s32, f32, f32, s32);
f32 calcNerveEaseByTypeValue(const IUseNerve* user, s32, s32, f32, f32, s32);
f32 calcNerveCosCycle(const IUseNerve* user, s32);
f32 calcNerveSinCycle(const IUseNerve* user, s32);
f32 calcNerveRepeatRate(const IUseNerve* user, s32);
f32 calcNerveRepeatDegree(const IUseNerve* user, s32);
f32 calcNerveJumpValue(const IUseNerve* user, s32, s32, s32, f32);
f32 calcNerveStartEndValue(const IUseNerve* user, s32, s32, s32, f32, f32);

void initNerveState(IUseNerve* user, NerveStateBase* state, const Nerve* nerve,
                    const char* hostName);
void addNerveState(IUseNerve* user, NerveStateBase* state, const Nerve* nerve,
                   const char* hostName);
bool updateNerveState(IUseNerve* user);
bool updateNerveStateAndNextNerve(IUseNerve* user, const Nerve* nerve);
bool isStateEnd(const IUseNerve* user);
void initNerve(LiveActor* actor, const Nerve* nerve, s32);
}  // namespace al

namespace alNerveFunction {
void setNerveAction(al::IUseNerve* user, const char* action);

class NerveActionCollector {
    friend class al::NerveAction;

public:
    NerveActionCollector();

    void addNerve(al::NerveAction* action);

private:
    s32 mActionCount = 0;
    al::NerveAction* mHead = nullptr;
    al::NerveAction* mTail = nullptr;

    static alNerveFunction::NerveActionCollector* sCurrentCollector;
};

}  // namespace alNerveFunction
