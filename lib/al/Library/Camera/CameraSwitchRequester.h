#pragma once

#include <basis/seadTypes.h>

namespace al {

struct CameraPoseInfo;
class CameraSwitchRequestInfo;
class CameraTicket;

class CameraSwitchRequester {
public:
    CameraSwitchRequester();

    void init(CameraSwitchRequestInfo* start, CameraSwitchRequestInfo* end);
    // TODO: rename `i`
    void requestStart(CameraTicket* ticket, s32 i);
    // TODO: rename `i`, `b`
    void requestEnd(CameraTicket* ticket, s32 i, bool b);
    // TODO: rename `i`
    void requestEndWithNextCameraPose(CameraTicket* ticket, const CameraPoseInfo* poseInfo, s32 i);

private:
    CameraSwitchRequestInfo* mStart = nullptr;
    CameraSwitchRequestInfo* mEnd = nullptr;
};

static_assert(sizeof(CameraSwitchRequester) == 0x10);

}  // namespace al
