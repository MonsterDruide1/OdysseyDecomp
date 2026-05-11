#pragma once

#include "Scene/DemoScene.h"

class CaptionInfoHolder;
class CinemaCaption;

namespace al {
class LayoutInitInfo;
class WipeHolder;
}  // namespace al

class DemoSceneWithCinemaCaption : public DemoScene {
public:
    DemoSceneWithCinemaCaption(al::WipeHolder* wipeHolder);

    void init(const al::SceneInitInfo& info) override;
    void exePlay() override;
    void initLayout(const al::LayoutInitInfo& layoutInitInfo) override;

private:
    CinemaCaption* mCinemaCaption = nullptr;
    CaptionInfoHolder* mCaptionInfoHolder = nullptr;
};

static_assert(sizeof(DemoSceneWithCinemaCaption) == 0x1a8);
