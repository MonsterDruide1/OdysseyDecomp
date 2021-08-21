#pragma once

#include <cstddef>
template <class T, std::ptrdiff_t N>
constexpr std::ptrdiff_t ssize(const T (&array)[N]) noexcept
{
    return N;
}

namespace al
{
    class LiveActor;

    struct FactoryEntry
    {
        const char* mName; // _0
        void* mCreationFunction; // _8
    };

    template<typename T>
    class Factory
    {
    public:
        inline Factory() {}
        
        virtual const char* convertName(const char *) const;
    };

    class ActorFactory : public al::Factory<al::LiveActor * (*)(char const *)>
    {
    public:
        ActorFactory(const char *);

        const char* mFactoryName; // _8
        al::FactoryEntry* mFactoryEntries; // _10
        int mNumFactoryEntries; // _18
    };
};