#include "Npc/AmiiboNpcStateChangeCostume.h"

#include "Library/Camera/CameraUtil.h"
#include "Library/Event/EventFlowUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"

#include "Util/NpcEventFlowUtil.h"
#include "Util/PlayerDemoUtil.h"

AmiiboNpcStateChangeCostume::AmiiboNpcStateChangeCostume(al::LiveActor* actor,
                                                         const al::PlacementInfo& placementInfo,
                                                         al::EventFlowExecutor* eventFlowExecutor)
    : al::ActorStateBase("", actor), mPlacementInfo(&placementInfo),
      mEventFlowExecutor(eventFlowExecutor) {}

void AmiiboNpcStateChangeCostume::init() {
    mCameraTicket = al::initEntranceCamera(mActor, *mPlacementInfo, "");

    al::PlacementInfo placementInfo;
    if (al::tryGetLinksInfo(&placementInfo, *mPlacementInfo, "PlayerRestartPos")) {
        al::getTrans(&mPlayerRestartTrans, placementInfo);
        al::getQuat(&mPlayerRestartQuat, placementInfo);
    }
}

void AmiiboNpcStateChangeCostume::appear() {
    al::NerveStateBase::appear();

    if (!al::isActiveCamera(mCameraTicket)) {
        al::startCamera(mActor, mCameraTicket, -1);
        al::requestCancelCameraInterpole(mActor, 0);
    }

    rs::startEventFlow(mEventFlowExecutor, "ChangeCostume");
}

void AmiiboNpcStateChangeCostume::control() {
    if (rs::updateEventFlow(mEventFlowExecutor))
        kill();
}

bool AmiiboNpcStateChangeCostume::receiveEvent(const al::EventFlowEventData* eventData) {
    if (al::isEventName(eventData, "AmiiboFirstTalkEnd") ||
        al::isEventName(eventData, "AmiiboTalkEnd"))
        return true;

    if (al::isEventName(eventData, "AmiiboPlayerPoseSet")) {
        rs::replaceDemoPlayer(mActor, mPlayerRestartTrans, mPlayerRestartQuat);
        return true;
    }

    return false;
}
