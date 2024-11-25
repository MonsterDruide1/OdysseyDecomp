#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"
#include "Library/LiveActor/LiveActor.h"

namespace al {
class LiveActor;

class LiveActorGroup : public HioNode {
public:
    LiveActorGroup(const char*, s32);

    virtual s32 registerActor(LiveActor*);

    void removeActor(const LiveActor*);
    void removeActorAll();
    bool isExistActor(const LiveActor*) const;
    bool isFull() const;
    s32 calcAliveActorNum() const;
    LiveActor* getDeadActor() const;
    LiveActor* tryFindDeadActor() const;
    void appearAll();
    void killAll();
    void makeActorAliveAll();
    void makeActorDeadAll();

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

    T* getActor(s32 idx) const { return (T*)LiveActorGroup::getActor(idx); }

    T* tryFindDeadActor() const { return (T*)LiveActorGroup::tryFindDeadActor(); }
};
}  // namespace al
