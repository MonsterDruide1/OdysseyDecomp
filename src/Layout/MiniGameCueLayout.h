#pragma once

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
}

class MiniGameCueLayout : public al::LayoutActor {
public:
    MiniGameCueLayout(const char* name, const al::LayoutInitInfo& info);

    void appearMiss();
    void appearCount(s32 count);
    void appearGo();
    void appearFinish();
    void exeWait();
    void exeAppear();
};
