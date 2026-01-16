#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"
#include "Library/LiveActor/LiveActor.h"

namespace al {
class LiveActor;

class LiveActorGroup : public HioNode {
public:
    LiveActorGroup(const char* groupName, s32 maxActors);

    virtual s32 registerActor(LiveActor* pActor);

    void removeActor(const LiveActor* pActor);
    void removeActorAll();
    bool isExistActor(const LiveActor* pActor) const;
    bool isFull() const;
    s32 calcAliveActorNum() const;
    LiveActor* getDeadActor() const;
    LiveActor* tryFindDeadActor() const;
    void appearAll();
    void killAll();
    void makeActorAliveAll();
    void makeActorDeadAll();

    s32 getMaxActorCount() const { return mMaxActorCount; }

    s32 getActorCount() const { return mActorCount; }

    LiveActor* getActor(s32 idx) const { return mActors[idx]; }

private:
    const char* mGroupName;
    s32 mMaxActorCount;
    s32 mActorCount;
    LiveActor** mActors;
};

template <class T>
class DeriveActorGroup : public LiveActorGroup {
public:
    DeriveActorGroup(const char* groupName, s32 groupCount)
        : LiveActorGroup(groupName, groupCount) {}

    T* getDeriveActor(s32 idx) const { return (T*)LiveActorGroup::getActor(idx); }

    T* tryFindDeadDeriveActor() const { return (T*)LiveActorGroup::tryFindDeadActor(); }
};
}  // namespace al
