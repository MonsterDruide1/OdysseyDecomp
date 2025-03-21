#pragma once

#include <basis/seadTypes.h>

class GameDataHolder;

namespace SaveDataAccessFunction {
void startSaveDataInit(GameDataHolder*);
void startSaveDataInitSync(GameDataHolder*);
void startSaveDataLoadFile(GameDataHolder*);
void startSaveDataReadSync(GameDataHolder*);
void startSaveDataReadAll(GameDataHolder*);
void startSaveDataWrite(GameDataHolder*);
void startSaveDataWriteWithWindow(GameDataHolder*);
void startSaveDataCopyWithWindow(GameDataHolder*, s32, s32);
void startSaveDataDeleteWithWindow(GameDataHolder*, s32);
void startSaveDataWriteSync(GameDataHolder*);
bool updateSaveDataAccess(GameDataHolder*, bool);
bool isEnableSave(const GameDataHolder*);
bool isDoneSave(GameDataHolder*);
}  // namespace SaveDataAccessFunction
