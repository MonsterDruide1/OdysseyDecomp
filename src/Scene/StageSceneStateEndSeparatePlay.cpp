#include "Scene/StageSceneStateEndSeparatePlay.h"

#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Play/Layout/WipeSimple.h"

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
    mIsCancel = false;
    al::setNerve(this, &NrvStageSceneStateEndSeparatePlay.FadeOut);
}

bool StageSceneStateEndSeparatePlay::isNeedRequestGraphicsPreset() const {
    return (mIsCancel && al::isNerve(this, &NrvStageSceneStateEndSeparatePlay.FadeIn)) ||
           al::isNerve(this, &NrvStageSceneStateEndSeparatePlay.FadeOut);
}

void StageSceneStateEndSeparatePlay::exeFadeOut() {
    if (al::isFirstStep(this))
        mWipeSimple->startClose();
    if (mWipeSimple->isCloseEnd())
        al::setNerve(this, &Applet);
}

void StageSceneStateEndSeparatePlay::exeApplet() {
    if (ControllerAppletFunction::connectControllerSinglePlay(mGamePadSystem))
        rs::changeSeparatePlayMode(getScene(), false);
    else
        mIsCancel = true;
    al::setNerve(this, &NrvStageSceneStateEndSeparatePlay.WaitDraw);
}

al::Scene* StageSceneStateEndSeparatePlay::getScene() {
    return getHost()->getHost();
}

void StageSceneStateEndSeparatePlay::exeFadeIn() {
    if (al::isFirstStep(this))
        mWipeSimple->startOpen();
    if (mWipeSimple->isOpenEnd())
        kill();
}

void StageSceneStateEndSeparatePlay::exeWaitDraw() {
    if (al::isFirstStep(this) && !mIsCancel)
        getHost()->killPauseMenu();
    if (al::isGreaterEqualStep(this, 2))
        al::setNerve(this, &NrvStageSceneStateEndSeparatePlay.FadeIn);
}

bool StageSceneStateEndSeparatePlay::isDrawViewRenderer() const {
    if (isDead() || mIsCancel)
        return false;
    return al::isNerve(this, &NrvStageSceneStateEndSeparatePlay.WaitDraw) ||
           al::isNerve(this, &NrvStageSceneStateEndSeparatePlay.FadeIn);
}
