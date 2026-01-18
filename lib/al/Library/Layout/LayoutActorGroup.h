#pragma once

#include <basis/seadTypes.h>

namespace al {
class LayoutActor;

class LayoutActorGroup {
public:
    LayoutActorGroup(const char*, s32);
    void registerActor(LayoutActor*);
    LayoutActor* findDeadActor() const;
    LayoutActor* tryFindDeadActor() const;

    const char* getGroupName() const { return mGroupName; }

    s32 getMaxActorCount() const { return mMaxActorCount; }

    s32 getActorCount() const { return mActorCount; }

    LayoutActor* getActor(s32 idx) const { return mActors[idx]; }

private:
    const char* mGroupName;
    s32 mMaxActorCount;
    s32 mActorCount;
    LayoutActor** mActors;
};
}  // namespace al
