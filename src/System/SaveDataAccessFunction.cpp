#include "System/SaveDataAccessFunction.h"

#include "System/GameDataHolder.h"
#include "System/SaveDataAccessSequence.h"

void SaveDataAccessFunction::startSaveDataInit(GameDataHolder* holder) {
    holder->mSaveDataAccessSequence->startSaveDataInit();
}

void SaveDataAccessFunction::startSaveDataInitSync(GameDataHolder* holder) {
    holder->mSaveDataAccessSequence->startSaveDataInitSync();
}

void SaveDataAccessFunction::startSaveDataLoadFile(GameDataHolder* holder) {
    holder->mSaveDataAccessSequence->setWindowSwitch();
    holder->mSaveDataAccessSequence->startSaveDataLoadFile();
}

void SaveDataAccessFunction::startSaveDataReadSync(GameDataHolder* holder) {
    holder->mSaveDataAccessSequence->startSaveDataReadSync();
}

void SaveDataAccessFunction::startSaveDataReadAll(GameDataHolder* holder) {
    holder->mSaveDataAccessSequence->startSaveDataReadAll();
}

void SaveDataAccessFunction::startSaveDataWrite(GameDataHolder* holder) {
    holder->mSaveDataAccessSequence->startSaveDataWrite();
}

void SaveDataAccessFunction::startSaveDataWriteWithWindow(GameDataHolder* holder) {
    holder->mSaveDataAccessSequence->setWindowSave();
    holder->mSaveDataAccessSequence->startSaveDataWriteWithWindow();
}

void SaveDataAccessFunction::startSaveDataCopyWithWindow(GameDataHolder* holder, s32 srcFileIndex,
                                                         s32 destFileIndex) {
    holder->mSaveDataAccessSequence->setWindowSave();
    holder->mSaveDataAccessSequence->startSaveDataCopyWithWindow(srcFileIndex, destFileIndex);
}

void SaveDataAccessFunction::startSaveDataDeleteWithWindow(GameDataHolder* holder, s32 fileIndex) {
    holder->mSaveDataAccessSequence->setWindowDelete();
    holder->mSaveDataAccessSequence->startSaveDataDeleteWithWindow(fileIndex);
}

void SaveDataAccessFunction::startSaveDataWriteSync(GameDataHolder* holder) {
    holder->mSaveDataAccessSequence->startSaveDataWriteSync();
}

bool SaveDataAccessFunction::updateSaveDataAccess(GameDataHolder* holder, bool isForce) {
    return holder->mSaveDataAccessSequence->updateSaveDataAccess();
}

bool SaveDataAccessFunction::isEnableSave(const GameDataHolder* holder) {
    return holder->mSaveDataAccessSequence->isEnableSave();
}

bool SaveDataAccessFunction::isDoneSave(GameDataHolder* holder) {
    return holder->mSaveDataAccessSequence->isDoneSave();
}
