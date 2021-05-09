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

    int LiveActorGroup::registerActor(al::LiveActor *pActor)
    {
        this->mActors[this->mActorCount] = pActor;
        auto count = this->mActorCount;
        this->mActorCount = count + 1;
        return count;
    }

    void LiveActorGroup::removeActor(const al::LiveActor *pActor)
    {
        auto actorCount = mActorCount;

        if (actorCount >= 1)
        {
            for (auto i = 0; i < actorCount; i++)
            {
                if (mActors[i] == pActor)
                {
                    mActors[mActorCount] = mActors[mActorCount - 1];
                    mActorCount--;
                }
            }
        }
    }
};