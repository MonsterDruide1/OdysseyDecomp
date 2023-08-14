#pragma once

#include <basis/seadTypes.h>
#include "Library/HostIO/HioNode.h"

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

private:
    const char* mGroupName;
    s32 mMaxActorCount;
    s32 mActorCount;
    LiveActor** mActors;
};

template <class T>
class DeriveActorGroup : LiveActorGroup {
public:
    s32 registerActor(T* actor) { LiveActorGroup::registerActor(actor); }
    void removeActor(const T* actor) { LiveActorGroup::removeActor(actor); }
    void removeActorAll() { LiveActorGroup::removeActorAll(); }
    bool isExistActor(const T* actor) const { return LiveActorGroup::isExistActor(actor); }
    bool isFull() const { return LiveActorGroup::isFull(); }
    s32 calcAliveActorNum() const { return LiveActorGroup::calcAliveActorNum(); }
    T* getDeadActor() const { return LiveActorGroup::getDeadActor(); }
    T* tryFindDeadActor() const { return LiveActorGroup::tryFindDeadActor(); }
    void appearAll() { LiveActorGroup::appearAll(); }
    void killAll() { LiveActorGroup::killAll(); }
    void makeActorAliveAll() { LiveActorGroup::makeActorAliveAll(); }
    void makeActorDeadAll() { LiveActorGroup::makeActorDeadAll(); }
};
};  // namespace al
