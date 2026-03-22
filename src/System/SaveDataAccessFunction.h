#pragma once

#include <basis/seadTypes.h>

class GameDataHolder;

namespace SaveDataAccessFunction {
void startSaveDataInit(GameDataHolder* holder);
void startSaveDataInitSync(GameDataHolder* holder);
void startSaveDataLoadFile(GameDataHolder* holder);
void startSaveDataReadSync(GameDataHolder* holder);
void startSaveDataReadAll(GameDataHolder* holder);
void startSaveDataWrite(GameDataHolder* holder);
void startSaveDataWriteWithWindow(GameDataHolder* holder);
void startSaveDataCopyWithWindow(GameDataHolder* holder, s32 srcFileIndex, s32 destFileIndex);
void startSaveDataDeleteWithWindow(GameDataHolder* holder, s32 fileIndex);
void startSaveDataWriteSync(GameDataHolder* holder);
bool updateSaveDataAccess(GameDataHolder* holder, bool _unused);
bool isEnableSave(const GameDataHolder* holder);
bool isDoneSave(GameDataHolder* holder);
}  // namespace SaveDataAccessFunction
