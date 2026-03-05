#include "System/SaveDataAccessFunction.h"

#include "System/GameDataHolder.h"
#include "System/SaveDataAccessSequence.h"

namespace SaveDataAccessFunction {

void startSaveDataInit(GameDataHolder* holder) {
    holder->getSaveDataAccessSequence()->startInit();
}

void startSaveDataInitSync(GameDataHolder* holder) {
    holder->getSaveDataAccessSequence()->startInitSync();
}

void startSaveDataLoadFile(GameDataHolder* holder) {
    holder->getSaveDataAccessSequence()->setWindowSwitch();
    holder->getSaveDataAccessSequence()->startWrite();
}

void startSaveDataReadSync(GameDataHolder* holder) {
    holder->getSaveDataAccessSequence()->startReadSync();
}

void startSaveDataReadAll(GameDataHolder* holder) {
    holder->getSaveDataAccessSequence()->startReadAll();
}

void startSaveDataWrite(GameDataHolder* holder) {
    holder->getSaveDataAccessSequence()->startWrite();
}

void startSaveDataWriteWithWindow(GameDataHolder* holder) {
    holder->getSaveDataAccessSequence()->setWindowSave();
    holder->getSaveDataAccessSequence()->startWriteWithWindow();
}

void startSaveDataCopyWithWindow(GameDataHolder* holder, s32 srcFileIndex, s32 destFileIndex) {
    holder->getSaveDataAccessSequence()->setWindowSave();
    holder->getSaveDataAccessSequence()->startCopyWithWindow(srcFileIndex, destFileIndex);
}

void startSaveDataDeleteWithWindow(GameDataHolder* holder, s32 fileIndex) {
    holder->getSaveDataAccessSequence()->setWindowDelete();
    holder->getSaveDataAccessSequence()->startDeleteWithWindow(fileIndex);
}

void startSaveDataWriteSync(GameDataHolder* holder) {
    holder->getSaveDataAccessSequence()->startWriteSync();
}

bool updateSaveDataAccess(GameDataHolder* holder, bool) {
    return holder->getSaveDataAccessSequence()->update();
}

bool isEnableSave(const GameDataHolder* holder) {
    return holder->getSaveDataAccessSequence()->isEnableSave();
}

bool isDoneSave(GameDataHolder* holder) {
    return holder->getSaveDataAccessSequence()->isDoneSave();
}

}  // namespace SaveDataAccessFunction
