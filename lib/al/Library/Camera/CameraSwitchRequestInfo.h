#pragma once

#include <basis/seadTypes.h>

namespace al {

class CameraTicket;
struct CameraPoseInfo;

class CameraSwitchRequestInfo {
public:
    CameraSwitchRequestInfo();

    void addRequest(CameraTicket* ticket, s32, bool);
    void addRequestWithNextCameraPose(CameraTicket* ticket, const CameraPoseInfo* poseInfo, s32);
    bool tryRemoveRequestIfExist(CameraTicket* ticket);
    void reset();

private:
    CameraTicket** mTicketArray;
    s32 mTicketNumMax;
    s32 _c;
    bool _10;
    bool _11;
    CameraPoseInfo* mPoseInfo;
};

static_assert(sizeof(CameraSwitchRequestInfo) == 0x20);
}  // namespace al
