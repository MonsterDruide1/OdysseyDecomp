#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class Scene;
}

class StageSceneStateSkipDemo : public al::NerveStateBase {
public:
    void tryAppearSkipDemoLayout();
    bool tryStartSkipDemo();
    bool isCancelSkip() const;
};
