#pragma once

namespace al {
class AudioDirector;
class BgmDataBase;
struct BgmPlayInfo;
struct BgmResourceInfo;
struct BgmStageInfo;
struct BgmSituationInfo;
struct BgmUserInfo;
}  // namespace al

namespace alBgmFunction {
al::BgmPlayInfo* tryFindBgmPlayInfo(const al::BgmDataBase*, const char*);
al::BgmResourceInfo* tryFindBgmResourceInfo(const al::BgmDataBase*, const char*);
al::BgmStageInfo* tryFindBgmStageInfo(const al::BgmDataBase*, const char*);
al::BgmSituationInfo* tryFindBgmSituationInfo(const al::BgmDataBase*, const char*);
al::BgmUserInfo* tryFindBgmUserInfo(const al::BgmDataBase*, const char*);
}  // namespace alBgmFunction
