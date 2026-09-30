#pragma once

#include <basis/seadTypes.h>
#include <prim/seadEnum.h>

namespace al {
class IUseSceneObjHolder;
class Resource;
}  // namespace al

class CapMessageEnableChecker;

class CapMessageKeeper {
public:
    SEAD_ENUM(CapMsgTypeNoSave,
              GrowFlowerPotLookFirst , WorldWarpEnterAlready , WarpDisableInMiniGame ,
              YukimaruTutorial);

    SEAD_ENUM(CapMsgType,
                 PlayerInIceWaterFirst = 0 , PlayerSinkSandFirst = 1 ,
                 CatchBombCatchFirst = 32 , PackunFireChokeCap = 34 ,
                 PackunFireRescueCap = 35 , WorldWarpOutFirst = 40 ,
                 WorldWarpOutAgain = 60 , CorkNearFirst = 81 ,
                 CorkNearLaunchedFirst = 82 , CorkNearSecond = 83 ,
                 WorldWarpLookFirst = 84 , CollectCoinGetFirst = 85 ,
                 ShineChipGetFirst = 88 , KakkuHideFirst = 89 ,
                 MoonRockLookFirst = 96 , MoonRockLookFirstMoonWorld = 97 ,
                 MoonRockLookMoonWorld = 98 , CatupultLookFirst = 99 ,
                 CatupultAfterShootFirst = 100 , GrowFlowerSeedLookFirst = 101 ,
                 GrowFlowerPotAfterImplant = 103 , GrowFlowerPotAfterImplantWater = 104 ,
                 MoonGetSpecial1 = 105 , MoonGetSpecial2 = 106 , IntroducePowerStar = 107 ,
                 WorldWarpCloseLookFirst = 115 , MoonRockLookLongTime = 116 ,
                 CapThrow = 250);

    CapMessageKeeper();

    void createAndReadYamlAll();
    bool isShowCapMsg(const al::IUseSceneObjHolder* sceneObjHolder, s32 id);
    bool isShowCapMsgCurrentWorld(const al::IUseSceneObjHolder* sceneObjHolder,
                                  const CapMsgType& type);
    bool tryShowSaveCapMsg(const al::IUseSceneObjHolder* sceneObjHolder, const CapMsgType& type,
                           bool isSave);
    bool tryShowCapMsgPrivate(const al::IUseSceneObjHolder* sceneObjHolder, const CapMsgType& type,
                              const char* messageName, s32 id, bool isDemo, bool isSave);
    bool tryCheckShowCapMsg(const al::IUseSceneObjHolder* sceneObjHolder, const CapMsgType& type,
                            CapMessageEnableChecker* enableChecker, bool isSave);
    void saveCapMsg(const al::IUseSceneObjHolder* sceneObjHolder, s32 id);
    bool tryShowCapMsgPriorityLow(const al::IUseSceneObjHolder* sceneObjHolder,
                                  const CapMsgType& type, s32 delayTime, s32 waitTime);
    bool tryReadYamlOne(const al::Resource* resource, const char* messageName, s32* delayTime,
                        s32* waitTime);
    bool tryShowCapMsgCurrentWorld(const al::IUseSceneObjHolder* sceneObjHolder,
                                   const CapMsgType& type);
    void saveCapMsgCurrentWorld(const al::IUseSceneObjHolder* sceneObjHolder,
                                const CapMsgType& type);
    void saveCapMsg(const al::IUseSceneObjHolder* sceneObjHolder, const char* messageName);
    s32* getCapMessageParam(const CapMsgType& type);

private:
    s32** mCapMessageParamTable = nullptr;
    s32 mDefaultDelay = -1;
    s32 mDefaultWaitFrame = -1;
    void* mParamArray = nullptr;
    s32 mUnused = 0;
    u32 _1c = 0;
};

static_assert(sizeof(CapMessageKeeper) == 0x20, "CapMessageKeeper");

namespace CapMessageKeeperFuncrion {
CapMessageKeeper* getCapMsgKeeper(const al::IUseSceneObjHolder* sceneObjHolder);
}  // namespace CapMessageKeeperFuncrion
