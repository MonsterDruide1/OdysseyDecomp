#pragma once

#include <basis/seadTypes.h>

#include "Library/Data/IUseDataStore.h"
#include "Library/Nerve/NerveExecutor.h"

namespace al {
class DataStoreDirector;
}

class BalloonFindSearchBalloonUpdater : public al::IUseDataStore, public al::NerveExecutor {
public:
    BalloonFindSearchBalloonUpdater(al::DataStoreDirector* director);

    bool start(u64 dataId, u64 ownerAchievementDataId, bool isFriendBalloon);
    void reset();
    bool isEndSuccess() const;
    bool isEndFailure() const;
    void exeUpdate();

    al::DataStoreDirector* getDataStoreDirector() const override { return mDataStoreDirector; }

    void exeWait() {}

    void exeSuccess() {}

    void exeFailure() {}

private:
    al::DataStoreDirector* mDataStoreDirector = nullptr;
    bool mIsError = false;
};

static_assert(sizeof(BalloonFindSearchBalloonUpdater) == 0x28);
