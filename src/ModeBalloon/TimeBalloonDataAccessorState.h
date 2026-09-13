#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}

namespace TimeBalloon {
class BalloonDataServer;
}

class TimeBalloonDataAccessor;
class TimeBalloonDataAccessorExecutor;

namespace TimeBalloonDataState {

class TimeBalloonDataAccessorExecutorStateBase : public al::NerveStateBase {
public:
    TimeBalloonDataAccessorExecutorStateBase(TimeBalloonDataAccessorExecutor* executor,
                                             TimeBalloonDataAccessor* accessor);

    void appear() override;
    void errorAndKill();
    void errorConnectionAndKill();

protected:
    TimeBalloonDataAccessorExecutor* mExecutor;
    TimeBalloonDataAccessor* mAccessor;
    bool mIsError = false;
};

static_assert(sizeof(TimeBalloonDataAccessorExecutorStateBase) == 0x30);

namespace UpdatePutBalloonOwn {
class State : public TimeBalloonDataAccessorExecutorStateBase {
public:
    State(TimeBalloonDataAccessorExecutor* executor, TimeBalloonDataAccessor* accessor);

    void appear() override;
    void setAccessor(al::LiveActor* accessor);
    void setting(TimeBalloon::BalloonDataServer* balloonDataServer, u64 previousDataId,
                 u64 noticeDataId);
    void exeDeleteNotice();
    void exeCreateNotice();
    void exeCreateBalloon();

private:
    al::LiveActor* mAccessorActor = nullptr;
    TimeBalloon::BalloonDataServer* mBalloonDataServer = nullptr;
    u64 mPreviousDataId = 0;
    u64 mNoticeDataId = 0;
};

static_assert(sizeof(State) == 0x50);
}  // namespace UpdatePutBalloonOwn

namespace DownloadAllBalloonDataOwn {
class State : public TimeBalloonDataAccessorExecutorStateBase {
public:
    State(TimeBalloonDataAccessorExecutor* executor, TimeBalloonDataAccessor* accessor);

    void appear() override;
    void exeSearchAll();
    void exeDownloadBufferQueueAll();

private:
    TimeBalloonDataAccessor* mSearchAccessor = nullptr;
};

static_assert(sizeof(State) == 0x38);
}  // namespace DownloadAllBalloonDataOwn

namespace DownloadFindBalloon {
class State : public TimeBalloonDataAccessorExecutorStateBase {
public:
    State(TimeBalloonDataAccessorExecutor* executor, TimeBalloonDataAccessor* accessor);

    void appear() override;
    void exeSearchBalloon();
    void exeDownloadAchievementAll();
};

static_assert(sizeof(State) == 0x30);
}  // namespace DownloadFindBalloon

namespace RateUpBalloon {
class State : public TimeBalloonDataAccessorExecutorStateBase {
public:
    State(TimeBalloonDataAccessorExecutor* executor, TimeBalloonDataAccessor* accessor);

    void appear() override;
    void setting(u64 dataId, u64 unused, u64 ownerId, bool isInvalidRating);
    void exeUpdateRating();
    void exeUpdateBufferQueuePlayed();

private:
    u64 mDataId = 0;
    u64 _38 = 0;
    u64 mOwnerId = 0;
    bool _48 = false;
};

static_assert(sizeof(State) == 0x50);
}  // namespace RateUpBalloon

namespace UpdateGetBalloon {
class State : public TimeBalloonDataAccessorExecutorStateBase {
public:
    State(TimeBalloonDataAccessorExecutor* executor, TimeBalloonDataAccessor* accessor);

    void appear() override;
    void setting(u64 dataId, u64 noticeDataId, u64 ownerId, s32 arg4, s32 arg5);
    void exeUpdateRating();
    void exeUpdateNotice();

    u64 mDataId = 0;
    u64 mNoticeDataId = 0;
    u64 mOwnerId = 0;
    s32 mArg4 = 0;
    s32 mArg5 = 0;
};

static_assert(sizeof(State) == 0x50);
}  // namespace UpdateGetBalloon

namespace UpdateGetBalloonTutorial {
class State : public TimeBalloonDataAccessorExecutorStateBase {
public:
    State(TimeBalloonDataAccessorExecutor* executor, TimeBalloonDataAccessor* accessor);

    void appear() override;
    void exeUpdateRating();
};

static_assert(sizeof(State) == 0x30);
}  // namespace UpdateGetBalloonTutorial

namespace TryCreateAchievementData {
class State : public TimeBalloonDataAccessorExecutorStateBase {
public:
    State(TimeBalloonDataAccessorExecutor* executor, TimeBalloonDataAccessor* accessor);

    void appear() override;
    void setAccessor(al::LiveActor* accessor);
    void exeGetDataId();
    void exeCreate();

    al::LiveActor* mAccessorActor = nullptr;
};

static_assert(sizeof(State) == 0x38);
}  // namespace TryCreateAchievementData

}  // namespace TimeBalloonDataState
