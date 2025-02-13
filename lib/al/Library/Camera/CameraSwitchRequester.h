#pragma once

namespace al {

struct CameraPoseInfo;
class CameraSwitchRequestInfo;
class CameraTicket;

class CameraSwitchRequester {
public:
    CameraSwitchRequester();

    void init(CameraSwitchRequestInfo* info0, CameraSwitchRequestInfo* info1);
    void requestStart(CameraTicket* ticket, s32);
    void requestEnd(CameraTicket* ticket, s32, bool);
    void requestEndWithNextCameraPose(CameraTicket* ticket, CameraPoseInfo* poseInfo, s32, bool);
};

}  // namespace al
