#include "Library/LiveActor/LiveActorGroup.h"

#include "Library/LiveActor/LiveActorUtil.h"

namespace al {
LiveActorGroup::LiveActorGroup(const char* groupName, s32 maxActors)
    : mGroupName(groupName), mMaxActorCount(maxActors) {
    mActorCount = 0;
    mActors = new LiveActor*[maxActors];
}

s32 LiveActorGroup::registerActor(LiveActor* pActor) {
    this->mActors[this->mActorCount] = pActor;
    auto count = this->mActorCount;
    this->mActorCount = count + 1;
    return count;
}

void LiveActorGroup::removeActor(const LiveActor* pActor) {
    for (s32 i = 0; i < mActorCount; i++) {
        if (mActors[i] == pActor) {
            mActors[i] = mActors[mActorCount - 1];
            mActorCount--;
            break;
        }
    }
}

void LiveActorGroup::removeActorAll() {
    mActorCount = 0;
}

bool LiveActorGroup::isExistActor(const LiveActor* pActor) const {
    if (mActorCount < 1) {
        return false;
    }

    for (s32 i = 0; i < mActorCount; i++) {
        if (mActors[i] == pActor) {
            return true;
        }
    }

    return false;
}

bool LiveActorGroup::isFull() const {
    return mActorCount >= mMaxActorCount;
}

s32 LiveActorGroup::calcAliveActorNum() const {
    s32 count = 0;

    for (s32 i = 0; i < mActorCount; i++) {
        if (!isDead(mActors[i])) {
            count++;
        }
    }

    return count;
}

LiveActor* LiveActorGroup::getDeadActor() const {
    for (s32 i = 0; i < mActorCount; i++) {
        if (isDead(mActors[i])) {
            return mActors[i];
        }
    }

    return nullptr;
}

LiveActor* LiveActorGroup::tryFindDeadActor() const {
    for (s32 i = 0; i < mActorCount; i++) {
        if (isDead(mActors[i])) {
            return mActors[i];
        }
    }

    return nullptr;
}

void LiveActorGroup::appearAll() {
    for (s32 i = 0; i < mActorCount; i++) {
        if (isDead(mActors[i])) {
            mActors[i]->appear();
        }
    }
}

void LiveActorGroup::killAll() {
    for (s32 i = 0; i < mActorCount; i++) {
        if (isAlive(mActors[i])) {
            mActors[i]->kill();
        }
    }
}

void LiveActorGroup::makeActorAliveAll() {
    for (s32 i = 0; i < mActorCount; i++) {
        mActors[i]->makeActorAlive();
    }
}

void LiveActorGroup::makeActorDeadAll() {
    for (s32 i = 0; i < mActorCount; i++) {
        mActors[i]->makeActorDead();
    }
}
};  // namespace al
