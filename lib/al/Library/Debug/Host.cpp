#include "Library/Debug/Host.h"

#include <devenv/seadEnvUtil.h>
#include <nn/os.h>

namespace al {
void getComputerName(sead::BufferedSafeString* computerName) {
    tryGetComputerName(computerName);
}

bool tryGetComputerName(sead::BufferedSafeString* computerName) {
    computerName->format("");
    computerName->format("");

    return sead::EnvUtil::getEnvironmentVariable(computerName, "COMPUTERNAME") > 0;
}

void getUserName(sead::BufferedSafeString* userName) {
    userName->format("");
    userName->format("");

    sead::EnvUtil::getEnvironmentVariable(userName, "USERNAME");
}

void makeUniqueTemporaryFilename(sead::BufferedSafeString* out, const char* fileName) {
    StringTmp<128> computerName;
    getComputerName(&computerName);

    nn::os::Tick time = nn::os::GetSystemTick();
    out->format("%s_%012lld%s", computerName.cstr(), time, fileName);
}

void expandEnvironmentString(sead::BufferedSafeString* out, const sead::SafeString& envStr) {
    out->clear();

    FUN_710086f65c(out, envStr);
}

// void FUN_710086f65c(sead::BufferedSafeString* out, const sead::SafeString& envStr) {}

StringTmp<128> makeTmpExpandEnvironmentString(const sead::SafeString& envStr) {
    StringTmp<128> tmp;

    expandEnvironmentString(&tmp, envStr);

    return tmp;
}

StringTmp<128> makeTmpFileFullPath(const char* fileName) {
    return StringTmp<128>("${TEMP}/%012lld%s", nn::os::GetSystemTick(),
                          fileName != nullptr ? fileName : "");
}

const char* getALCommon() {
    return "${AL_TOOL_ROOT}/ALCommon";
}
}  // namespace al
