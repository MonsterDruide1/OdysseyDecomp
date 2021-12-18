#pragma once

namespace al {
class LiveActor;

struct FactoryEntry {
    const char* mName;
    void* mCreationFunction;
};

template <typename T>
class Factory {
public:
    inline Factory(const char* factory_name)
        : mFactoryName(factory_name), mFactoryEntries(nullptr), mNumFactoryEntries(0) {}
    template <int N>
    inline Factory(const char* factory_name, al::FactoryEntry (&entries)[N])
        : mFactoryName(factory_name) {
        initFactory(entries);
    }
    template <int N>
    inline void initFactory(al::FactoryEntry (&entries)[N]) {
        mFactoryEntries = entries;
        mNumFactoryEntries = N;
    }

    virtual const char* convertName(const char*) const;

private:
    const char* mFactoryName;
    al::FactoryEntry* mFactoryEntries;
    int mNumFactoryEntries;
};

class ActorFactory : public al::Factory<al::LiveActor* (*)(char const*)> {
public:
    ActorFactory(const char*);
};
}  // namespace al
