#pragma once

#include "types.h"

#include "al/scene/Scene.h"
#include "al/audio/AudioDirector.h"
#include "game/StageScene/StageScene.h"
#include "game/WorldList/WorldResourceLoader.h"
#include "game/GameData/GameDataHolderAccessor.h"

class HakoniwaStateDemoWorldWarp;

namespace al
{
    class WipeHolder;
    class SequenceInitInfo;
} // namespace al

// generated with Ghidra
class HakoniwaSequence {
    public:

        HakoniwaSequence(const char *);
        bool isDisposable(void);
        void updatePadSystem(void);
        void destroySceneHeap(bool);
        void init(al::SequenceInitInfo const &);
        void initSystem(void);
        void update(void);
        bool isEnableSave(void);
        void drawMain(void);
        al::Scene *getCurrentScene(void); // {return this->curScene}

        undefined * * field_0x0;
        undefined padding_120[120];
                            al::Scene * curScene;
        undefined padding_8[8];
                            al::AudioDirector * field_0x90;
        undefined padding_24[24];
                            StageScene * stageScene;
                            GameDataHolderAccessor *gameDataHolder;
        undefined padding_024[24];
                            HakoniwaStateDemoWorldWarp * stateDemoWorldWarp;
        undefined padding_192[192];
                            int nextScenarioNo;
        undefined padding_12[12];
                            al::WipeHolder * field_0x1b0;
        undefined padding_0024[24];
                            long * field_0x1d0;
        undefined padding_48[48];
                            WorldResourceLoader * worldResourceLoader;
        undefined padding_0x16[16];
        undefined * field_0x220;
        undefined padding_0x144[144];
        undefined * field_0x2b8;
        undefined padding_0x160[160];
        undefined8 field_0x360;
};