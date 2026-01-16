#include "Library/SaveData/SaveDataFunction.h"

#include "Library/System/SystemKit.h"
#include "Project/Memory/Util.h"
#include "Project/SaveData/SaveDataDirector.h"
#include "Project/SaveData/SaveDataSequenceRead.h"

#include "System/ProjectInterface.h"

namespace al {
inline SaveDataDirector* getSaveDataDirector() {
    return alProjectInterface::getSystemKit()->getSaveDataDirector();
}

u8* getSaveDataWorkBuffer() {
    return getSaveDataDirector()->getWorkBuffer();
}

bool isInitializedSaveData() {
    return getSaveDataDirector()->isInitialized();
}

bool requestInitSaveDir(const char* fileName, u32 dirSize, u32 version) {
    return getSaveDataDirector()->requestInitSaveDir(fileName, dirSize, version);
}

bool requestFormatSaveData(s32 a, s32 b) {
    return getSaveDataDirector()->requestFormat(a, b);
}

bool requestReadSaveData(const char* fileName, u32 readSize, u32 version) {
    return getSaveDataDirector()->requestRead(fileName, readSize, version);
}

bool requestWriteSaveData(const char* fileName, u32 writeSize, u32 version, bool isFlushNeeded) {
    return getSaveDataDirector()->requestWrite(fileName, writeSize, version, isFlushNeeded);
}

bool requestFlushSaveData() {
    return getSaveDataDirector()->requestFlush();
}

bool initSaveDirSync(const char* fileName, u32 dirSize, u32 version) {
    return getSaveDataDirector()->initSaveDirSync(fileName, dirSize, version);
}

bool formatSaveDataSync(s32 a, s32 b) {
    return getSaveDataDirector()->formatSync(a, b);
}

bool readSaveDataSync(const char* fileName, u32 readSize, u32 version) {
    return getSaveDataDirector()->readSync(fileName, readSize, version);
}

bool writeSaveDataSync(const char* fileName, u32 writeSize, u32 isFlushNeeded) {
    return getSaveDataDirector()->writeSync(fileName, writeSize, isFlushNeeded);
}

void copyReadSaveDataFromBuffer(void* buffer, u32 size) {
    u8* workBuffer = getSaveDataDirector()->getWorkBuffer();
    copyMemory(buffer, workBuffer, size);
}

void copyWriteSaveDataToBuffer(const void* buffer, u32 size) {
    u8* workBuffer = getSaveDataDirector()->getWorkBuffer();
    copyMemory(workBuffer, buffer, size);
}

bool updateSaveDataSequence() {
    return getSaveDataDirector()->updateSequence();
}

bool isSuccessSaveDataSequence() {
    return getSaveDataSequenceResult() == 0;
}

bool isDoneSaveDataSequence() {
    return getSaveDataDirector()->isDoneSequence();
}

bool isCorruptedSaveDataSequenceRead() {
    return getSaveDataDirector()->getReadSequence()->isCorrupted();
}

bool isCorruptedSaveDataSequenceResult() {
    return false;
}

s32 getSaveDataSequenceResult() {
    return getSaveDataDirector()->getResult();
}

s32 getLastSaveDataFSErrorCode() {
    return getSaveDataDirector()->getFSErrorCode();
}

}  // namespace al
