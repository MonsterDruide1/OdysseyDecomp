#pragma once

#include <basis/seadTypes.h>

namespace al {

class CameraTicket;
struct CameraPoseInfo;

class CameraSwitchRequestInfo {
public:
    CameraSwitchRequestInfo();

    void addRequest(CameraTicket* ticket, s32, bool);
    void addRequestWithNextCameraPose(CameraTicket* ticket, const CameraPoseInfo* nextPose, s32);
    bool tryRemoveRequestIfExist(CameraTicket* ticket);
    void reset();

private:
    CameraTicket** mRequests;
    s32 mNumRequests;
    s32 _c;
    bool _10;
    bool mHasNextCameraPose;
    CameraPoseInfo* mNextCameraPose;
};

static_assert(sizeof(CameraSwitchRequestInfo) == 0x20);
}  // namespace al
