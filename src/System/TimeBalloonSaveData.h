#pragma once

#include <basis/seadTypes.h>

#include "System/ByamlSave.h"

namespace al {
class ByamlIter;
class ByamlWriter;
}  // namespace al

class TimeBalloonSaveData : public ByamlSave {
public:
    TimeBalloonSaveData();
    void init();
    void write(al::ByamlWriter* writer) override;
    void read(const al::ByamlIter& save) override;

    bool isExplainedTutorial() const { return mIsExplainedTutorial; }

    void setExplainedTutorial() { mIsExplainedTutorial = true; }

    bool isReactedLuigiCostume() const { return mIsReactedLuigiCostume; }

    void setReactedLuigiCostume() { mIsReactedLuigiCostume = true; }

    bool isShortExplainGet() const { return mIsShortExplainGet; }

    void setShortExplainGet() { mIsShortExplainGet = true; }

    bool isShortExplainSet() const { return mIsShortExplainSet; }

    void setShortExplainSet() { mIsShortExplainSet = true; }

    s32 getPlayerRank() const { return mPlayerRank; }

    void setPlayerRank(s32 rank) { mPlayerRank = rank; }

    u64 getPrincipalId() const { return mPrincipalId; }

    void setPrincipalId(u64 id) { mPrincipalId = id; }

private:
    bool mIsExplainedTutorial;
    bool mIsReactedLuigiCostume;
    u64 mPrincipalId;
    s32 mPlayerRank;
    bool mIsShortExplainGet;
    bool mIsShortExplainSet;
};

static_assert(sizeof(TimeBalloonSaveData) == 0x20);
