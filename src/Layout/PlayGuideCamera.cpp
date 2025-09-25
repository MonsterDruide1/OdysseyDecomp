#include "Layout/PlayGuideCamera.h"

#include "Library/Camera/CameraUtil.h"
#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/DemoUtil.h"
#include "Util/PlayerUtil.h"

namespace {
NERVE_IMPL(PlayGuideCamera, Hide);
NERVE_IMPL(PlayGuideCamera, Appear);
NERVE_IMPL(PlayGuideCamera, Wait);
NERVE_IMPL(PlayGuideCamera, End);

NERVES_MAKE_NOSTRUCT(PlayGuideCamera, Wait);
NERVES_MAKE_STRUCT(PlayGuideCamera, Hide, Appear, End);
}  // namespace

PlayGuideCamera::PlayGuideCamera(const char* name, const al::LayoutInitInfo& info,
                                 const al::LiveActor* player)
    : al::LayoutActor(name), mPlayer(player) {
    al::initLayoutActor(this, info, "PlayGuideCamera");
    initNerve(&NrvPlayGuideCamera.Hide);
    appear();
    hide();
}

void PlayGuideCamera::hide() {
    al::hidePaneRoot(this);
    al::setNerve(this, &NrvPlayGuideCamera.Hide);
}

void PlayGuideCamera::start() {
    if (!rs::isActiveDemo(mPlayer))
        al::setNerve(this, &NrvPlayGuideCamera.Appear);
}

void PlayGuideCamera::exeHide() {
    if (!mIsShown && tryAppear())
        return;

    if (!al::isExistCameraInputAtDisableTiming(this, 0))
        mIsShown = false;
}

bool PlayGuideCamera::tryAppear() {
    if (!rs::isActiveDemo(mPlayer) && al::isExistCameraInputAtDisableTiming(this, 0) &&
        !rs::isPlayerHackGroupUseCameraStick(mPlayer)) {
        al::setNerve(this, &NrvPlayGuideCamera.Appear);
        return true;
    }
    return false;
}

void PlayGuideCamera::exeAppear() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Appear");
        al::showPaneRoot(this);
        mIsShown = true;
    }
    if (al::isActionEnd(this))
        al::setNerve(this, &Wait);
    else if (!al::isExistCameraInputAtDisableTiming(this, 0))
        mIsShown = false;
}

void PlayGuideCamera::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");

    if (al::isGreaterEqualStep(this, 30)) {
        al::setNerve(this, &NrvPlayGuideCamera.End);
        return;
    }

    if (al::isExistCameraInputAtDisableTiming(this, 0)) {
        if (mIsShown)
            return;
    } else
        mIsShown = false;

    tryAppear();
}

void PlayGuideCamera::exeEnd() {
    if (al::isFirstStep(this))
        al::startAction(this, "End");

    if (al::isActionEnd(this)) {
        al::hidePaneRoot(this);
        al::setNerve(this, &NrvPlayGuideCamera.Hide);
    }

    if (al::isExistCameraInputAtDisableTiming(this, 0)) {
        if (mIsShown)
            return;
    } else
        mIsShown = false;

    tryAppear();
}
