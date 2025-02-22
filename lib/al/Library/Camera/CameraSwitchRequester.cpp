#include "Library/Camera/CameraSwitchRequester.h"

#include "Library/Camera/CameraSwitchRequestInfo.h"
#include "Library/Camera/CameraTicket.h"

namespace al {

CameraSwitchRequester::CameraSwitchRequester() = default;

void CameraSwitchRequester::init(CameraSwitchRequestInfo* start, CameraSwitchRequestInfo* end) {
    mStart = start;
    mEnd = end;
};

void CameraSwitchRequester::requestStart(CameraTicket* ticket, s32 i) {
    ticket->setActiveCamera(true);
    mStart[ticket->getPriority()].addRequest(ticket, i, false);
}

void CameraSwitchRequester::requestEnd(CameraTicket* ticket, s32 i, bool b) {
    ticket->setActiveCamera(false);
    if (!mStart[ticket->getPriority()].tryRemoveRequestIfExist(ticket))
        mEnd[ticket->getPriority()].addRequest(ticket, i, b);
}

void CameraSwitchRequester::requestEndWithNextCameraPose(CameraTicket* ticket,
                                                         const CameraPoseInfo* poseInfo, s32 i) {
    ticket->setActiveCamera(false);
    if (!mStart[ticket->getPriority()].tryRemoveRequestIfExist(ticket))
        mEnd[ticket->getPriority()].addRequestWithNextCameraPose(ticket, poseInfo, i);
}

}  // namespace al
