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

int getNerveStep(const IUseNerve*);
const Nerve* getCurrentNerve(const IUseNerve*);

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

int calcNerveInterval(const IUseNerve* user, s32, s32);
float calcNerveRate(const IUseNerve* user, s32);
float calcNerveRate(const IUseNerve* user, s32, s32);
float calcNerveEaseInRate(const IUseNerve* user, s32);
float calcNerveEaseInRate(const IUseNerve* user, s32, s32);
float calcNerveEaseOutRate(const IUseNerve* user, s32);
float calcNerveEaseOutRate(const IUseNerve* user, s32, s32);
float calcNerveEaseInOutRate(const IUseNerve* user, s32);
float calcNerveEaseInOutRate(const IUseNerve* user, s32, s32);
float calcNerveSquareInRate(const IUseNerve* user, s32);
float calcNerveSquareInRate(const IUseNerve* user, s32, s32);
float calcNerveSquareOutRate(const IUseNerve* user, s32);
float calcNerveSquareOutRate(const IUseNerve* user, s32, s32);
float calcNerveEaseByTypeRate(const IUseNerve* user, s32, s32);
float calcNerveEaseByTypeRate(const IUseNerve* user, s32, s32, s32);
float calcNervePowerInRate(const IUseNerve* user, s32, f32);
float calcNervePowerInRate(const IUseNerve* user, s32, s32, f32);
float calcNervePowerOutRate(const IUseNerve* user, s32, f32);
float calcNervePowerOutRate(const IUseNerve* user, s32, s32, f32);
float calcNerveJumpRate(const IUseNerve* user, s32, s32, s32);
float calcNerveEaseInValue(const IUseNerve* user, s32, s32, f32, f32);
float calcNerveStartEndRate(const IUseNerve* user, s32, s32, s32);
float calcNerveEaseInOutValue(const IUseNerve* user, s32, s32, f32, f32);
float calcNerveValue(const IUseNerve* user, s32, f32, f32);
float calcNerveValue(const IUseNerve* user, s32, s32, f32, f32);
float calcNerveEaseInValue(const IUseNerve* user, s32, f32, f32);
float calcNerveEaseOutValue(const IUseNerve* user, s32, f32, f32);
float calcNerveEaseOutValue(const IUseNerve* user, s32, s32, f32, f32);
float calcNerveEaseInOutValue(const IUseNerve* user, s32, f32, f32);
float calcNerveSquareInValue(const IUseNerve* user, s32, f32, f32);
float calcNerveSquareInValue(const IUseNerve* user, s32, s32, f32, f32);
float calcNerveSquareOutValue(const IUseNerve* user, s32, f32, f32);
float calcNerveSquareOutValue(const IUseNerve* user, s32, s32, f32, f32);
float calcNerveEaseByTypeValue(const IUseNerve* user, s32, f32, f32, s32);
float calcNerveEaseByTypeValue(const IUseNerve* user, s32, s32, f32, f32, s32);
float calcNerveCosCycle(const IUseNerve* user, s32);
float calcNerveSinCycle(const IUseNerve* user, s32);
float calcNerveRepeatRate(const IUseNerve* user, s32);
float calcNerveRepeatDegree(const IUseNerve* user, s32);
float calcNerveJumpValue(const IUseNerve* user, s32, s32, s32, f32);
float calcNerveStartEndValue(const IUseNerve* user, s32, s32, s32, f32, f32);

void initNerveState(IUseNerve* user, NerveStateBase* state, const Nerve* nerve, const char* hostName);
void addNerveState(IUseNerve* user, NerveStateBase* state, const Nerve* nerve, const char* hostName);
bool updateNerveState(IUseNerve* user);
bool updateNerveStateAndNextNerve(IUseNerve* user, const Nerve* nerve);
bool isStateEnd(const IUseNerve* user);
void initNerve(LiveActor* actor, const Nerve* nerve, int);
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
