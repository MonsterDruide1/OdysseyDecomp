#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Play/Layout/SimpleLayoutAppearWaitEnd.h"

class IBalloonFindMyAchievementHolder;

namespace al {
class LayoutInitInfo;
}  // namespace al

namespace TimeBalloon {
class BalloonDataServer {
public:
    BalloonDataServer();
    virtual ~BalloonDataServer();

    void setNoticeDataId(u64 noticeDataId) { mNoticeDataId = noticeDataId; }

private:
    s32 mDataTypeCurrentWorldId = 0;
    bool mIsInvalid = false;
    s32 _10 = 0;
    sead::Vector3f mBalloonPos = sead::Vector3f::zero;
    u64 mNoticeDataId = 0;
    u64 _28 = 0;
    u64 _30 = 0;
    u64 _38 = 0;
    u64 _40 = 0;
    u64 _48 = 0;
    void* _50 = nullptr;
    void* _58 = nullptr;
};

static_assert(sizeof(BalloonDataServer) == 0x60);
}  // namespace TimeBalloon

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
