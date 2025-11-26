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

bool requestInitSaveDir(const char* name, u32 a, u32 b) {
    return getSaveDataDirector()->requestInitSaveDir(name, a, b);
}

bool requestFormatSaveData(s32 a, s32 b) {
    return getSaveDataDirector()->requestFormat(a, b);
}

bool requestReadSaveData(const char* name, u32 a, u32 b) {
    return getSaveDataDirector()->requestRead(name, a, b);
}

bool requestWriteSaveData(const char* name, u32 a, u32 b, bool c) {
    return getSaveDataDirector()->requestWrite(name, a, b, c);
}

bool requestFlushSaveData() {
    return getSaveDataDirector()->requestFlush();
}

bool initSaveDirSync(const char* name, u32 b, u32 c) {
    return getSaveDataDirector()->initSaveDirSync(name, b, c);
}

bool formatSaveDataSync(s32 a, s32 b) {
    return getSaveDataDirector()->formatSync(a, b);
}

bool readSaveDataSync(const char* name, u32 a, u32 b) {
    return getSaveDataDirector()->readSync(name, a, b);
}

bool writeSaveDataSync(const char* name, u32 a, u32 b) {
    return getSaveDataDirector()->writeSync(name, a, b);
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
    return alProjectInterface::getSystemKit()
        ->getSaveDataDirector()
        ->getReadSequence()
        ->isCorrupted();
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
