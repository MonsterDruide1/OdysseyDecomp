#pragma once

namespace al {
class AudioDirector;
class BgmDataBase;
struct BgmUserInfo;
}  // namespace al

namespace alBgmFunction {
al::BgmUserInfo* tryFindBgmUserInfo(const al::BgmDataBase*, const char*);
}
