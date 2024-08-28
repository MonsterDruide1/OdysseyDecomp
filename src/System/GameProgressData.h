#pragma once

#include <basis/seadTypes.h>

class GameProgressData {
public:
    s32 getHomeLevel() const;
    void upHomeLevel();
    void talkCapNearHomeInWaterfall();
};
