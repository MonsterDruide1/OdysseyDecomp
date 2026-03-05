#pragma once

#include <basis/seadTypes.h>

class GameDataHolder;

class SaveDataAccessFunction {
public:
    static void startSaveDataInit(GameDataHolder* holder);
    static void startSaveDataInitSync(GameDataHolder* holder);
    static void startSaveDataLoadFile(GameDataHolder* holder);
    static void startSaveDataReadSync(GameDataHolder* holder);
    static void startSaveDataReadAll(GameDataHolder* holder);
    static void startSaveDataWrite(GameDataHolder* holder);
    static void startSaveDataWriteWithWindow(GameDataHolder* holder);
    static void startSaveDataCopyWithWindow(GameDataHolder* holder, s32 srcFileIndex,
                                            s32 destFileIndex);
    static void startSaveDataDeleteWithWindow(GameDataHolder* holder, s32 fileIndex);
    static void startSaveDataWriteSync(GameDataHolder* holder);
    static bool updateSaveDataAccess(GameDataHolder* holder, bool isForce);
    static bool isEnableSave(const GameDataHolder* holder);
    static bool isDoneSave(GameDataHolder* holder);
};
