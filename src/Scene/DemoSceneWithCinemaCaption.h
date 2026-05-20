#pragma once

#include "Scene/DemoScene.h"

class DemoSceneWithCinemaCaption : public DemoScene {
public:
    DemoSceneWithCinemaCaption(al::WipeHolder* wipeHolder = nullptr);
    ~DemoSceneWithCinemaCaption() override;

    void init(const al::SceneInitInfo& info) override;
    void exePlay() override;
    void initLayout(const al::LayoutInitInfo&) override;

private:
    s8 filler_198[0x1A8 - sizeof(DemoScene)];
};
