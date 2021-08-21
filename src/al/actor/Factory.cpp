#include "al/actor/Factory.h"

namespace al
{
    template <typename T>
    const char* Factory<T>::convertName(const char *pName) const
    {
        return pName;
    }

    ActorFactory::ActorFactory(const char *pName)
        : mFactoryName(pName), mFactoryEntries(nullptr), mNumFactoryEntries(0)
    { }
};