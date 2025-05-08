#include "Project/ArchiveEntry.h"

#include <resource/seadArchiveRes.h>

#include "Library/File/FileUtil.h"

namespace al {

ArchiveEntry::ArchiveEntry() = default;

void ArchiveEntry::load() {
    sead::Resource* resource = sead::ResourceMgr::instance()->tryLoad(mLoadArg, "sarc", nullptr);
    mArchiveRes = sead::DynamicCast<sead::ArchiveRes>(resource);
    sendMessageDone();
}

void ArchiveEntry::setLoadRequestInfo(const sead::SafeStringBase<char>& fileName, sead::Heap* heap,
                                      sead::FileDevice* fileDevice) {
    setFileName(fileName);
    mLoadArg.path = getFileName();
    mLoadArg.device = fileDevice;
    mLoadArg.instance_heap = heap;
    mLoadArg.load_data_heap = heap;
    mLoadArg.load_data_alignment = calcFileAlignment(fileName);
    mLoadArg.load_data_alignment = calcFileAlignment(mLoadArg.path);
    mLoadArg.load_data_buffer_alignment = calcBufferSizeAlignment(mLoadArg.path);
    setLoadStateRequested();
}

sead::ArchiveRes* ArchiveEntry::getArchiveRes() {
    return mArchiveRes;
}

void ArchiveEntry::clear() {
    FileEntryBase::clear();
    mArchiveRes = nullptr;
}

}  // namespace al
