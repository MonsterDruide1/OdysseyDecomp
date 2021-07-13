#include "al/resource/Resource.h"
#include "al/util/ArchiveUtil.h"
#include "sead/seadFileDevice.h"
#include "sead/seadHeap.h"
#include "sead/seadResource.h"

#include <agl/g3d.h>

namespace al
{
    Resource::Resource(const sead::SafeStringBase<char> &rPath)
        : mArchive(nullptr), mDevice(nullptr), mName(rPath)
    {
        mHeap = sead::HeapMgr::sInstancePtr->getCurrentHeap();
        _B0 = 0;
        mResFile = 0;
        mArchive = al::loadArchive(rPath);
        mDevice = new sead::ArchiveFileDevice(mArchive);
    }

    Resource::Resource(const sead::SafeStringBase<char> &rPath, sead::ArchiveRes *pResource)
        : mArchive(nullptr), mDevice(nullptr), mName(rPath)
    {
        mArchive = 0;
        mDevice = 0;

        mHeap = sead::HeapMgr::sInstancePtr->getCurrentHeap();
        _B0 = 0;
        mResFile = 0;
        mArchive = pResource;
        mDevice = new sead::ArchiveFileDevice(mArchive);
    }

    bool Resource::isExistFile(const sead::SafeStringBase<char> &rFileName) const
    {
        bool ret = false;
        mDevice->tryIsExistFile(&ret, rFileName);
        return ret;
    }

    unsigned int Resource::getFileSize(const sead::SafeStringBase<char> &rFileName) const
    {
        unsigned int ret;
        mDevice->tryGetFileSize(&ret, rFileName);
        return ret;
    }

    void Resource::cleanupResGraphicsFile()
    {
        mResFile->ReleaseTexture();
        mResFile->Reset();
        agl::g3d::ResFile::Cleanup(mResFile);
        mResFile = nullptr;
    }
};