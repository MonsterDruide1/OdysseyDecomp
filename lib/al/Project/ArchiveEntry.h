#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>
#include <resource/seadResourceMgr.h>

#include "Project/FileEntryBase.h"

namespace sead {
class ArchiveRes;
class FileDevice;
class Heap;
}  // namespace sead

namespace al {

class ArchiveEntry : public FileEntryBase {
public:
    ArchiveEntry();

    void load() override;
    void setLoadRequestInfo(const sead::SafeString& fileName, sead::Heap* heap,
                            sead::FileDevice* fileDevice);
    sead::ArchiveRes* getArchiveRes();
    void clear();

private:
    sead::ResourceMgr::LoadArg mLoadArg;
    sead::ArchiveRes* mArchiveRes = nullptr;
};

static_assert(sizeof(ArchiveEntry) == 0x118);

}  // namespace al
