#include "Library/Debug/Host.h"

#include <devenv/seadEnvUtil.h>
#include <nn/os.h>

namespace al {
s32 getComputerName(sead::BufferedSafeStringBase<char>* computerName) {
    computerName->format("");
    computerName->format("");

    return sead::EnvUtil::getEnvironmentVariable(computerName, "COMPUTERNAME");
}

bool tryGetComputerName(sead::BufferedSafeStringBase<char>* computerName) {
    return getComputerName(computerName) > 0;
}

s32 getUserName(sead::BufferedSafeStringBase<char>* userName) {
    userName->format("");
    userName->format("");

    return sead::EnvUtil::getEnvironmentVariable(userName, "USERNAME");
}

void makeUniqueTemporaryFilename(sead::BufferedSafeStringBase<char>* out, const char* fileName) {
    sead::FixedSafeString<0x80> computerName;
    getComputerName(&computerName);

    nn::os::Tick time = nn::os::GetSystemTick();
    out->format("%s_%012lld%s", computerName.cstr(), time, fileName);
}

StringTmp<128> makeTmpFileFullPath(const char* fileName) {
    return StringTmp<128>("${TEMP}/%012lld%s", nn::os::GetSystemTick(),
                          fileName != nullptr ? fileName : "");
}

char* getALCommon() {
    return (char*)"${AL_TOOL_ROOT}/ALCommon";
}
}  // namespace al
