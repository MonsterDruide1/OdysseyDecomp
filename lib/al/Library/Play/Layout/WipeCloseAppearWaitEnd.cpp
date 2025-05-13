#include "Library/Play/Layout/WipeCloseAppearWaitEnd.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
using namespace al;

NERVE_HOST_TYPE_IMPL(WipeCloseAppearWaitEnd, CloseStart)
NERVE_HOST_TYPE_IMPL(WipeCloseAppearWaitEnd, CloseEnd)
NERVE_HOST_TYPE_IMPL(WipeCloseAppearWaitEnd, Open)
NERVE_HOST_TYPE_IMPL(WipeCloseAppearWaitEnd, Wait)
NERVE_HOST_TYPE_IMPL(WipeCloseAppearWaitEnd, CloseWait)

NERVES_MAKE_NOSTRUCT(HostType, CloseStart, CloseEnd, Open, Wait, CloseWait)
}  // namespace

namespace al {
WipeCloseAppearWaitEnd::WipeCloseAppearWaitEnd(const char* name, const char* param_2,
                                               const LayoutInitInfo& info, const char* param_4,
                                               bool isLocalized)
    : LayoutActor(name) {
    if (isLocalized)
        initLayoutActorLocalized(this, info, param_2, param_4);
    else
        initLayoutActor(this, info, param_2, param_4);

    initNerve(&CloseStart, 0);
}

void WipeCloseAppearWaitEnd::startClose(s32 param_1) {
    _130 = param_1;

    startAction(this, "CloseStart", nullptr);
    LayoutActor::appear();

    f32 rate;
    if (_130 <= 0) {
        asm("");
        rate = 1.0f;
    } else {
        rate = getActionFrameMax(this, "CloseStart", nullptr) / (f32)_130;
    }

    setActionFrameRate(this, rate, nullptr);
    setNerve(this, &CloseStart);
}

void WipeCloseAppearWaitEnd::startCloseEnd() {
    startFreezeActionEnd(this, "CloseEnd", nullptr);
    LayoutActor::appear();
    setNerve(this, &CloseEnd);
}

void WipeCloseAppearWaitEnd::startOpen(s32 param_1) {
    _130 = param_1;
    startAction(this, "End", nullptr);
    setNerve(this, &Open);
}

bool WipeCloseAppearWaitEnd::isCloseEnd() const {
    return isNerve(this, &Wait) && isGreaterEqualStep(this, 2);
}

void WipeCloseAppearWaitEnd::exeCloseStart() {
    if (isActionEnd(this, nullptr))
        setNerve(this, &CloseWait);
}

void WipeCloseAppearWaitEnd::exeCloseWait() {
    if (isFirstStep(this))
        startAction(this, "CloseWait", nullptr);

    if (mWaitDelay >= 0 && isGreaterEqualStep(this, mWaitDelay))
        setNerve(this, &CloseEnd);
}

void WipeCloseAppearWaitEnd::exeCloseEnd() {
    if (isFirstStep(this))
        startAction(this, "CloseEnd", nullptr);

    if (isActionEnd(this, nullptr))
        setNerve(this, &Wait);
}

void WipeCloseAppearWaitEnd::exeWait() {
    if (isFirstStep(this))
        startAction(this, "Wait", nullptr);
}

void WipeCloseAppearWaitEnd::exeOpen() {
    if (isActionEnd(this, nullptr))
        kill();
}
}  // namespace al
