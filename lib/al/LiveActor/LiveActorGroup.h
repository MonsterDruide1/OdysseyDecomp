#pragma once

#include "al/hio/HioNode.h"

namespace al {
class LiveActor;

class LiveActorGroup : public al::HioNode {
public:
    LiveActorGroup(const char*, int);

    virtual int registerActor(al::LiveActor*);

    void removeActor(const al::LiveActor*);
    void removeActorAll();
    bool isExistActor(const al::LiveActor*) const;
    bool isFull() const;
    int calcAliveActorNum() const;
    al::LiveActor* getDeadActor() const;
    al::LiveActor* tryFindDeadActor() const;
    void appearAll();
    void killAll();
    void makeActorAliveAll();
    void makeActorDeadAll();

    const char* mGroupName;   // _8
    int mMaxActorCount;       // _10
    int mActorCount;          // _14
    al::LiveActor** mActors;  // _18
};
};  // namespace al