#pragma once

#include <basis/seadTypes.h>

namespace al {

// TODO: Rename a, b and c arguments

u8* getSaveDataWorkBuffer();
bool isInitializedSaveData();
bool requestInitSaveDir(const char* name, u32 a, u32 b);
bool requestFormatSaveData(s32 a, s32 b);
bool requestReadSaveData(const char* name, u32 a, u32 b);
bool requestWriteSaveData(const char* name, u32 a, u32 b, bool c);
bool requestFlushSaveData();
bool initSaveDirSync(const char* name, u32 a, u32 b);
bool formatSaveDataSync(s32 a, s32 b);
bool readSaveDataSync(const char* name, u32 a, u32 b);
bool writeSaveDataSync(const char* name, u32 a, u32 b);
void copyReadSaveDataFromBuffer(void* buffer, u32 size);
void copyWriteSaveDataToBuffer(const void* buffer, u32 size);
bool updateSaveDataSequence();
bool isSuccessSaveDataSequence();
bool isDoneSaveDataSequence();
bool isCorruptedSaveDataSequenceRead();
bool isCorruptedSaveDataSequenceResult();
s32 getSaveDataSequenceResult();
s32 getLastSaveDataFSErrorCode();

}  // namespace al
