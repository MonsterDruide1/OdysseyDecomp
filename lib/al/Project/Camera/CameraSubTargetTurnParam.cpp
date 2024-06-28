#include "Project/Camera/CameraSubTargetTurnParam.h"

#include "Library/Placement/PlacementFunction.h"

namespace al {
void CameraSubTargetTurnParam::init(const ActorInitInfo& info) {
    tryGetArg(&mTurnSpeedRate1, info, "TurnSpeedRate1");
    tryGetArg(&mTurnSpeedRate2, info, "TurnSpeedRate2");
    tryGetArg(&mTurnBrakeEndDistance, info, "TurnBrakeEndDistance");
    tryGetArg(&mTurnBrakeStartDistance, info, "TurnBrakeStartDistance");
    tryGetArg(&mTurnStopStartDistance, info, "TurnStopStartDistance");
    tryGetArg(&mTurnStopEndDistance, info, "TurnStopEndDistance");

    if (tryGetArg(&mIsTurnV, info, "IsTurnV") && mIsTurnV) {
        tryGetArg(&mIsResetAfterTurnV, info, "IsResetAfterTurnV");
        tryGetArg(&mMinTurnDegreeV, info, "MinTurnDegreeV");
        tryGetArg(&mMaxTurnDegreeV, info, "MaxTurnDegreeV");
    }

    tryGetArg(&mValidTurnDegreeRangeH, info, "ValidTurnDegreeRangeH");
    tryGetArg(&mValidFaceDegreeRangeH, info, "ValidFaceDegreeRangeH");
}
}  // namespace al
