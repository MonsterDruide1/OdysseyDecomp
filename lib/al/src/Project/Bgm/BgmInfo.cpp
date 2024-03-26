#include "Project/Bgm/BgmInfo.h"

#include <math.h>

namespace al {
BgmChangeableParams::BgmChangeableParams() {}

void BgmChangeableParams::calcPitch(float value) {
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

int BgmUserInfo::compareInfo(const BgmUserInfo* info_1, const BgmUserInfo* info_2) {
    return strcmp(info_1->mName, info_2->mName);
}
int BgmUserInfo::compareInfoByKey(const BgmUserInfo* info, const char* string) {
    return strcmp(info->mName, string);
}
}  // namespace al
