#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class CameraTicket;
}  // namespace al

class DoorSnow : public al::LiveActor {
public:
    DoorSnow(const char* name);

    void init(const al::ActorInitInfo& info) override;

    void exeWait();
    void exeOpen();
    void reset(u32 doorIndex);
    void startDemo(u32 doorIndex);
    bool isDemoEnd() const;

private:
    u32 mDoorIndex = 0;
    al::CameraTicket* mCameraTicketOpen = nullptr;
};

static_assert(sizeof(DoorSnow) == 0x118);
