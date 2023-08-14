#pragma once

#include <basis/seadTypes.h>

namespace al {
struct NameToCreator {
    const char* mName;
    void* mCreationFunction;
};

template <typename T>
using CreatorFunction = T* (*)(const char*);

template <typename T>
class Factory {
public:
    inline Factory(const char* factoryName)
        : mFactoryName(factoryName), mFactoryEntries(nullptr), mNumFactoryEntries(0) {}

    template <s32 N>
    inline Factory(const char* factoryName, NameToCreator (&entries)[N])
        : mFactoryName(factoryName) {
        initFactory(entries);
    }
    template <s32 N>
    inline void initFactory(NameToCreator (&entries)[N]) {
        mFactoryEntries = entries;
        mNumFactoryEntries = N;
    }

    virtual const char* convertName(const char* name) const { return name; }

private:
    const char* mFactoryName;
    NameToCreator* mFactoryEntries;
    s32 mNumFactoryEntries;
};

}  // namespace al
