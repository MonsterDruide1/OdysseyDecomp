#pragma once

#include "Library/Scene/Scene.h"

namespace al {
struct SceneInitInfo;
class WaveVibrationHolder;
}  // namespace al
class SceneStateSelectMode;
class BootLayout;

class FirstSequenceScene : public al::Scene {
public:
    FirstSequenceScene();
    ~FirstSequenceScene();

    void init(const al::SceneInitInfo& info) override;
    void appear() override;
    void control() override;
    void drawMain() const override;

    void setNoWipe();

    void exeWipe();
    void exeSelectMode();

private:
    SceneStateSelectMode* mStateSelectMode = nullptr;
    BootLayout* mBootLayout = nullptr;
    al::WaveVibrationHolder* mWaveVibrationHolder = nullptr;
};

static_assert(sizeof(FirstSequenceScene) == 0xf0);
