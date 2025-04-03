#pragma once

#include <basis/seadTypes.h>

namespace rs {
void convertCsecTo(s32* minutes, s32* seconds, s32* csec, s32 time);
}  // namespace rs

namespace RaceTimeFunction {
s32 getRaceTimeMaxFrame();
s32 getRaceTimeMaxCsec();
s32 clampRaceRecordCsec(s32 csec);
}  // namespace RaceTimeFunction
