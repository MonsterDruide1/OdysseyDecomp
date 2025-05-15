#pragma once

#include <basis/seadTypes.h>
#include <container/seadBuffer.h>
#include <prim/seadSafeString.h>

namespace sead {
class FileDevice;
class Heap;
}  // namespace sead

namespace al {
class ArchiveEntry;

class ArchiveHolder {
public:
    ArchiveHolder();
    ArchiveEntry* addNewLoadRequestEntry(const sead::SafeString& fileName, sead::Heap* heap,
                                         sead::FileDevice* fileDevice);
    ArchiveEntry* tryFindEntry(const sead::SafeString& fileName);
    void waitLoadDoneAll();
    void clearEntry();

private:
    sead::Buffer<ArchiveEntry> mArchiveEntries;
    s32 mSize = 0;
};

}  // namespace al
