#pragma once

#include <prim/seadSafeString.h>

namespace al {

class BgmChangeableParams {
public:
    BgmChangeableParams();

    void operator=(const BgmChangeableParams&);

    void calcPitch(float);

private:
    float mVolume = 1.0;
    float mPitch = 0;
    float mLpfFreq = 0;
    float mBiquadFilter = 0;
    float mVar = 0;
    float mAuxBusSend = 0;
    float mTrackVolume0 = 1.0;
    float mTrackVolume1 = 0;
    float mTrackVolume2 = 0;
    float mTrackVolume3 = 0;
    float mTrackVolume4 = 0;
    float mTrackVolume5 = 0;
};

struct BgmUserInfo;
class ByamlIter;
class SafeString;
class AudioInfoListWithParts;

struct BgmUserInfo {
    static BgmUserInfo* createInfo(const ByamlIter&, const sead::SafeString&);

    BgmUserInfo();

    static int compareInfo(const BgmUserInfo*, const BgmUserInfo*);
    static int compareInfoByKey(const BgmUserInfo*, const char*);

    const char* mName = nullptr;
    AudioInfoListWithParts* mBgmActionInfoList = nullptr;
    AudioInfoListWithParts* mBgmSourceInfoList = nullptr;
};
}  // namespace al
