#pragma once

#include <prim/seadSafeString.h>

namespace al {   

struct BgmUserInfo;
class ByamlIter;
class SafeStringBase;

struct BgmUserInfo {
public:
    BgmUserInfo() = default;

    BgmUserInfo* createInfo(const al::ByamlIter&, const sead::SafeStringBase<char> &);
    static int compareInfo(const BgmUserInfo*, const BgmUserInfo*);
    static int compareInfoByKey(const al::BgmUserInfo*, const char*);
private:
    const char* mName;
};
}