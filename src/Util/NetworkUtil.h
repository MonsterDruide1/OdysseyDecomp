#pragma once

#include <prim/seadSafeString.h>

namespace rs {
const char* getNetworkPrototypeDirectory();
void convertComputerNameToFullName(sead::BufferedSafeString* fullName, const char* computerName);
void convertComputerNameToSecondName(sead::BufferedSafeString* secondName,
                                     const char* computerName);
void convertComputerNameToSecondKanaName(sead::BufferedSafeString* secondKanaName,
                                         const char* computerName);
}  // namespace rs
