#include "al/resource/Resource.h"

#include <agl/g3d.h>

namespace al
{
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