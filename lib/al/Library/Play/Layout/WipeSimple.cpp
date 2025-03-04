#include "Library/Play/Layout/WipeSimple.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActor.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
using namespace al;
NERVE_IMPL(WipeSimple, Close);
NERVE_IMPL(WipeSimple, CloseEnd);
NERVE_IMPL(WipeSimple, Open);

NERVES_MAKE_NOSTRUCT(WipeSimple, Open);
NERVES_MAKE_STRUCT(WipeSimple, Close, CloseEnd);
}  // namespace

namespace al {

WipeSimple::WipeSimple(const char* name, const char* layoutName, const LayoutInitInfo& info,
                       const char* actorName)
    : LayoutActor(name) {
    initLayoutActor(this, info, layoutName, actorName);
    initNerve(&NrvWipeSimple.Close, 0);
}

void WipeSimple::startClose(s32 frames) {
    mFrames = frames;
    startAction(this, "Appear", nullptr);
    LayoutActor::appear();

    if (mFrames <= 0)
        setActionFrameRate(this, 1.0f, nullptr);
    else
        setActionFrameRate(this, getActionFrameMax(this, "Appear", nullptr) / mFrames, nullptr);

    setNerve(this, &NrvWipeSimple.Close);
}

void WipeSimple::tryStartClose(s32 frames) {
    if (isOpenEnd() || (!isNerve(this, &NrvWipeSimple.Close) && !isCloseEnd()))
        startClose(frames);
}

void WipeSimple::startCloseEnd() {
    startAction(this, "Wait", nullptr);
    LayoutActor::appear();
    setNerve(this, &NrvWipeSimple.CloseEnd);
}

void WipeSimple::startOpen(s32 frames) {
    mFrames = frames;
    startAction(this, "End", nullptr);
    setNerve(this, &Open);
}

void WipeSimple::tryStartOpen(s32 frames) {
    if (!isOpenEnd() && !isNerve(this, &Open))
        startOpen(frames);
}

bool WipeSimple::isCloseEnd() const {
    return isNerve(this, &NrvWipeSimple.CloseEnd);
}

bool WipeSimple::isOpenEnd() const {
    return !isAlive();
}

void WipeSimple::exeClose() {
    if (!isFirstStep(this) && isActionEnd(this, nullptr))
        setNerve(this, &NrvWipeSimple.CloseEnd);
}

void WipeSimple::exeCloseEnd() {
    if (isFirstStep(this))
        startAction(this, "Wait", nullptr);
}

void WipeSimple::exeOpen() {
    if (isFirstStep(this)) {
        if (mFrames <= 0)
            setActionFrameRate(this, 1.0f, nullptr);
        else
            setActionFrameRate(this, getActionFrameMax(this, "End", nullptr) / mFrames, nullptr);
    }
    if (isActionEnd(this, nullptr))
        kill();
}

void WipeSimple::appear() {
    LayoutActor::appear();
}

}  // namespace al
