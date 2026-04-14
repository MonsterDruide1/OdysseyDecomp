#include "MapObj/DoorSnow.h"

#include "Library/Base/StringUtil.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "Util/NpcEventFlowUtil.h"

namespace {
NERVE_IMPL(DoorSnow, Wait);
NERVE_IMPL(DoorSnow, Open);

NERVES_MAKE_NOSTRUCT(DoorSnow, Wait, Open);

static const char* getIndexedAction(const char* actionNameFormat, u32 doorIndex) {
    al::StringTmp<64> actionName(actionNameFormat, doorIndex);
    return actionName.cstr();
}
}  // namespace

DoorSnow::DoorSnow(const char* name) : al::LiveActor(name) {}

void DoorSnow::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    al::initNerve(this, &Wait, 0);
    makeActorAlive();

    mCameraTicketOpen = al::initObjectCamera(this, info, "扉オープン1", nullptr);
    return alCameraFunction::validateKeepPreSelfPoseNextCamera(mCameraTicketOpen);
}

void DoorSnow::exeWait() {}

void DoorSnow::exeOpen() {
    if (al::isFirstStep(this)) {
        al::startAction(this, getIndexedAction("Open%d", mDoorIndex));
        al::startCamera(this, mCameraTicketOpen);
    }

    if (al::isActionEnd(this) && al::isGreaterEqualStep(this, 120)) {
        al::startAction(this, getIndexedAction("OpenWait%d", mDoorIndex));

        al::setNerve(this, &Wait);
        al::endCamera(this, mCameraTicketOpen, 0, false);
        rs::requestSwitchTalkNpcEventAfterDoorSnow(this, mDoorIndex);

        al::StringTmp<128> switchName("OutputSwitch%dOn", mDoorIndex);
        al::tryOnStageSwitch(this, switchName.cstr());

        al::validateClipping(this);
    }
}

void DoorSnow::reset(u32 doorIndex) {
    mDoorIndex = doorIndex;
    if (doorIndex == 0)
        return;

    al::startAction(this, getIndexedAction("OpenWait%d", doorIndex));

    al::setNerve(this, &Wait);

    al::StringTmp<128> switchName("OutputSwitch%dOn", mDoorIndex);
    al::tryOnStageSwitch(this, switchName.cstr());

    rs::requestSwitchTalkNpcEventAfterDoorSnow(this, mDoorIndex);
}

void DoorSnow::startDemo(u32 doorIndex) {
    mDoorIndex = doorIndex;
    al::invalidateClipping(this);
    return al::setNerve(this, &Open);
}

bool DoorSnow::isDemoEnd() const {
    return al::isNerve(this, &Wait);
}
