#pragma once

#include <basis/seadTypes.h>

namespace al {

u8* getSaveDataWorkBuffer();
bool isInitializedSaveData();
bool requestInitSaveDir(const char*, u32, u32);
bool requestFormatSaveData(s32, s32);
bool requestReadSaveData(const char*, u32, u32);
bool requestWriteSaveData(const char*, u32, u32, bool);
bool requestFlushSaveData();
bool initSaveDirSync(const char*, u32, u32);
bool formatSaveDataSync(s32, s32);
bool readSaveDataSync(const char*, u32, u32);
bool writeSaveDataSync(const char*, u32, u32);
void copyReadSaveDataFromBuffer(void*, u32);
void copyWriteSaveDataToBuffer(void const*, u32);
bool updateSaveDataSequence();
bool isSuccessSaveDataSequence();
bool isDoneSaveDataSequence();
bool isCorruptedSaveDataSequenceRead();
bool isCorruptedSaveDataSequenceResult();
s32 getSaveDataSequenceResult();
s32 getLastSaveDataFSErrorCode();

}  // namespace al
