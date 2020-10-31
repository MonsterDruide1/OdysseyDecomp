#include "al/actor/LiveActor.h"

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