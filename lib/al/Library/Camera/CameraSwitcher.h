#pragma once

#include <basis/seadTypes.h>

#include "Library/Camera/CameraSwitchRequester.h"

namespace al {
class CameraTicket;

class CameraSwitcher {
public:
    void update();

    bool isExistNextCamera() const;
    CameraTicket* getNextCamera() const;

    bool isSetNextPoseInfo() const;
    CameraPoseInfo* getNextPoseInfo() const;

    s32 getNextInterpoleStep() const;

    bool isNextKeepPose() const;

    bool get_10() const { return _10; }

private:
    u32 _0;
    void* _8;
    bool _10;
    void* _18;
    CameraSwitchRequester mRequester;
};

static_assert(sizeof(CameraSwitcher) == 0x30);

}  // namespace al
