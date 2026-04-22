#include "ModeBalloon/TimeBalloonDataAccessorState.h"

#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "ModeBalloon/TimeBalloonData.h"
#include "ModeBalloon/TimeBalloonDataAccessor.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"

namespace TimeBalloonDataState {

TimeBalloonDataAccessorExecutorStateBase::TimeBalloonDataAccessorExecutorStateBase(
    TimeBalloonDataAccessorExecutor* executor, TimeBalloonDataAccessor* accessor)
    : al::NerveStateBase(""), mExecutor(executor), mAccessor(accessor) {}

void TimeBalloonDataAccessorExecutorStateBase::appear() {
    al::NerveStateBase::appear();
    mIsError = false;
}

void TimeBalloonDataAccessorExecutorStateBase::errorAndKill() {
    mIsError = true;
    kill();
}

void TimeBalloonDataAccessorExecutorStateBase::errorConnectionAndKill() {
    mIsError = true;
    kill();
}

namespace UpdatePutBalloonOwn {

NERVE_IMPL(State, DeleteNotice)
NERVE_IMPL(State, CreateNotice)
NERVE_IMPL(State, CreateBalloon)

NERVES_MAKE_NOSTRUCT(State, DeleteNotice, CreateNotice, CreateBalloon)

State::State(TimeBalloonDataAccessorExecutor* executor, TimeBalloonDataAccessor* accessor)
    : TimeBalloonDataAccessorExecutorStateBase(executor, accessor) {
    initNerve(&DeleteNotice, 0);
}

void State::appear() {
    TimeBalloonDataAccessorExecutorStateBase::appear();
    al::setNerve(this, &DeleteNotice);
}

void State::setAccessor(al::LiveActor* accessor) {
    mAccessorActor = accessor;
}

void State::setting(TimeBalloon::BalloonDataServer* balloonDataServer, u64 previousDataId,
                    u64 noticeDataId) {
    mBalloonDataServer = balloonDataServer;
    mPreviousDataId = previousDataId;
    mNoticeDataId = noticeDataId;
}

void State::exeDeleteNotice() {
    if (mNoticeDataId == 0) {
        al::setNerve(this, &CreateBalloon);
        return;
    }

    if (al::isFirstStep(this) && !mAccessor->tryDeleteNoticeDataOne(mNoticeDataId)) {
        mIsError = true;
        kill();
        return;
    }

    if (mAccessor->isEnd()) {
        TimeBalloonDataAccessor* accessor = mAccessor;
        GameDataHolderAccessor holderAccessor(mAccessorActor);
        accessor->deleteNotice(GameDataFunction::getCurrentWorldId(holderAccessor) & 0xFFFF);
        al::setNerve(this, &CreateNotice);
    }
}

void State::exeCreateNotice() {
    if (al::isFirstStep(this) && !mAccessor->tryCreateNoticeDataOne()) {
        mIsError = true;
        kill();
        return;
    }

    if (mAccessor->isEnd()) {
        mNoticeDataId = mAccessor->getCreateNoticeDataOneLast();
        mBalloonDataServer->setNoticeDataId(mNoticeDataId);

        TimeBalloonDataAccessor* accessor = mAccessor;
        u64 noticeDataId = mNoticeDataId;
        GameDataHolderAccessor holderAccessor(mAccessorActor);

        accessor->registerNewNotice(noticeDataId,
                                    GameDataFunction::getCurrentWorldId(holderAccessor) & 0xFFFF);
        al::setNerve(this, &CreateBalloon);
    }
}

void State::exeCreateBalloon() {
    if (al::isFirstStep(this))
        mAccessor->tryCreateBalloonDataOne(mBalloonDataServer);

    if (mAccessor->isEnd()) {
        mAccessor->getBalloonDataIdLast();
        kill();
    }
}

}  // namespace UpdatePutBalloonOwn

namespace DownloadAllBalloonDataOwn {

NERVE_IMPL(State, SearchAll)
NERVE_IMPL(State, DownloadBufferQueueAll)

NERVES_MAKE_NOSTRUCT(State, SearchAll, DownloadBufferQueueAll)

State::State(TimeBalloonDataAccessorExecutor* executor, TimeBalloonDataAccessor* accessor)
    : TimeBalloonDataAccessorExecutorStateBase(executor, accessor), mSearchAccessor(accessor) {
    initNerve(&SearchAll, 0);
}

void State::appear() {
    TimeBalloonDataAccessorExecutorStateBase::appear();
    al::setNerve(this, &SearchAll);
}

void State::exeSearchAll() {
    if (al::isFirstStep(this) && !mSearchAccessor->trySearchOwnAll()) {
        mIsError = true;
        kill();
        return;
    }

    if (mSearchAccessor->isEnd()) {
        if (mSearchAccessor->getOwnBalloonCount() >= 1)
            al::setNerve(this, &DownloadBufferQueueAll);
        else
            kill();
    }
}

void State::exeDownloadBufferQueueAll() {
    if (al::isFirstStep(this) && !mSearchAccessor->tryDownloadBalloonDataOwnAllBufferQueue()) {
        mIsError = true;
        kill();
        return;
    }

    if (mSearchAccessor->isEnd())
        kill();
}

}  // namespace DownloadAllBalloonDataOwn

namespace DownloadFindBalloon {

NERVE_IMPL(State, SearchBalloon)
NERVE_IMPL(State, DownloadAchievementAll)

NERVES_MAKE_NOSTRUCT(State, SearchBalloon, DownloadAchievementAll)

State::State(TimeBalloonDataAccessorExecutor* executor, TimeBalloonDataAccessor* accessor)
    : TimeBalloonDataAccessorExecutorStateBase(executor, accessor) {
    initNerve(&SearchBalloon, 0);
}

void State::appear() {
    TimeBalloonDataAccessorExecutorStateBase::appear();
    al::setNerve(this, &SearchBalloon);
}

void State::exeSearchBalloon() {
    if (al::isFirstStep(this) && !mAccessor->trySearchBalloon()) {
        mIsError = true;
        kill();
        return;
    }

    if (mAccessor->isEnd())
        al::setNerve(this, &DownloadAchievementAll);
}

void State::exeDownloadAchievementAll() {
    if (al::isFirstStep(this) && !mAccessor->tryDownloadAchievementOther()) {
        mIsError = true;
        kill();
        return;
    }

    if (mAccessor->isEnd())
        kill();
}

}  // namespace DownloadFindBalloon

namespace RateUpBalloon {

NERVE_IMPL(State, UpdateRating)
NERVE_IMPL(State, UpdateBufferQueuePlayed)

NERVES_MAKE_NOSTRUCT(State, UpdateRating, UpdateBufferQueuePlayed)

State::State(TimeBalloonDataAccessorExecutor* executor, TimeBalloonDataAccessor* accessor)
    : TimeBalloonDataAccessorExecutorStateBase(executor, accessor) {
    initNerve(&UpdateRating, 0);
}

void State::appear() {
    al::NerveStateBase::appear();
    al::setNerve(this, &UpdateRating);
}

void State::setting(u64 dataId, u64 unused, u64 ownerId, bool isInvalidRating) {
    mDataId = dataId;
    _38 = unused;
    mOwnerId = ownerId;
    _48 = isInvalidRating;
}

void State::exeUpdateRating() {
    if (al::isFirstStep(this) && !mAccessor->tryRateUpBalloonRating(mDataId, mOwnerId, false)) {
        mIsError = true;
        kill();
        return;
    }

    if (mAccessor->isEnd())
        al::setNerve(this, &UpdateBufferQueuePlayed);
}

void State::exeUpdateBufferQueuePlayed() {
    if (al::isFirstStep(this) && !mAccessor->tryUpdateBalloonPlayed(mDataId)) {
        mIsError = true;
        kill();
        return;
    }

    if (mAccessor->isEnd())
        kill();
}

}  // namespace RateUpBalloon

namespace UpdateGetBalloon {

NERVE_IMPL(State, UpdateRating)
NERVE_IMPL(State, UpdateNotice)

NERVES_MAKE_NOSTRUCT(State, UpdateRating, UpdateNotice)

State::State(TimeBalloonDataAccessorExecutor* executor, TimeBalloonDataAccessor* accessor)
    : TimeBalloonDataAccessorExecutorStateBase(executor, accessor) {
    initNerve(&UpdateRating, 0);
}

void State::appear() {
    al::NerveStateBase::appear();
    al::setNerve(this, &UpdateRating);
}

void State::setting(u64 dataId, u64 noticeDataId, u64 ownerId, s32 arg4, s32 arg5) {
    mDataId = dataId;
    mNoticeDataId = noticeDataId;
    mOwnerId = ownerId;
    mArg4 = arg4;
    mArg5 = arg5;
}

void State::exeUpdateRating() {
    if (mDataId != 0) {
        if (!al::isFirstStep(this) || mAccessor->tryGotBalloon(mDataId, mOwnerId, mArg4, mArg5)) {
            if (mAccessor->isEnd())
                al::setNerve(this, &UpdateNotice);
            return;
        }
    }

    mIsError = true;
    kill();
}

void State::exeUpdateNotice() {
    if (mNoticeDataId == 0) {
        kill();
        return;
    }

    if (al::isFirstStep(this) && !mAccessor->tryUpdateNoticeGot(mNoticeDataId)) {
        mIsError = true;
        kill();
        return;
    }

    if (mAccessor->isEnd())
        kill();
}

}  // namespace UpdateGetBalloon

namespace UpdateGetBalloonTutorial {

NERVE_IMPL(State, UpdateRating)

NERVES_MAKE_NOSTRUCT(State, UpdateRating)

State::State(TimeBalloonDataAccessorExecutor* executor, TimeBalloonDataAccessor* accessor)
    : TimeBalloonDataAccessorExecutorStateBase(executor, accessor) {
    initNerve(&UpdateRating, 0);
}

void State::appear() {
    al::NerveStateBase::appear();
    al::setNerve(this, &UpdateRating);
}

void State::exeUpdateRating() {
    if (al::isFirstStep(this) && !mAccessor->tryGotBalloonTutorial()) {
        mIsError = true;
        kill();
        return;
    }

    if (mAccessor->isEnd())
        kill();
}

}  // namespace UpdateGetBalloonTutorial

namespace TryCreateAchievementData {

NERVE_IMPL(State, GetDataId)
NERVE_IMPL(State, Create)

NERVES_MAKE_NOSTRUCT(State, GetDataId, Create)

State::State(TimeBalloonDataAccessorExecutor* executor, TimeBalloonDataAccessor* accessor)
    : TimeBalloonDataAccessorExecutorStateBase(executor, accessor) {
    initNerve(&GetDataId, 0);
}

void State::appear() {
    al::NerveStateBase::appear();
    al::setNerve(this, &GetDataId);
}

void State::setAccessor(al::LiveActor* accessor) {
    mAccessorActor = accessor;
}

void State::exeGetDataId() {
    if (al::isFirstStep(this) && !mAccessor->tryGetAndSaveAchievementIfExist()) {
        mIsError = true;
        kill();
        return;
    }

    if (mAccessor->isEnd()) {
        if (mAccessor->isExistAchievementData()) {
            kill();
            return;
        }

        u32 achievementDataId = mAccessor->getAchievementDataId();
        u8 lowByte = achievementDataId;

        if (lowByte == 0) {
            kill();
            return;
        }

        if (!(achievementDataId < 0x100u)) {
            al::setNerve(this, &Create);
            return;
        }

        mIsError = true;
        kill();
    }
}

void State::exeCreate() {
    if (al::isFirstStep(this) && !mAccessor->tryCreateAndSaveAchievement()) {
        mIsError = true;
        kill();
        return;
    }

    if (mAccessor->isEnd())
        kill();
}

}  // namespace TryCreateAchievementData

}  // namespace TimeBalloonDataState
