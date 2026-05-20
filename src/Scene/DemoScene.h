#pragma once

#include "Library/Scene/Scene.h"

namespace al {
class WipeHolder;
class StageInfo;
}  // namespace al

class DemoScene : public al::Scene {
public:
    DemoScene(al::WipeHolder* wipeHolder = nullptr);
    ~DemoScene() override;

    void init(const al::SceneInitInfo& info) override;
    void initPlacement(const al::ActorInitInfo&);
    void appear() override;
    void control() override;
    void kill() override;
    void drawMain() const override;
    virtual void exePlay();
    bool isLoadEnd() const;
    void exeSkipProc();
    virtual void initLayout(const al::LayoutInitInfo&);

    void initPlacementSky(const al::StageInfo*, const al::ActorInitInfo&);
    void initPlacementDemo(const al::StageInfo*, const al::ActorInitInfo&);
    void initPlacementObject(const al::StageInfo*, const al::ActorInitInfo&, const char*);

private:
    s8 filler_d8[0x198 - sizeof(al::Scene)];
};
