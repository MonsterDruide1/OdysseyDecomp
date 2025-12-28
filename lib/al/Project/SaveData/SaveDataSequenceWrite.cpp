#include "Project/SaveData/SaveDataSequenceWrite.h"

#include <filedevice/nin/seadNinSaveFileDeviceNin.h>
#include <filedevice/seadFileDevice.h>

#include "Project/SaveData/SaveDataFunction.h"

namespace al {
SaveDataSequenceWrite::SaveDataSequenceWrite(u8 a) : _16(a) {}

s32 SaveDataSequenceWrite::threadFunc(const char* fileName) {
    sead::NinSaveFileDevice fileDevice("save");

    if (mIsWriteNeeded) {
        if (!write(&fileDevice, fileName))
            return -1;
    }

    if (mIsFlushNeeded)
        fileDevice.tryCommit();

    return fileDevice.getLastRawError();
}

void SaveDataSequenceWrite::start(u8* buffer, u32 bufferSize, u32 version, bool isFlushNeeded) {
    mIsWriteNeeded = true;
    mBuffer = buffer;
    mBufferSize = bufferSize;
    mIsFlushNeeded = isFlushNeeded;

    SaveDataFunction::SaveDataHeader* header = SaveDataFunction::getSaveDataHeader(buffer);
    header->version = version;
    header->fileSize = bufferSize;
    header->checkSum = SaveDataFunction::calcSaveDataCheckSum(buffer);
}

void SaveDataSequenceWrite::startFlushOnly() {
    mBuffer = nullptr;
    mBufferSize = 0;
    mIsWriteNeeded = false;
    mIsFlushNeeded = true;
}

bool SaveDataSequenceWrite::write(sead::FileDevice* fileDevice, const char* fileName) {
    sead::FileHandle fileHandle;
    if (!fileDevice->tryOpen(&fileHandle, fileName, sead::FileDevice::cFileOpenFlag_WriteOnly, 0))
        return false;

    u32 writeSize = 0;
    if (!fileDevice->tryWrite(&writeSize, &fileHandle, mBuffer, mBufferSize))
        return false;

    if (writeSize != mBufferSize)
        return false;

    return fileDevice->tryClose(&fileHandle);
}

}  // namespace al
