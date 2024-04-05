#pragma once

namespace al {
class AudioDirector;
class BgmDataBase;
struct BgmUserInfo;
}  // namespace al

struct alBgmFunction {
    static al::BgmUserInfo* tryFindBgmUserInfo(const al::BgmDataBase*, const char*);
};
