#include "Project/SaveData/SaveDataSequenceRead.h"

#include <filedevice/nin/seadNinSaveFileDeviceNin.h>
#include <filedevice/seadFileDevice.h>

#include "Project/SaveData/SaveDataFunction.h"

namespace al {

SaveDataSequenceRead::SaveDataSequenceRead(u8 a) : _19(a) {}

inline bool isSaveDataCorrupted(const u8* buffer, u32 version, u32 bufferSize) {
    const SaveDataFunction::SaveDataHeader* header = SaveDataFunction::getSaveDataHeader(buffer);
    if (header->version != version)
        return true;

    if (header->fileSize == bufferSize &&
        header->checkSum != SaveDataFunction::calcSaveDataCheckSum(buffer))
        return true;

    return false;
}

s32 SaveDataSequenceRead::threadFunc(const char* fileName) {
    sead::NinSaveFileDevice fileDevice("save");

    u32 readSize = 0;
    s32 result = read(&fileDevice, fileName, &readSize);

    if (result == 0)
        return result;

    if (isSaveDataCorrupted(mBuffer, mVersion, readSize)) {
        mIsCorrupted = true;
        return -1;
    }

    return result;
}

void SaveDataSequenceRead::start(u8* buffer, u32 bufferSize, u32 version) {
    mBuffer = buffer;
    mBufferSize = bufferSize;
    mVersion = version;
    mIsCorrupted = false;
}

s32 SaveDataSequenceRead::read(sead::FileDevice* fileDevice, const char* fileName, u32* readSize) {
    sead::FileHandle fileHandle;
    if (!fileDevice->tryOpen(&fileHandle, fileName, sead::FileDevice::cFileOpenFlag_ReadOnly, 0))
        return fileDevice->getLastRawError();

    if (!fileDevice->tryRead(readSize, &fileHandle, mBuffer, mBufferSize))
        return 1;  // BUG: N's mistake here. This should be -1

    fileDevice->tryClose(&fileHandle);
    return fileDevice->getLastRawError();
}

}  // namespace al
