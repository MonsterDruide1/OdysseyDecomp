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

    void LiveActorGroup::removeActor(const al::LiveActor *pActor) {
        for (int i = 0; i < mActorCount; i++) {
            if (mActors[i] == pActor) {
                mActors[i] = mActors[mActorCount - 1];
                mActorCount--;
                break;
            }
        }
    }
};