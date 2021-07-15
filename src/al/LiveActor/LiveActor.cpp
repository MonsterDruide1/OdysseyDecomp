#include "al/LiveActor/LiveActor.h"

#include "al/resource/Resource.h"

namespace al
{
    al::NerveKeeper* LiveActor::getNerveKeeper() const
    {
        return mNerveKeeper;
    }
};