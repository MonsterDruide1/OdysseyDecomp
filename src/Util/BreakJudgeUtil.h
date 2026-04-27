#pragma once

#include "Library/MapObj/BreakMapPartsBase.h"

namespace al {
class SensorMsg;
class HitSensor;
}  // namespace al

namespace rs {
al::JudgeFuncPtr getBreakJudgeFunction(const char* name);
}  // namespace rs
