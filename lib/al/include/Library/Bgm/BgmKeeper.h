#pragma once

namespace al {
class AudioSystemInfo;
class BgmDirector;
struct BgmUserInfo;

class BgmKeeper {
public:
    BgmKeeper(const AudioSystemInfo*, BgmDirector*, const char*);
    
    static BgmKeeper* create(const AudioSystemInfo*, BgmDirector*, const char*);
    BgmUserInfo* getUserName() const;
    void update();

private:
    BgmDirector* mBgmDirector;
    BgmUserInfo* mBgmUserInfo;
};
}
