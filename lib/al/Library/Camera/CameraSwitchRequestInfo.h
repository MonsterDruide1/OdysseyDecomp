#pragma once

#include <basis/seadTypes.h>

namespace al {

class CameraTicket;
struct CameraPoseInfo;

class CameraSwitchRequestInfo {
public:
    CameraSwitchRequestInfo();

    void addRequest(CameraTicket*, s32, bool);
    void addRequestWithNextCameraPose(CameraTicket* ticket, CameraPoseInfo* poseInfo, s32);
    bool tryRemoveRequestIfExist(CameraTicket* ticket);
    void reset();
};
}  // namespace al
