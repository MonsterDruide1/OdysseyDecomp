#pragma once

#include "game/Layouts/CoinCounter.h"

class StageSceneLayout {
    public:
        unsigned char padding_18[0x18];
        CoinCounter *coinCounter; // 0x18
        unsigned char padding_12[0x10];
        CoinCounter *coinCollectCounter; // 0x30
};