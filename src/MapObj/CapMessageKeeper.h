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
    SEAD_ENUM_EX(CapMsgType, PlayerInIceWaterFirst = 0 , PlayerSinkSandFirst = 1 ,
                 CatchBombCatchFirst = 32 , PackunFireChokeCap = 34 ,
                 PackunFireRescueCap = 35 , WorldWarpOutFirst = 40 ,
                 WorldWarpOutAgain = 60 , CorkNearFirst = 81 , CorkNearLaunchedFirst = 82 ,
                 CorkNearSecond = 83 , WorldWarpLookFirst = 84 , CollectCoinGetFirst = 85 ,
                 ShineChipGetFirst = 88 , KakkuHideFirst = 89 , MoonRockLookFirst = 96 ,
                 MoonRockLookFirstMoonWorld = 97 , MoonRockLookMoonWorld = 98 ,
                 CatupultLookFirst = 99 , CatupultAfterShootFirst = 100 ,
                 GrowFlowerSeedLookFirst = 101 , GrowFlowerPotAfterImplant = 103 ,
                 GrowFlowerPotAfterImplantWater = 104 , MoonGetSpecial1 = 105 ,
                 MoonGetSpecial2 = 106 , IntroducePowerStar = 107 ,
                 WorldWarpCloseLookFirst = 115 , MoonRockLookLongTime = 116 , CapThrow = 250);

    CapMessageKeeper();

    void createAndReadYamlAll();
    bool isShowCapMsg(const al::IUseSceneObjHolder* user, s32 type);
    bool isShowCapMsgCurrentWorld(const al::IUseSceneObjHolder* user, const CapMsgType& type);
    bool tryShowSaveCapMsg(const al::IUseSceneObjHolder* user, const CapMsgType& type, bool low);
    bool tryShowCapMsgPrivate(const al::IUseSceneObjHolder* user, const CapMsgType& type,
                              const char* label, s32 delay, bool isLow, bool isStage);
    bool tryCheckShowCapMsg(const al::IUseSceneObjHolder* user, const CapMsgType& type,
                            CapMessageEnableChecker* checker, bool isLow);
    void saveCapMsg(const al::IUseSceneObjHolder* user, s32 type);
    bool tryShowCapMsgPriorityLow(const al::IUseSceneObjHolder* user, const CapMsgType& type,
                                  s32 delay, s32 waitFrame);
    bool tryReadYamlOne(const al::Resource* resource, const char* label, s32* delay,
                        s32* waitFrame);
    bool tryShowCapMsgCurrentWorld(const al::IUseSceneObjHolder* user, const CapMsgType& type);
    void saveCapMsgCurrentWorld(const al::IUseSceneObjHolder* user, const CapMsgType& type);
    void saveCapMsg(const al::IUseSceneObjHolder* user, const char* label);
    s32* getCapMessageParam(const CapMsgType& type);

private:
    void* mParamList = nullptr;
    s32 mDelay = -1;
    s32 mWaitFrame = -1;
    void* _10 = nullptr;
    s32 _18 = 0;
    u32 _1c = 0;
};

static_assert(sizeof(CapMessageKeeper) == 0x20);

namespace CapMessageKeeperFuncrion {
CapMessageKeeper* getCapMsgKeeper(const al::IUseSceneObjHolder* user);
}
