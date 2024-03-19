#pragma once

namespace al {
class AudioDirector;
struct BgmUserInfo;
}  // namespace al

class alBgmFunction {
public:
    static al::BgmUserInfo* tryFindBgmUserInfo(const al::BgmDataBase*, const char*);
};
