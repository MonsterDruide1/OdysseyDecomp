#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LayoutInitInfo;
class SimpleLayoutAppearWaitEnd;
}  // namespace al

class PlayGuideBgm : public al::NerveStateBase {
public:
    PlayGuideBgm(const char* name, const al::LayoutInitInfo& info);

    void start();
    void end();
    void endImmediate();

    void exeHide();
    void exeShow();
    void exeEnd();
    void exeEndForDead();

private:
    al::SimpleLayoutAppearWaitEnd* mLayout = nullptr;
};
