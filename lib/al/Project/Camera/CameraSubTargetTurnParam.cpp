#include "Project/Camera/CameraSubTargetTurnParam.h"

#include "Library/Placement/PlacementFunction.h"

namespace al {
void CameraSubTargetTurnParam::init(const ActorInitInfo& info) {
    tryGetArg(&turnSpeedRate1, info, "TurnSpeedRate1");
    tryGetArg(&turnSpeedRate2, info, "TurnSpeedRate2");
    tryGetArg(&turnBrakeEndDistance, info, "TurnBrakeEndDistance");
    tryGetArg(&turnBrakeStartDistance, info, "TurnBrakeStartDistance");
    tryGetArg(&turnStopStartDistance, info, "TurnStopStartDistance");
    tryGetArg(&turnStopEndDistance, info, "TurnStopEndDistance");

    if (tryGetArg(&isTurnV, info, "IsTurnV") && isTurnV) {
        tryGetArg(&isResetAfterTurnV, info, "IsResetAfterTurnV");
        tryGetArg(&minTurnDegreeV, info, "MinTurnDegreeV");
        tryGetArg(&maxTurnDegreeV, info, "MaxTurnDegreeV");
    }

    tryGetArg(&validTurnDegreeRangeH, info, "ValidTurnDegreeRangeH");
    tryGetArg(&validFaceDegreeRangeH, info, "ValidFaceDegreeRangeH");
}
}  // namespace al
