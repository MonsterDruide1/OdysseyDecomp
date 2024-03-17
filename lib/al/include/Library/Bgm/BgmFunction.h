#pragma once

namespace al {
class AudioDirector;
struct BgmUserInfo;
}

class alBgmFunction{
public:
    static al::AudioDirector* getBgmPlayInfoList(const al::AudioDirector*);
    static al::AudioDirector* getBgmResourceInfoList(const al::AudioDirector*);
    static al::AudioDirector* getBgmStageInfoList(const al::AudioDirector*);
    static al::AudioDirector* getBgmSituationInfoList(const al::AudioDirector*);
    static al::AudioDirector* getBgmUserInfoList(const al::AudioDirector*);
    static al::BgmUserInfo* tryFindBgmUserInfo(const al::BgmDataBase*, const char*);
};
