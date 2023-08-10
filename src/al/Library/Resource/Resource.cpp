#include "al/Library/Resource/Resource.h"
#include <filedevice/seadFileDevice.h>
#include <g3d/aglNW4FToNN.h>
#include <heap/seadHeap.h>
#include <heap/seadHeapMgr.h>
#include <resource/seadResource.h>
#include "al/Library/File/FileUtil.h"

namespace al {
Resource::Resource(const sead::SafeString& path)
    : mArchive(nullptr), mDevice(nullptr), mName(path) {
    mHeap = sead::HeapMgr::sInstancePtr->getCurrentHeap();
    mData = nullptr;
    mResFile = nullptr;
    mArchive = loadArchive(path);
    mDevice = new sead::ArchiveFileDevice(mArchive);
}

Resource::Resource(const sead::SafeString& path, sead::ArchiveRes* archive)
    : mArchive(nullptr), mDevice(nullptr), mName(path) {
    mHeap = sead::HeapMgr::sInstancePtr->getCurrentHeap();
    mData = nullptr;
    mResFile = nullptr;
    mArchive = archive;
    mDevice = new sead::ArchiveFileDevice(mArchive);
}

bool Resource::isExistFile(const sead::SafeString& path) const {
    bool ret = false;
    mDevice->tryIsExistFile(&ret, path);
    return ret;
}

u32 Resource::getFileSize(const sead::SafeString& path) const {
    auto device = mDevice;
    u32 ret = 0;
    device->tryGetFileSize(&ret, path);
    return ret;
}

void Resource::cleanupResGraphicsFile() {
    if (!mResFile)
        return;

    mResFile->ReleaseTexture();
    mResFile->Reset();
    agl::g3d::ResFile::Cleanup(mResFile);
    mResFile = nullptr;
}
};  // namespace al
