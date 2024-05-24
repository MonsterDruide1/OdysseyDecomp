#pragma once

#include <prim/seadSafeString.h>
#include "Library/Base/String.h"

namespace al {
bool tryGetComputerName(sead::BufferedSafeStringBase<char>* computerName);
s32 getComputerName(sead::BufferedSafeStringBase<char>* computerName);
s32 getUserName(sead::BufferedSafeStringBase<char>* userName);
void makeUniqueTemporaryFilename(sead::BufferedSafeStringBase<char>* out, const char* fileName);
void expandEnvironmentString(
    sead::BufferedSafeStringBase<char>* out,
    const sead::SafeStringBase<char>& envStr);       // TODO: Needs FUN_710086f65c
void* FUN_710086f65c(long* param_1, long* param_2);  // TODO: Find what is this function
void makeTmpExpandEnvironmentString(
    const sead::SafeStringBase<char>& envStr);  // TODO: Needs FUN_710086f65c
StringTmp<128> makeTmpFileFullPath(const char* fileName);
char* getALCommon();
}  // namespace al
