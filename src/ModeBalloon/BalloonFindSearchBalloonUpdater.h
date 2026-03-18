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
    ~BalloonFindSearchBalloonUpdater() override = default;

    bool start(u64 dataId, u64 ownerAchievementDataId, bool isFriendBalloon);
    void reset();
    bool isEndSuccess() const;
    bool isEndFailure() const;
    void exeUpdate();
    al::DataStoreDirector* getDataStoreDirector() const override;

    void exeWait() {}

    void exeEndSuccess() {}

    void exeEndFailure() {}

private:
    al::DataStoreDirector* mDataStoreDirector = nullptr;
    bool mIsError = false;
};
