#pragma once

#include "Library/Scene/Scene.h"

namespace al {
class StageInfo;
class WipeHolder;
}  // namespace al

class WorldWarpHoleScene : public al::Scene {
public:
    WorldWarpHoleScene(al::WipeHolder* wipeHolder = nullptr);
    ~WorldWarpHoleScene() override;

    void init(const al::SceneInitInfo& info) override;
    void initPlacement(const al::ActorInitInfo&);
    void appear() override;
    void control() override;
    void drawMain() const override;
    bool tryEnd();
    bool isEnableEndLoop() const;

    void exeStart();
    void exeLoop();
    void exeEnd();
    void exeSkipProc();

    void initPlacementSky(const al::StageInfo*, const al::ActorInitInfo&);
    void initPlacementObject(const al::StageInfo*, const al::ActorInitInfo&, const char*);
    void initPlacementDemo(const al::StageInfo*, const al::ActorInitInfo&);

private:
    s8 filler_d8[0x1D0 - sizeof(al::Scene)];
};
