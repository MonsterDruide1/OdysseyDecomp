#pragma once

#include <basis/seadTypes.h>

#include "ModeBalloon/TimeBalloonBalloonDataHolder.h"

namespace TimeBalloon {
class BalloonDataServer;
}

class TimeBalloonDataAccessor {
public:
    bool tryDeleteNoticeDataOne(u64 noticeDataId);
    bool isEnd() const;
    void deleteNotice(s32 worldId);
    bool tryCreateNoticeDataOne();
    u64 getCreateNoticeDataOneLast() const;
    void registerNewNotice(u64 noticeDataId, s32 worldId);
    bool tryCreateBalloonDataOne(const TimeBalloon::BalloonDataServer* balloonDataServer);
    u64 getBalloonDataIdLast() const;
    bool trySearchOwnAll();
    bool tryDownloadBalloonDataOwnAllBufferQueue();
    bool trySearchBalloon();
    bool tryDownloadAchievementOther();
    bool tryRateUpBalloonRating(u64 dataId, u64 ownerId, bool isInvalidRating);
    bool tryUpdateBalloonPlayed(u64 dataId);
    bool tryGotBalloon(u64 dataId, u64 ownerId, s32 arg4, s32 arg5);
    bool tryUpdateNoticeGot(u64 noticeDataId);
    bool tryGotBalloonTutorial();
    bool tryGetAndSaveAchievementIfExist();
    bool isExistAchievementData() const;
    bool tryCreateAndSaveAchievement();

    s32 getOwnBalloonCount() const { return mBalloonDataHolder->getCurrentCount(); }

    u16 getAchievementDataId() const { return mAchievementDataId; }

private:
    u8 _00[0x60];
    TimeBalloonBalloonDataHolder* mBalloonDataHolder = nullptr;
    u8 _68[0x10];
    u16 mAchievementDataId = 0;
    u8 _7a[0x86];
};

static_assert(sizeof(TimeBalloonDataAccessor) == 0x100);
