#include "Scene/StageSceneStateEndSeparatePlay.h"

#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Wipe/WipeSimple.h"

#include "Scene/StageSceneStatePauseMenu.h"
#include "Util/StageInputFunction.h"

namespace {
NERVE_IMPL(StageSceneStateEndSeparatePlay, FadeOut);
NERVE_IMPL(StageSceneStateEndSeparatePlay, FadeIn);
NERVE_IMPL(StageSceneStateEndSeparatePlay, Applet);
NERVE_IMPL(StageSceneStateEndSeparatePlay, WaitDraw);

NERVES_MAKE_NOSTRUCT(StageSceneStateEndSeparatePlay, Applet);
NERVES_MAKE_STRUCT(StageSceneStateEndSeparatePlay, FadeOut, FadeIn, WaitDraw);
}  // namespace

StageSceneStateEndSeparatePlay::StageSceneStateEndSeparatePlay(const char* name,
                                                               StageSceneStatePauseMenu* host,
                                                               const al::LayoutInitInfo& info,
                                                               al::WipeSimple* wipeSimple,
                                                               al::GamePadSystem* gamePadSystem)
    : al::HostStateBase<StageSceneStatePauseMenu>(name, host), mGamePadSystem(gamePadSystem),
      mWipeSimple(wipeSimple) {
    initNerve(&NrvStageSceneStateEndSeparatePlay.FadeOut, 0);
}

void StageSceneStateEndSeparatePlay::appear() {
    setDead(false);
    field_30 = false;
    al::setNerve(this, &NrvStageSceneStateEndSeparatePlay.FadeOut);
}

bool StageSceneStateEndSeparatePlay::isNeedRequestGraphicsPreset() const {
    return (field_30 && al::isNerve(this, &NrvStageSceneStateEndSeparatePlay.FadeIn)) ||
           al::isNerve(this, &NrvStageSceneStateEndSeparatePlay.FadeOut);
}

void StageSceneStateEndSeparatePlay::exeFadeOut() {
    if (al::isFirstStep(this))
        mWipeSimple->startClose(-1);
    if (mWipeSimple->isCloseEnd())
        al::setNerve(this, &Applet);
}

void StageSceneStateEndSeparatePlay::exeApplet() {
    if (ControllerAppletFunction::connectControllerSinglePlay(mGamePadSystem))
        rs::changeSeparatePlayMode(getScene(), false);
    else
        field_30 = true;
    al::setNerve(this, &NrvStageSceneStateEndSeparatePlay.WaitDraw);
}

al::Scene* StageSceneStateEndSeparatePlay::getScene() {
    return getHost()->getHost();
}

void StageSceneStateEndSeparatePlay::exeFadeIn() {
    if (al::isFirstStep(this))
        mWipeSimple->startOpen(-1);
    if (mWipeSimple->isOpenEnd())
        kill();
}

void StageSceneStateEndSeparatePlay::exeWaitDraw() {
    if (al::isFirstStep(this) && !field_30)
        getHost()->killPauseMenu();
    if (al::isGreaterEqualStep(this, 2))
        al::setNerve(this, &NrvStageSceneStateEndSeparatePlay.FadeIn);
}

bool StageSceneStateEndSeparatePlay::isDrawViewRenderer() const {
    if (isDead() || field_30)
        return false;
    return al::isNerve(this, &NrvStageSceneStateEndSeparatePlay.WaitDraw) ||
           al::isNerve(this, &NrvStageSceneStateEndSeparatePlay.FadeIn);
}
