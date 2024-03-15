#pragma once

#include <prim/seadSafeString.h>

namespace al {   
class ByamlIter;
class SafeStringBase;
struct BgmUserInfo;

struct BgmUserInfo {
public:
    BgmUserInfo() = default;

    BgmUserInfo* createInfo(const al::ByamlIter&, const sead::SafeStringBase<char> &);
    int compareInfo(const BgmUserInfo*, const BgmUserInfo*);
    int compareInfoByKey(const al::BgmUserInfo*, const char*);
private:
    const char* mName;
};

}