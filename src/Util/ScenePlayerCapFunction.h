#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
}

struct BindInfo;
class CapTargetInfo;

namespace PlayerCapFunction {
bool tryCalcHackCapThrowInputNormal(sead::Vector3f* out, const al::LiveActor* actor);
s32 getCapPadRumblePort(const al::LiveActor* actor);
bool isEnableBirdLandPlayerCapOn(const al::LiveActor* actor);
}  // namespace PlayerCapFunction

namespace InformationWindowFunction {
bool isShowPlayerHackHackTutorial(const al::LiveActor* actor, const char* suffix);
bool isShowPlayerBindTutorial(const al::LiveActor* actor, const BindInfo& bindInfo);
void setShowPlayerHackHackTutorial(const al::LiveActor* actor, const char* suffix);
}  // namespace InformationWindowFunction

namespace EventFlowFunction {
bool isCapTargetHackType(const CapTargetInfo* capTargetInfo, s32 hackType);
}  // namespace EventFlowFunction
