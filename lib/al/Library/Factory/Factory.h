#pragma once

#include <basis/seadTypes.h>

#include "Library/Base/StringUtil.h"

namespace al {
template <typename T>
struct NameToCreator {
    const char* name;
    T creationFunction;
};

template <typename T>
class Factory {
public:
    inline Factory(const char* factoryName)
        : mFactoryName(factoryName), mFactoryEntries(nullptr), mNumFactoryEntries(0) {}

    template <s32 N>
    inline Factory(const char* factoryName, const NameToCreator<T> (&entries)[N])
        : mFactoryName(factoryName) {
        initFactory(entries);
    }

    template <s32 N>
    inline void initFactory(const NameToCreator<T> (&entries)[N]) {
        mFactoryEntries = entries;
        mNumFactoryEntries = N;
    }

    virtual const char* convertName(const char* name) const { return name; }

    inline const NameToCreator<T>* getFactoryEntry(const char* name) const {
        const char* convertedName = convertName(name);
        s32 numEntries = mNumFactoryEntries;
        for (s32 i = 0; i < numEntries; ++i) {
            const NameToCreator<T>& entry = mFactoryEntries[i];
            if (isEqualString(convertedName, entry.name))
                return &entry;
        }
        return nullptr;
    }

    inline const T* getCreationFunction(const char* name) const {
        const NameToCreator<T>* entry = getFactoryEntry(name);
        if (entry == nullptr)
            return nullptr;
        return &entry->creationFunction;
    }

private:
    const char* mFactoryName;
    const NameToCreator<T>* mFactoryEntries;
    s32 mNumFactoryEntries;
};

}  // namespace al
