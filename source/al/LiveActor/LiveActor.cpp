#include "al/LiveActor/LiveActor.h"

#include "al/resource/Resource.h"

namespace al
{
    LiveActor::LiveActor(const char *pName)
    {

    }

    al::NerveKeeper* LiveActor::getNerveKeeper() const
    {
        return mNerveKeeper;
    }
};