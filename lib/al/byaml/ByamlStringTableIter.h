#pragma once

#include <basis/seadTypes.h>

namespace al {
class ByamlStringTableIter {
public:
    ByamlStringTableIter();
    ByamlStringTableIter(const u8* data, bool isRev);

    int getSize() const;
    const u32* getAddressTable() const;
    int getStringAddress(int) const;
    int getEndAddress() const;
    const char* getString(int) const;
    int getStringSize(int) const;
    int findStringIndex(const char*) const;
    bool isValidate() const;

private:
    const u8* mData = nullptr;
    bool isRev = false;
};
}  // namespace al
