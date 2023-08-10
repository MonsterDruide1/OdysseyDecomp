#pragma once

#include "al/Library/Nerve/NerveExecutor.h"
#include "al/Library/Nerve/NerveSetupUtil.h"
#include "al/Library/System/GameSystemInfo.h"

namespace al {
class Sequence;
}

class GameConfigData;

class GameSystem : public al::NerveExecutor {
public:
    GameSystem();
    virtual void init();
    virtual void movement();
    void setPadName();
    bool tryChangeSequence(const char*);
    void drawMain();

    void exePlay();

private:
    al::Sequence* mSequence;
    al::GameSystemInfo mSystemInfo;
    GameConfigData* mConfig;
    bool field_70;
};
