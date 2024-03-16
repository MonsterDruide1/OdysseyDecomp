#pragma once

namespace al {
class AudioSystemInfo;
class BgmDirector;
struct BgmUserInfo;

class BgmKeeper {
public:
    BgmKeeper(const al::AudioSystemInfo*, al::BgmDirector*, const char*);
    
    static BgmKeeper* create(const al::AudioSystemInfo*, al::BgmDirector*, const char*);
    void update();
    const al::BgmUserInfo** getUserName();
private:
    BgmDirector* mBgmDirector;
    BgmUserInfo* mUserName;
};
}