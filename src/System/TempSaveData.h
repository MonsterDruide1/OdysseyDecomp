#pragma once

#include <basis/seadTypes.h>

namespace al {
class PlacementId;
}  // namespace al

class TempSaveData {
public:
    TempSaveData();

    void init();
    void initForScenario();
    void resetMiniGame();
    void setInfo(s32, s32);
    void writeInWorld(const al::PlacementId*, const char*);
    void deleteInWorld(const al::PlacementId*, const char*);
    bool isOnInWorld(const al::PlacementId*, const char*) const;
    void writeInWorldResetMiniGame(const al::PlacementId*, const char*);
    void deleteInWorldResetMiniGame(const al::PlacementId*, const char*);
    bool isOnInWorldResetMiniGame(const al::PlacementId*, const char*) const;
    void writeInScenario(const al::PlacementId*, const char*);
    bool isOnInScenario(const al::PlacementId*, const char*) const;
    void writeHashInWorld(const char*, bool);
    bool findHashValueInWorld(const char*) const;

    s32 getWorldIndex() { return mWorldIndex; }

private:
    char filler[0x18];
    s32 mWorldIndex;
    char filler_1c[0x24];
};

static_assert(sizeof(TempSaveData) == 0x40);
