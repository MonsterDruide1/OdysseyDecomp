#include "Util/SequentialUtil.h"

#include "Library/Math/MathUtil.h"

void rs::convertCsecTo(s32* minutes, s32* seconds, s32* csec, s32 time) {
    *minutes = time / 6000;
    *seconds = (time - (time / 6000) * 6000) / 100;
    *csec = time - (*minutes * 6000) - (*seconds * 100);
}

s32 RaceTimeFunction::getRaceTimeMaxFrame() {
    return 215999;
}

u32 RaceTimeFunction::getRaceTimeMaxCsec() {
    return 359999;
}

s32 RaceTimeFunction::clampRaceRecordCsec(s32 csec) {
    return sead::Mathi::clamp(csec, 0, getRaceTimeMaxCsec());
}
