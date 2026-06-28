#pragma once

#include <basis/seadTypes.h>
#include <container/seadStrTreeMap.h>

namespace al {
class PlacementId;
}  // namespace al

class UniqObjInfo;

class TempSaveData {
public:
    TempSaveData();

    void init();
    void initForScenario();
    void resetMiniGame();
    void setInfo(s32 worldIndex, s32 unknown);
    void writeInWorld(const al::PlacementId* placementId, const char* stageName);
    void deleteInWorld(const al::PlacementId* placementId, const char* stageName);
    bool isOnInWorld(const al::PlacementId* placementId, const char* stageName) const;
    void writeInWorldResetMiniGame(const al::PlacementId* placementId, const char* stageName);
    void deleteInWorldResetMiniGame(const al::PlacementId* placementId, const char* stageName);
    bool isOnInWorldResetMiniGame(const al::PlacementId* placementId, const char* stageName) const;
    void writeInScenario(const al::PlacementId* placementId, const char* stageName);
    bool isOnInScenario(const al::PlacementId* placementId, const char* stageName) const;
    void writeHashInWorld(const char* hash, bool value);
    bool findHashValueInWorld(const char* hash) const;

    s32 getWorldIndex() const { return mWorldIndex; }

private:
    UniqObjInfo* mWordObjects = nullptr;
    UniqObjInfo* mMiniGameObjects = nullptr;
    UniqObjInfo* mScenarioObjects = nullptr;
    s32 mWorldIndex = -1;
    s32 _1c = -1;
    sead::StrTreeMap<32, bool> mWorldValues;
};

static_assert(sizeof(TempSaveData) == 0x40);
