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

    ProjectActorFactory::ProjectActorFactory() : ActorFactory("繧｢繧ｯ繧ｿ繝ｼ逕滓")
    {
        #ifdef VER_100
        mNumFactoryEntries = 0x23A;
        #endif

        #ifdef VER_110
        mNumFactoryEntries = 0x23A;
        #endif

        #ifdef VER_120
        mNumFactoryEntries = 0x23B;
        #endif
    }
};