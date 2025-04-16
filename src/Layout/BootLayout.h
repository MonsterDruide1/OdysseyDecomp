#pragma once

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
}

class BootLayout : public al::LayoutActor {
public:
    BootLayout(const al::LayoutInitInfo& info);

    void appear() override;
    void kill() override;

    void startWipe();
    void endWipe();
    void end();
    void endImmidiate();
    bool isEndWipe() const;
    f32 getBgFrame() const;

    void exeAppear();
    void exeWait();
    void exeStartWipe();
    void exeEndWipe();
    void exeEnd();

private:
    al::LayoutActor* mParBg = nullptr;
};
