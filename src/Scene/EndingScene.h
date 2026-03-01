#pragma once

#include "Library/Scene/Scene.h"

namespace al {
class StageInfo;
class WipeHolder;
}  // namespace al

class EndingScene : public al::Scene {
public:
    EndingScene(al::WipeHolder* wipeHolder = nullptr);
    ~EndingScene() override;

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
    s8 filler_d8[0x190 - sizeof(al::Scene)];
};
