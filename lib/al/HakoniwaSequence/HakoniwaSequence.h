#pragma once

#include <basis/seadTypes.h>

class HakoniwaStateDemoWorldWarp;

namespace al {
class WipeHolder;
class SequenceInitInfo;
class AudioDirector;
class Scene;
}  // namespace al
class StageScene;
class GameDataHolderAccessor;
class WorldResourceLoader;

class HakoniwaSequence {
public:
    HakoniwaSequence(const char*);
    bool isDisposable();
    void updatePadSystem();
    void destroySceneHeap(bool);
    void init(const al::SequenceInitInfo&);
    void initSystem();
    void update();
    bool isEnableSave();
    void drawMain();
    al::Scene* getCurrentScene();  // {return this->curScene}

    char** field_0x0;
    char padding_120[120];
    al::Scene* curScene;
    char padding_8[8];
    al::AudioDirector* field_0x90;
    char padding_24[24];
    StageScene* stageScene;
    GameDataHolderAccessor* gameDataHolder;
    char padding_024[24];
    HakoniwaStateDemoWorldWarp* stateDemoWorldWarp;
    char padding_192[192];
    int nextScenarioNo;
    char padding_12[12];
    al::WipeHolder* field_0x1b0;
    char padding_0024[24];
    long* field_0x1d0;
    char padding_48[48];
    WorldResourceLoader* worldResourceLoader;
    char padding_0x16[16];
    char* field_0x220;
    char padding_0x144[144];
    char* field_0x2b8;
    char padding_0x160[160];
    u32 field_0x360;
};
