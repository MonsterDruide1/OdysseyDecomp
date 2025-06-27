#include "Scene/StageSceneStateStartSeparatePlay.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Message/MessageHolder.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Play/Layout/SimpleLayoutAppearWaitEnd.h"
#include "Library/Play/Layout/WipeSimple.h"
#include "Library/Scene/Scene.h"
#include "Library/Se/SeFunction.h"

#include "Layout/FooterParts.h"
#include "Scene/StageSceneStatePauseMenu.h"
#include "Util/SpecialBuildUtil.h"
#include "Util/StageInputFunction.h"

namespace {
NERVE_IMPL(StageSceneStateStartSeparatePlay, Appear);
NERVE_IMPL(StageSceneStateStartSeparatePlay, FadeOut);
NERVE_IMPL(StageSceneStateStartSeparatePlay, FadeIn);
NERVE_IMPL(StageSceneStateStartSeparatePlay, Back);
NERVE_IMPL(StageSceneStateStartSeparatePlay, WaitDraw);
NERVE_IMPL(StageSceneStateStartSeparatePlay, Wait);
NERVE_IMPL(StageSceneStateStartSeparatePlay, Applet);

NERVES_MAKE_NOSTRUCT(StageSceneStateStartSeparatePlay, Wait, Applet);
NERVES_MAKE_STRUCT(StageSceneStateStartSeparatePlay, Appear, FadeOut, FadeIn, Back, WaitDraw);
}  // namespace

StageSceneStateStartSeparatePlay::StageSceneStateStartSeparatePlay(
    const char* name, StageSceneStatePauseMenu* host, const al::LayoutInitInfo& info,
    al::WipeSimple* wipeSimple, al::GamePadSystem* gamePadSystem, FooterParts* footerParts)
    : al::HostStateBase<StageSceneStatePauseMenu>(name, host), mGamePadSystem(gamePadSystem),
      mWipeSimple(wipeSimple), mFooterParts(footerParts) {
    initNerve(&NrvStageSceneStateStartSeparatePlay.Appear, 0);

    mControllerGuideMulti = new al::SimpleLayoutAppearWaitEnd(
        "おすそ分け開始", "ControllerGuideMulti", info, nullptr, false);
    al::killLayoutIfActive(mControllerGuideMulti);
}

void StageSceneStateStartSeparatePlay::appear() {
    field_40 = 0;
    mIsCancel = false;
    al::NerveStateBase::appear();
    if (rs::isModeE3LiveRom()) {
        startTreeHouse();
        return;
    }
    al::setNerve(this, &NrvStageSceneStateStartSeparatePlay.Appear);
}

void StageSceneStateStartSeparatePlay::startTreeHouse() {
    al::setNerve(this, &NrvStageSceneStateStartSeparatePlay.FadeOut);
}

bool StageSceneStateStartSeparatePlay::isNeedRequestGraphicsPreset() const {
    return (mIsCancel && al::isNerve(this, &NrvStageSceneStateStartSeparatePlay.FadeIn)) ||
           (al::isNerve(this, &NrvStageSceneStateStartSeparatePlay.Appear) ||
            al::isNerve(this, &NrvStageSceneStateStartSeparatePlay.Back));
}

bool StageSceneStateStartSeparatePlay::isDrawViewRenderer() const {
    if (isDead() || mIsCancel)
        return false;
    return al::isNerve(this, &NrvStageSceneStateStartSeparatePlay.WaitDraw) ||
           al::isNerve(this, &NrvStageSceneStateStartSeparatePlay.FadeIn);
}

void StageSceneStateStartSeparatePlay::exeAppear() {
    if (al::isFirstStep(this)) {
        mControllerGuideMulti->appear();
        al::startAction(mControllerGuideMulti, "Loop", "Loop");
    }
    if (mControllerGuideMulti->isWait())
        al::setNerve(this, &Wait);
}

void StageSceneStateStartSeparatePlay::exeWait() {
    if (rs::isTriggerUiCancel(getScene())) {
        mIsCancel = true;
        al::setNerve(this, &NrvStageSceneStateStartSeparatePlay.Back);
        return;
    }
    if (rs::isTriggerUiDecide(getScene()))
        al::setNerve(this, &NrvStageSceneStateStartSeparatePlay.FadeOut);
}

al::Scene* StageSceneStateStartSeparatePlay::getScene() {
    return getHost()->getHost();
}

void StageSceneStateStartSeparatePlay::exeBack() {
    if (al::isFirstStep(this)) {
        mControllerGuideMulti->end();
        mFooterParts->changeTextFade(
            al::getSystemMessageString(mFooterParts, "Footer", "MenuMessage_Footer"));
    }
    if (!al::isActive(mControllerGuideMulti))
        kill();
}

void StageSceneStateStartSeparatePlay::exeFadeOut() {
    if (al::isFirstStep(this)) {
        mWipeSimple->startClose(60);
        al::startSe(mControllerGuideMulti, "Decide");
        al::startAction(mControllerGuideMulti, "Decide");
    }
    if (mWipeSimple->isCloseEnd())
        al::setNerve(this, &Applet);
}

void StageSceneStateStartSeparatePlay::exeApplet() {
    if (ControllerAppletFunction::connectControllerSeparatePlay(mGamePadSystem)) {
        rs::changeSeparatePlayMode(getScene(), true);
        al::setNerve(this, &NrvStageSceneStateStartSeparatePlay.WaitDraw);
        return;
    }
    mFooterParts->changeTextFade(
        al::getSystemMessageString(mFooterParts, "Footer", "MenuMessage_Footer"));
    mIsCancel = true;
    al::setNerve(this, &NrvStageSceneStateStartSeparatePlay.FadeIn);
}

void StageSceneStateStartSeparatePlay::exeFadeIn() {
    if (al::isFirstStep(this)) {
        mControllerGuideMulti->kill();
        mWipeSimple->startOpen(-1);
    }
    if (mWipeSimple->isOpenEnd())
        kill();
}

void StageSceneStateStartSeparatePlay::exeWaitDraw() {
    if (al::isFirstStep(this) && !mIsCancel)
        getHost()->killPauseMenu();
    if (al::isGreaterEqualStep(this, 2))
        al::setNerve(this, &NrvStageSceneStateStartSeparatePlay.FadeIn);
}
