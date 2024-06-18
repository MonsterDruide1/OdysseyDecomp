#pragma once

#include <basis/seadTypes.h>

class HakoniwaStateDemoWorldWarp;
class WorldResourceLoader;
class StageScene;
class GameDataHolderAccessor;

namespace al {
class WipeHolder;
class SequenceInitInfo;
class Scene;
class AudioDirector;
}  // namespace al

// generated with Ghidra
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
    al::Scene* getCurrentScene();  // {return curScene}

    u8** field_0x0;
    u8 padding_120[120];
    al::Scene* curScene;
    u8 padding_8[8];
    al::AudioDirector* field_0x90;
    u8 padding_24[24];
    StageScene* stageScene;
    GameDataHolderAccessor* gameDataHolder;
    u8 padding_024[24];
    HakoniwaStateDemoWorldWarp* stateDemoWorldWarp;
    u8 padding_192[192];
    s32 nextScenarioNo;
    u8 padding_12[12];
    al::WipeHolder* field_0x1b0;
    u8 padding_0024[24];
    s64* field_0x1d0;
    u8 padding_48[48];
    WorldResourceLoader* worldResourceLoader;
    u8 padding_0x16[16];
    u8* field_0x220;
    u8 padding_0x144[144];
    u8* field_0x2b8;
    u8 padding_0x160[160];
    u8 field_0x360;
};
