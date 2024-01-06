#pragma once

#include "Library/Scene/Scene.h"

#include "Layout/StageSceneLayout.h"

class PlayerActorHakoniwa;

class StageScene : public al::Scene {
public:
    StageScene();

    virtual ~StageScene();
    virtual void init(const al::SceneInitInfo&);
    virtual void appear();
    virtual void kill();

    virtual void control();
    virtual void drawMain();

    bool isEnableSave(void) const;

    // somewhere here at 0xE0: stageName

    unsigned char padding_to_2D0[0x1F8];
    GameDataHolderAccessor* mHolder;
    unsigned char padding_2F8[0x20];
    StageSceneLayout* stageSceneLayout;
};

namespace rs {
PlayerActorHakoniwa* getPlayerActor(const al::Scene*);
}
