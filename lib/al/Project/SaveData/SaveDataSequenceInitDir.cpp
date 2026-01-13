#include "Project/SaveData/SaveDataSequenceInitDir.h"

#include <nn/account.h>
#include <nn/fs/fs_save.h>

namespace al {

SaveDataSequenceInitDir::SaveDataSequenceInitDir(u8 a) : _18(a) {}

s32 SaveDataSequenceInitDir::threadFunc(const char* filename) {
    nn::account::Uid uid;
    nn::account::GetLastOpenedUser(&uid);

    nn::fs::EnsureSaveData(uid);
    nn::fs::MountSaveData("save", uid);
    return 0;
}

void SaveDataSequenceInitDir::start(u8* buffer, u32 bufferSize, u32 version) {
    mBuffer = buffer;
    mBufferSize = bufferSize;
    mVersion = version;
}

}  // namespace al
