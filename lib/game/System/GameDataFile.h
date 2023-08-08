/**
 * @file GameDataFile.h
 * @brief Holds data for an individual save file.
 */

#pragma once

#include "GameProgressData.h"
#include "types.h"

class GameDataFile {
public:
    void wearCostume(char const*);
    void wearCap(char const*);

    unsigned char padding_6A8[0x6A8];
    GameProgressData* mGameProgressData;  // 0x6A8
    undefined padding_9F0[0x340];
    int curWorldId;
};