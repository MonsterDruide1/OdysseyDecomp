#pragma once

#include <basis/seadTypes.h>

#include "Library/Base/String.h"

namespace al {
template <typename T>
struct NameToCreator {
    const char* mName;
    T mCreationFunction;
};

template <typename T>
class Factory {
public:
    inline Factory(const char* factoryName)
        : mFactoryName(factoryName), mFactoryEntries(nullptr), mNumFactoryEntries(0) {}

    template <s32 N>
    inline Factory(const char* factoryName, NameToCreator<T> (&entries)[N])
        : mFactoryName(factoryName) {
        initFactory(entries);
    }
    template <s32 N>
    inline void initFactory(NameToCreator<T> (&entries)[N]) {
        mFactoryEntries = entries;
        mNumFactoryEntries = N;
    }

    virtual const char* convertName(const char* name) const { return name; }

    s32 getNumFactoryEntries() const { return mNumFactoryEntries; }

    s32 getEntryIndex(T* creationPtr, const char* entryName) const {
        const char* name = convertName(entryName);
        s32 nFactoryEntries = mNumFactoryEntries;
        const NameToCreator<T>* entries = mFactoryEntries;
        for (s32 i = 0; i < nFactoryEntries; i++) {
            if (isEqualString(name, entries[i].mName)) {
                *creationPtr = entries[i].mCreationFunction;
                return i;
            }
        }
        return -1;
    }

private:
    const char* mFactoryName;
    NameToCreator<T>* mFactoryEntries;
    s32 mNumFactoryEntries;
};

}  // namespace al
