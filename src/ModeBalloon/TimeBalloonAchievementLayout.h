#pragma once

#include <basis/seadTypes.h>

#include "Library/Play/Layout/SimpleLayoutAppearWaitEnd.h"

class IBalloonFindMyAchievementHolder;

namespace al {
class LayoutInitInfo;
}  // namespace al

class TimeBalloonAchievementLayout : public al::SimpleLayoutAppearWaitEnd {
public:
    TimeBalloonAchievementLayout(const al::LayoutInitInfo& info);

    void appear() override;
    void update();
    void setAchievement(const IBalloonFindMyAchievementHolder* holder);

private:
    u8 _130[0x288 - 0x130];
};

static_assert(sizeof(TimeBalloonAchievementLayout) == 0x288);
