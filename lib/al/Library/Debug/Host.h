#pragma once

#include <prim/seadSafeString.h>

#include "Library/Base/StringUtil.h"

namespace al {
bool tryGetComputerName(sead::BufferedSafeString* computerName);
void getComputerName(sead::BufferedSafeString* computerName);
void getUserName(sead::BufferedSafeString* userName);
void makeUniqueTemporaryFilename(sead::BufferedSafeString* out, const char* fileName);
void expandEnvironmentString(sead::BufferedSafeString* out, const sead::SafeString& envStr);
sead::FixedSafeString<128> makeTmpExpandEnvironmentString(const sead::SafeString& envStr);
StringTmp<128> makeTmpFileFullPath(const char* fileName);
const char* getALCommon();
}  // namespace al
