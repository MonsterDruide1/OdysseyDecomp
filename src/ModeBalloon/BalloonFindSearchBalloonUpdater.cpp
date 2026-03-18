#include "ModeBalloon/BalloonFindSearchBalloonUpdater.h"

#include "Library/Data/DataStoreDirector.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "ModeBalloon/NexRankingUploader.h"

namespace {
NERVE_IMPL(BalloonFindSearchBalloonUpdater, Wait);
NERVE_IMPL(BalloonFindSearchBalloonUpdater, Update);
NERVE_IMPL(BalloonFindSearchBalloonUpdater, EndSuccess);
NERVE_IMPL(BalloonFindSearchBalloonUpdater, EndFailure);
NERVES_MAKE_NOSTRUCT(BalloonFindSearchBalloonUpdater, Wait, Update, EndSuccess, EndFailure);
}  // namespace

BalloonFindSearchBalloonUpdater::BalloonFindSearchBalloonUpdater(al::DataStoreDirector* director)
    : al::NerveExecutor("SearchBalloonUpdater"), mDataStoreDirector(director) {
    initNerve(&Wait, 0);
}

bool BalloonFindSearchBalloonUpdater::start(u64 dataId, u64 ownerAchievementDataId,
                                            bool isFriendBalloon) {
    if (al::isNerve(this, &Update) || !rs::tryStartUpdateRatingPlayedBalloon(
                                          this, dataId, ownerAchievementDataId, isFriendBalloon))
        return false;
    al::setNerve(this, &Update);
    return true;
}

void BalloonFindSearchBalloonUpdater::reset() {
    if (al::tryCancelDataStoreProcess(this))
        al::setNerve(this, &Wait);
}

bool BalloonFindSearchBalloonUpdater::isEndSuccess() const {
    return al::isNerve(this, &EndSuccess);
}

bool BalloonFindSearchBalloonUpdater::isEndFailure() const {
    return al::isNerve(this, &EndFailure);
}

void BalloonFindSearchBalloonUpdater::exeUpdate() {
    if (rs::tryEndUpdateRating(&mIsError, this)) {
        if (mIsError)
            al::setNerve(this, &EndFailure);
        else
            al::setNerve(this, &EndSuccess);
    }
}

al::DataStoreDirector* BalloonFindSearchBalloonUpdater::getDataStoreDirector() const {
    return mDataStoreDirector;
}
