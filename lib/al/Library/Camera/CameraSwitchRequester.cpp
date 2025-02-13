#include "Library/Camera/CameraSwitchRequester.h"

#include "Library/Camera/CameraSwitchRequestInfo.h"
#include "Library/Camera/CameraTicket.h"

namespace al {

CameraSwitchRequester::CameraSwitchRequester() = default;

void CameraSwitchRequester::init(CameraSwitchRequestInfo* info0, CameraSwitchRequestInfo* info1) {
    _0 = info0;
    _8 = info1;
};

void CameraSwitchRequester::requestStart(CameraTicket* ticket, s32 i) {
    ticket->setActive(true);
    _0[ticket->getPriority()].addRequest(ticket, i, false);
}

void CameraSwitchRequester::requestEnd(CameraTicket* ticket, s32, bool) {}

void CameraSwitchRequester::requestEndWithNextCameraPose(CameraTicket* ticket,
                                                         CameraPoseInfo* poseInfo, s32) {}

}  // namespace al
