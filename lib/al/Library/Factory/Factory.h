#pragma once

#include <basis/seadTypes.h>

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

private:
    const char* mFactoryName;
    const NameToCreator<T>* mFactoryEntries;
    s32 mNumFactoryEntries;
};

}  // namespace al
