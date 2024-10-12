#pragma once

#include <prim/seadSafeString.hpp>

namespace sead{
    class FileDevice;
};

namespace  al {
    class IAudioResourceLoader;
    class ByamlIter;
}

namespace al {
    class FileLoader { 
    public:
        FileLoader(int);
        void listSubdirectories(sead::FixedSafeString<256>*, int, char const*);
        void listFiles(sead::FixedSafeString<256>*, int, char const*, char const*);
        void isExistFile(sead::SafeString const&, sead::FileDevice*) const;
        void getFileDevice(sead::SafeString const&, sead::FileDevice*) const;
        void isExistArchive(sead::SafeString const&, sead::FileDevice*) const;
        void isExistDirectory(sead::SafeString const&, sead::FileDevice*) const;
        void getFileSize(sead::SafeString const&, sead::FileDevice*) const;
        void loadFile(sead::SafeString const&, int, sead::FileDevice*);
        void tryLoadFileToBuffer(sead::SafeString const&, unsigned char*, unsigned int, int, sead::FileDevice*);
        void loadArchive(sead::SafeString const&, sead::FileDevice*);
        void loadArchiveLocal(sead::SafeString const&, char const*, sead::FileDevice*);
        void loadArchiveWithExt(sead::SafeString const&, char const*, sead::FileDevice*);
        void tryRequestLoadArchive(sead::SafeString const&, sead::Heap*, sead::FileDevice*);
        void requestLoadArchive(sead::SafeString const&, sead::Heap*, sead::FileDevice*);
        void loadSoundItem(unsigned int, unsigned int, al::IAudioResourceLoader*);
        void requestLoadSoundItem(unsigned int, unsigned int, al::IAudioResourceLoader*);
        void tryRequestLoadSoundItem(unsigned int, al::IAudioResourceLoader*);
        void requestPreLoadFile(al::ByamlIter const&, sead::Heap*, al::IAudioResourceLoader*);
        void waitLoadDoneAllFile();
        void clearAllEntry();
        void setThreadPriority(int);
    };
}
