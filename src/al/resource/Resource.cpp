#include "al/resource/Resource.h"
#include <agl/g3d/g3dResFile.h>
#include <filedevice/seadFileDevice.h>
#include <heap/seadHeap.h>
#include <heap/seadHeapMgr.h>
#include <resource/seadResource.h>
#include "al/util/ArchiveUtil.h"

namespace al {
Resource::Resource(const sead::SafeStringBase<char>& rPath)
    : mArchive(nullptr), mDevice(nullptr), mName(rPath) {
    mHeap = sead::HeapMgr::sInstancePtr->getCurrentHeap();
    _B0 = 0;
    mResFile = 0;
    mArchive = al::loadArchive(rPath);
    mDevice = new sead::ArchiveFileDevice(mArchive);
}

Resource::Resource(const sead::SafeStringBase<char>& rPath, sead::ArchiveRes* pResource)
    : mArchive(nullptr), mDevice(nullptr), mName(rPath) {
    mHeap = sead::HeapMgr::sInstancePtr->getCurrentHeap();
    _B0 = 0;
    mResFile = 0;
    mArchive = pResource;
    mDevice = new sead::ArchiveFileDevice(mArchive);
}

bool Resource::isExistFile(const sead::SafeStringBase<char>& rFileName) const {
    bool ret = false;
    mDevice->tryIsExistFile(&ret, rFileName);
    return ret;
}

unsigned int Resource::getFileSize(const sead::SafeStringBase<char>& rFileName) const {
    auto device = mDevice;
    u32 ret = 0;
    device->tryGetFileSize(&ret, rFileName);
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