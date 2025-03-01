#include "Project/Bgm/BgmInfo.h"

#include <cmath>

namespace al {
BgmChangeableParams::BgmChangeableParams() {}

void BgmChangeableParams::calcPitch(f32 value) {
    exp2f(value / 12);
}

void BgmChangeableParams::operator=(const BgmChangeableParams& value) {
    mVolume = value.mVolume;
    mPitch = value.mPitch;
    mLpfFreq = value.mLpfFreq;
    mBiquadFilter = value.mBiquadFilter;
    mVar = value.mVar;
    mAuxBusSend = value.mAuxBusSend;
    mTrackVolume0 = value.mTrackVolume0;
    mTrackVolume1 = value.mTrackVolume1;
    mTrackVolume2 = value.mTrackVolume2;
    mTrackVolume3 = value.mTrackVolume3;
    mTrackVolume4 = value.mTrackVolume4;
    mTrackVolume5 = value.mTrackVolume5;
}

BgmUserInfo::BgmUserInfo() {}

s32 BgmUserInfo::compareInfo(const BgmUserInfo* info_1, const BgmUserInfo* info_2) {
    return strcmp(info_1->name, info_2->name);
}

s32 BgmUserInfo::compareInfoByKey(const BgmUserInfo* info, const char* string) {
    return strcmp(info->name, string);
}
}  // namespace al
