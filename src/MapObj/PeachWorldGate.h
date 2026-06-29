#pragma once

#include "Library/LiveActor/LiveActor.h"

class SaveObjInfo;

namespace al {
class CollisionObj;
}  // namespace al

class PeachWorldGate : public al::LiveActor {
public:
    PeachWorldGate(const char* actorName);

    void init(const al::ActorInitInfo& info) override;

    void start();
    void exeCloseWait();
    void exeOpen();
    void exeOpenWait();

private:
    SaveObjInfo* mSaveObjInfo = nullptr;
    al::CollisionObj* mCollisionObj = nullptr;
};

static_assert(sizeof(PeachWorldGate) == 0x118);
