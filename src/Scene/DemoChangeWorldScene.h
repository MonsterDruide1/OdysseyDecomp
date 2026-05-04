#pragma once

#include "Library/Scene/Scene.h"

namespace al {
class WipeHolder;
class GamePadSystem;
class StageInfo;
}  // namespace al

class DemoChangeWorldGraphicsDataHolder;

class DemoChangeWorldScene : public al::Scene {
public:
    DemoChangeWorldScene(al::WipeHolder* wipeHolder = nullptr);
    ~DemoChangeWorldScene() override;

    void setGraphicsDataHolder(const DemoChangeWorldGraphicsDataHolder*);
    void init(const al::SceneInitInfo& info) override;
    void initPlacement(const al::GamePadSystem*, const al::ActorInitInfo&);
    u32 decideScenario(s32, s32, s32, s32);
    void appear() override;
    void kill() override;
    void control() override;
    void drawMain() const override;

    void exeFirstDemo();
    void exeDemoBegin();
    bool isTypeOnlyDemoBegin() const;
    void endDemoBegin();
    void exeTalk();
    void exeDemoEnd();
    void requestScreenCoverNerveDemoEnd();
    void exeSkipDemo();

    void initPlacementSky(const al::StageInfo*, const al::ActorInitInfo&);
    void initPlacementObject(const al::StageInfo*, const al::ActorInitInfo&, const char*);

private:
    s8 filler_d8[0x358 - sizeof(al::Scene)];
};
