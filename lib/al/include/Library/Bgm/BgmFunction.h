#pragma once

namespace al {
class AudioDirector;
struct BgmUserInfo;
}

class alBgmFunction {
public:
    static al::BgmUserInfo* tryFindBgmUserInfo(const al::BgmDataBase*, const char*);
};  // namespace al
