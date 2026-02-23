#include "Project/Bgm/BgmInfo.h"

#include <cmath>

namespace al {
BgmChangeableParams::BgmChangeableParams() = default;

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

BgmRegionChangeParams::BgmRegionChangeParams() {}

BgmRegionChangeParams::BgmRegionChangeParams(const BgmRegionChangeParams& other)
    : _0(other._0), _4(other._4), _8(other._8), _c(other._c), _d(other._d) {
    *this = other;
}

void BgmRegionChangeParams::operator=(const BgmRegionChangeParams& other) {
    _0 = other._0;
    _4 = other._4;
    _8 = other._8;
    _c = other._c;
    _d = other._d;
}

bool BgmRegionChangeParams::operator==(const BgmRegionChangeParams& other) const {
    if (_0 == other._0 && _4 == other._4 && _8 == other._8 && _c == other._c && _d == other._d)
        return true;
    return false;
}

BgmUserInfo::BgmUserInfo() = default;

s32 BgmUserInfo::compareInfo(const BgmUserInfo* info_1, const BgmUserInfo* info_2) {
    return strcmp(info_1->name, info_2->name);
}

s32 BgmUserInfo::compareInfoByKey(const BgmUserInfo* info, const char* string) {
    return strcmp(info->name, string);
}
}  // namespace al
