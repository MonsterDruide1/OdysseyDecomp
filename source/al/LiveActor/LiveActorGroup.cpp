#include "al/LiveActor/LiveActorGroup.h"

namespace al
{
    LiveActorGroup::LiveActorGroup(const char *pName, int max)
    {
        mMaxActorCount = max;
        mActorCount = 0;
        mGroupName = pName;
        mActors = new LiveActor*[max];
    }

    void LiveActorGroup::registerActor(al::LiveActor *pActor)
    {
        mActors[mActorCount] = pActor;
        ++mActorCount;
    }
};