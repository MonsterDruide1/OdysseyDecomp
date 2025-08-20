#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
}

class CapTargetInfo;

namespace PlayerCapFunction {
bool isEnableBirdLandPlayerCapOn(const al::LiveActor*);
}

namespace EventFlowFunction {
bool isCapTargetHackType(const CapTargetInfo*, s32);
}
