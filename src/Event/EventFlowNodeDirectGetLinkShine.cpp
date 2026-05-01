#include "Event/EventFlowNodeDirectGetLinkShine.h"

#include "Library/Base/StringUtil.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/Event/EventFlowFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Item/Shine.h"
#include "Util/ItemUtil.h"
#include "Util/NpcEventFlowUtil.h"
#include "Util/PlayerUtil.h"

namespace {
NERVE_IMPL(EventFlowNodeDirectGetLinkShine, WaitCameraInterpole);
NERVE_IMPL(EventFlowNodeDirectGetLinkShine, WaitGetDemo);
NERVES_MAKE_NOSTRUCT(EventFlowNodeDirectGetLinkShine, WaitCameraInterpole, WaitGetDemo);
}  // namespace

EventFlowNodeDirectGetLinkShine::EventFlowNodeDirectGetLinkShine(const char* name)
    : al::EventFlowNode(name),
      mIsTimeBalloon(al::isEqualString(name, "DirectGetLinkShineTimeBalloon")) {}

void EventFlowNodeDirectGetLinkShine::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    initNerve(&WaitCameraInterpole, 0);

    if (mIsTimeBalloon) {
        mShine = rs::initLinkShopShine(*al::getActorInitInfo(info), "ShineActor");
        return;
    }

    bool isMiniGame = false;
    al::tryGetParamIterKeyBool(&isMiniGame, info, "IsMiniGame");

    const char* linkName = al::tryGetParamIterKeyString(info, "LinkName");
    if (linkName == nullptr)
        linkName = "ShineActor";

    mShine = rs::tryInitLinkShine(*al::getActorInitInfo(info), linkName, 0);
}

void EventFlowNodeDirectGetLinkShine::start() {
    al::EventFlowNode::start();

    if (!rs::isActiveEventDemo(this))
        end();

    if (mShine == nullptr) {
        end();
        return;
    }

    if (!rs::isCloseNpcDemoEventTalkMessage(getActor()))
        rs::startCloseNpcDemoEventTalkMessage(getActor());

    if (al::isActiveCameraInterpole(getActor(), 0)) {
        al::setNerve(this, &WaitCameraInterpole);
        return;
    }

    al::setNerve(this, &WaitGetDemo);
}

void EventFlowNodeDirectGetLinkShine::exeWaitCameraInterpole() {
    if (!al::isActiveCameraInterpole(getActor(), 0))
        al::setNerve(this, &WaitGetDemo);
}

void EventFlowNodeDirectGetLinkShine::exeWaitGetDemo() {
    if (rs::isCloseNpcDemoEventTalkMessage(getActor())) {
        if (al::isFirstStep(this)) {
            al::resetPosition(mShine, rs::getPlayerHeadPos(mShine));
            rs::requestEventGetShineDirect(this, mShine);
        }

        if (rs::checkEndSceneExecuteAndResetRequest(this))
            end();
    } else {
        rs::startCloseNpcDemoEventTalkMessage(getActor());
        al::setNerve(this, &WaitGetDemo);
    }
}
