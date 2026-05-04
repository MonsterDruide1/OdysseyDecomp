#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class CameraTicket;
class KeyPoseKeeper;
class PlacementId;
}  // namespace al

class RiseMapPartsOld : public al::LiveActor {
public:
    RiseMapPartsOld(const char* name);
    void init(const al::ActorInitInfo& info) override;
    void startRise();
    void initAfterPlacement() override;
    void exeWait();
    void exeMove();
    void exeStop();

private:
    al::KeyPoseKeeper* mKeyPoseKeeper = nullptr;
    al::PlacementId* mPlacementId = nullptr;
    s32 mMoveTime = 0;
    al::CameraTicket* mObjectCamera = nullptr;
};

static_assert(sizeof(RiseMapPartsOld) == 0x128);
