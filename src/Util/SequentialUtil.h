#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace rs {
void convertCsecTo(s32*, s32*, s32*, s32);
}

namespace RaceTimeFunction {
s32 getRaceTimeMaxFrame();
s32 getRaceTimeMaxCsec();
s32 clampRaceRecordCsec(s32);
}  // namespace RaceTimeFunction
