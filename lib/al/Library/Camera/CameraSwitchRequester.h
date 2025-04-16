#pragma once

namespace al {

class CameraSwitchRequestInfo;
class CameraTicket;
struct CameraPoseInfo;

class CameraSwitchRequester {
public:
    CameraSwitchRequester();

    void init(CameraSwitchRequestInfo* start, CameraSwitchRequestInfo* end);
    void requestStart(CameraTicket* ticket, s32);
    void requestEnd(CameraTicket* ticket, s32, bool);
    void requestEndWithNextCameraPose(CameraTicket* ticket, const CameraPoseInfo* poseInfo, s32);

private:
    CameraSwitchRequestInfo* mStart = nullptr;
    CameraSwitchRequestInfo* mEnd = nullptr;
};

static_assert(sizeof(CameraSwitchRequester) == 0x10);

}  // namespace al
