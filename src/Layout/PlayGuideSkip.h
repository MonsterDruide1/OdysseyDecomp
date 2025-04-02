#pragma once

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
}

class PlayGuideSkip : public al::LayoutActor {
public:
    PlayGuideSkip(const char* name, const al::LayoutInitInfo& info);

    void kill() override;
    bool tryAppear();
    void appearCore();
    void end();
    bool isEnableSkipDemo() const;

    void exeAppear();
    void exeWait();
    void exeEnd();
};
