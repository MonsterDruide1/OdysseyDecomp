#include "al/LiveActor/LiveActorGroup.h"
#include "al/util/LiveActorUtil.h"

namespace al {
LiveActorGroup::LiveActorGroup(const char* pName, int max) {
    mMaxActorCount = max;
    mActorCount = 0;
    mGroupName = pName;
    mActors = new LiveActor*[max];
}

int LiveActorGroup::registerActor(al::LiveActor* pActor) {
    this->mActors[this->mActorCount] = pActor;
    auto count = this->mActorCount;
    this->mActorCount = count + 1;
    return count;
}

void LiveActorGroup::removeActor(const al::LiveActor* pActor) {
    for (int i = 0; i < mActorCount; i++) {
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

bool LiveActorGroup::isExistActor(const al::LiveActor* pActor) const {
    if (mActorCount < 1) {
        return false;
    }

    for (int i = 0; i < mActorCount; i++) {
        if (mActors[i] == pActor) {
            return true;
        }
    }

    return false;
}

bool LiveActorGroup::isFull() const {
    return mActorCount >= mMaxActorCount;
}

int LiveActorGroup::calcAliveActorNum() const {
    int count = 0;

    for (int i = 0; i < mActorCount; i++) {
        if (!al::isDead(mActors[i])) {
            count++;
        }
    }

    return count;
}

al::LiveActor* LiveActorGroup::getDeadActor() const {
    for (int i = 0; i < mActorCount; i++) {
        if (al::isDead(mActors[i])) {
            return mActors[i];
        }
    }

    return nullptr;
}

al::LiveActor* LiveActorGroup::tryFindDeadActor() const {
    for (int i = 0; i < mActorCount; i++) {
        if (al::isDead(mActors[i])) {
            return mActors[i];
        }
    }

    return nullptr;
}

void LiveActorGroup::appearAll() {
    for (int i = 0; i < mActorCount; i++) {
        if (al::isDead(mActors[i])) {
            mActors[i]->appear();
        }
    }
}

void LiveActorGroup::killAll() {
    for (int i = 0; i < mActorCount; i++) {
        if (al::isAlive(mActors[i])) {
            mActors[i]->kill();
        }
    }
}

void LiveActorGroup::makeActorAliveAll() {
    for (int i = 0; i < mActorCount; i++) {
        mActors[i]->makeActorAlive();
    }
}

void LiveActorGroup::makeActorDeadAll() {
    for (int i = 0; i < mActorCount; i++) {
        mActors[i]->makeActorDead();
    }
}
};  // namespace al