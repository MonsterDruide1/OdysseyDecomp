#pragma once

#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Obj/CollisionObj.h"

#include "System/SaveObjInfo.h"

class PeachWorldGate : public al::LiveActor {
public:
    PeachWorldGate(const char* name);
    void init(const al::ActorInitInfo& info) override;

    void start();
    void exeCloseWait();
    void exeOpenWait();
    void exeOpen();

private:
    SaveObjInfo* mSaveObjInfo = nullptr;
    al::CollisionObj* mCollisionObj = nullptr;
};

static_assert(sizeof(PeachWorldGate) == 0x118);
