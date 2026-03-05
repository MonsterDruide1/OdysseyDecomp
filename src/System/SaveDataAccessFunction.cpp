#include "System/SaveDataAccessFunction.h"

#include "System/GameDataHolder.h"
#include "System/SaveDataAccessSequence.h"

namespace SaveDataAccessFunction {
void startSaveDataInit(GameDataHolder* holder) {
    holder->getSaveDataAccessSequence()->startSaveDataInit();
}

void startSaveDataInitSync(GameDataHolder* holder) {
    holder->getSaveDataAccessSequence()->startSaveDataInitSync();
}

void startSaveDataLoadFile(GameDataHolder* holder) {
    holder->getSaveDataAccessSequence()->setWindowSwitch();
    holder->getSaveDataAccessSequence()->startSaveDataLoadFile();
}

void startSaveDataReadSync(GameDataHolder* holder) {
    holder->getSaveDataAccessSequence()->startSaveDataReadSync();
}

void startSaveDataReadAll(GameDataHolder* holder) {
    holder->getSaveDataAccessSequence()->startSaveDataReadAll();
}

void startSaveDataWrite(GameDataHolder* holder) {
    holder->getSaveDataAccessSequence()->startSaveDataWrite();
}

void startSaveDataWriteWithWindow(GameDataHolder* holder) {
    holder->getSaveDataAccessSequence()->setWindowSave();
    holder->getSaveDataAccessSequence()->startSaveDataWriteWithWindow();
}

void startSaveDataCopyWithWindow(GameDataHolder* holder, s32 srcFileIndex, s32 destFileIndex) {
    holder->getSaveDataAccessSequence()->setWindowSave();
    holder->getSaveDataAccessSequence()->startSaveDataCopyWithWindow(srcFileIndex, destFileIndex);
}

void startSaveDataDeleteWithWindow(GameDataHolder* holder, s32 fileIndex) {
    holder->getSaveDataAccessSequence()->setWindowDelete();
    holder->getSaveDataAccessSequence()->startSaveDataDeleteWithWindow(fileIndex);
}

void startSaveDataWriteSync(GameDataHolder* holder) {
    holder->getSaveDataAccessSequence()->startSaveDataWriteSync();
}

bool updateSaveDataAccess(GameDataHolder* holder, bool isForce) {
    return holder->getSaveDataAccessSequence()->updateSaveDataAccess();
}

bool isEnableSave(const GameDataHolder* holder) {
    return holder->getSaveDataAccessSequence()->isEnableSave();
}

bool isDoneSave(GameDataHolder* holder) {
    return holder->getSaveDataAccessSequence()->isDoneSave();
}
}  // namespace SaveDataAccessFunction
