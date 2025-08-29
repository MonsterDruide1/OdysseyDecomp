#include "Util/NpcEventFlowUtil.h"

#include "Library/Base/StringUtil.h"
#include "Library/Camera/CameraPoser.h"
#include "Library/Camera/CameraPoserFix.h"
#include "Library/Camera/CameraPoserFunction.h"
#include "Library/Camera/CameraTicket.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/Event/EventFlowDataHolder.h"
#include "Library/Event/EventFlowExecutor.h"
#include "Library/LiveActor/LiveActor.h"

namespace rs {

void initEventCameraObject(al::EventFlowExecutor* flowExecutor, const al::ActorInitInfo& initInfo,
                           const char* name) {
    al::CameraTicket* cameraTicket =
        al::initObjectCamera(flowExecutor->getActor(), initInfo, name, "会話用2点間");
    alCameraFunction::initPriorityDemoTalk(cameraTicket);
    alCameraFunction::validateKeepPreSelfPoseNextCamera(cameraTicket);
    alCameraFunction::validateCameraInterpoleEaseOut(cameraTicket);

    if (al::isEqualString(cameraTicket->getPoser()->getName(), "固定") ||
        al::isEqualString(cameraTicket->getPoser()->getName(), "完全固定") ||
        al::isEqualString(cameraTicket->getPoser()->getName(), "出入口専用固定")) {
        al::CameraPoserFix* poser = static_cast<al::CameraPoserFix*>(cameraTicket->getPoser());
        poser->setIsCalcNearestAtFromPreAt(true);
        alCameraPoserFunction::invalidateKeepDistanceNextCameraIfNoCollide(poser);
    }

    flowExecutor->getEventFlowDataHolder()->initCamera(name, cameraTicket);
}

}  // namespace rs
