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
                                 const al::LiveActor* actor)
    : al::LayoutActor(name), mActor(actor) {
    al::initLayoutActor(this, info, "PlayGuideCamera", nullptr);
    initNerve(&NrvPlayGuideCamera.Hide, 0);
    appear();
    hide();
}

void PlayGuideCamera::hide() {
    al::hidePaneRoot(this);
    al::setNerve(this, &NrvPlayGuideCamera.Hide);
}

void PlayGuideCamera::start() {
    if (!rs::isActiveDemo(mActor))
        al::setNerve(this, &NrvPlayGuideCamera.Appear);
}

void PlayGuideCamera::exeHide() {
    if (!field_129 && tryAppear())
        return;

    if (!al::isExistCameraInputAtDisableTiming(this, 0))
        field_129 = false;
}

bool PlayGuideCamera::tryAppear() {
    if (!rs::isActiveDemo(mActor) && al::isExistCameraInputAtDisableTiming(this, 0) &&
        !rs::isPlayerHackGroupUseCameraStick(mActor)) {
        al::setNerve(this, &NrvPlayGuideCamera.Appear);
        return true;
    }
    return false;
}

void PlayGuideCamera::exeAppear() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Appear", nullptr);
        al::showPaneRoot(this);
        field_129 = true;
    }
    if (al::isActionEnd(this, nullptr))
        al::setNerve(this, &Wait);
    else if (!al::isExistCameraInputAtDisableTiming(this, 0))
        field_129 = false;
}

void PlayGuideCamera::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait", nullptr);

    if (al::isGreaterEqualStep(this, 30)) {
        al::setNerve(this, &NrvPlayGuideCamera.End);
        return;
    }

    if (al::isExistCameraInputAtDisableTiming(this, 0)) {
        if (field_129)
            return;
    } else
        field_129 = false;

    tryAppear();
}

void PlayGuideCamera::exeEnd() {
    if (al::isFirstStep(this))
        al::startAction(this, "End", nullptr);

    if (al::isActionEnd(this, nullptr)) {
        al::hidePaneRoot(this);
        al::setNerve(this, &NrvPlayGuideCamera.Hide);
    }

    if (al::isExistCameraInputAtDisableTiming(this, 0)) {
        if (field_129)
            return;
    } else
        field_129 = false;

    tryAppear();
}
