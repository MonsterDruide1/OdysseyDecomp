#pragma once

#include "al/scene/Scene.h"
#include "game/StageScene/StageSceneLayout.h"

#define INHERITSIZE sizeof(al::Scene)

class StageScene : public al::Scene
{
    public:
        StageScene();

        virtual ~StageScene();
        virtual void init(const al::SceneInitInfo &);
        virtual void appear();
        virtual void kill();
        
        virtual void control();
        virtual void drawMain();

        bool isEnableSave(void) const;

        // 0xE0 stageName

        unsigned char padding_2D0[0x2D0 - INHERITSIZE];
        GameDataHolderAccessor *mHolder; // 0x2D0
        unsigned char padding_2F8[0x20];
        StageSceneLayout *stageSceneLayout; // 0x2F8
};
