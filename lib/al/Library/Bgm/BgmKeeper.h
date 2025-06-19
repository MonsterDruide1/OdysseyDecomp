#pragma once

#include <prim/seadSafeString.h>

namespace al {
struct AudioSystemInfo;
class BgmDirector;
struct BgmUserInfo;

class BgmKeeper {
public:
    static BgmKeeper* create(const AudioSystemInfo* audioInfo, BgmDirector* director,
                             const char* string);

    BgmKeeper(const AudioSystemInfo* audioInfo, BgmDirector* director, const char* string);

    const char* getUserName() const;
    void update();

private:
    BgmDirector* mBgmDirector;
    BgmUserInfo* mBgmUserInfo = nullptr;
};
}  // namespace al
