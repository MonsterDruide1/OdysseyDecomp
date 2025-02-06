#pragma once

#include <container/seadPtrArray.h>

#include "Library/LiveActor/LiveActor.h"

class SphinxRideMiniGameObjGenerator : public al::LiveActor {
public:
    SphinxRideMiniGameObjGenerator(const char* name, f32 a2, f32 a3);

    void init(const al::ActorInitInfo& info) override;
    void makeActorDeadAllActor();
    void exeWait();
    void appearAllActor();

private:
    sead::PtrArray<al::LiveActor> mActorArray;
    sead::Vector3f* mTransArray = nullptr;
    bool mIsAppearRandom = false;
};
