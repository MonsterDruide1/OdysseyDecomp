#include "Project/Controller/PadUiKeyInputAddon.h"

#include <controller/seadController.h>
#include <controller/seadControllerBase.h>
#include <controller/seadControllerDefine.h>
#include <controller/seadControllerMgr.h>
#include <prim/seadBitFlag.h>

#include "Library/Controller/PadReplayFunction.h"
#include "Library/Controller/ReplayController.h"

namespace al {

PadUiKeyInputAddon::PadUiKeyInputAddon(sead::Controller* controller)
    : sead::ControllerAddon(controller) {
    mId = sead::ControllerDefine::AddonId::cAddon_UiKeyInput;
}

bool PadUiKeyInputAddon::calc() {
    s32 port = mController->getMgr()->findControllerPort(mController);
    sead::ControllerBase* controller;
    if (isValidReplayController(port))
        controller = getReplayController(port);
    else
        controller = mController;

    u32 prevPadHold = mPadHold;

    u32 maskUp = (1 << sead::Controller::cPadIdx_Up) | (1 << sead::Controller::cPadIdx_LeftStickUp);
    u32 maskDown =
        (1 << sead::Controller::cPadIdx_Down) | (1 << sead::Controller::cPadIdx_LeftStickDown);
    u32 maskLeft =
        (1 << sead::Controller::cPadIdx_Left) | (1 << sead::Controller::cPadIdx_LeftStickLeft);
    u32 maskRight =
        (1 << sead::Controller::cPadIdx_Right) | (1 << sead::Controller::cPadIdx_LeftStickRight);

    mPadHold.changeBit(0, controller->isHold(maskUp));
    mPadHold.changeBit(1, controller->isHold(maskDown));
    mPadHold.changeBit(2, controller->isHold(maskLeft));
    mPadHold.changeBit(3, controller->isHold(maskRight));

    mPadTrig.setDirect(mPadHold & ~prevPadHold);
    mPadHoldAndPrev.setDirect(mPadHold & prevPadHold);

    mPadRepeat.changeBit(0, controller->isTrigWithRepeat(maskUp));
    mPadRepeat.changeBit(1, controller->isTrigWithRepeat(maskDown));
    mPadRepeat.changeBit(2, controller->isTrigWithRepeat(maskLeft));
    mPadRepeat.changeBit(3, controller->isTrigWithRepeat(maskRight));

    return mPadHold == 0;
}

}  // namespace al
