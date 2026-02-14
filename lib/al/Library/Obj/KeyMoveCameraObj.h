#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class CameraPoser;
class CameraTicket;

class KeyMoveCameraObj : public LiveActor {
public:
    KeyMoveCameraObj(const char*);
    void init(const ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void switchCamera(s32);
    void appear() override;
    void control() override;
    CameraPoser* getCurrentCamera() const;
    CameraTicket* getCurrentCameraTicket() const;
    void kill() override;
    void setFirstCameraStartInterpoleStepDefault();
    void validateStartAtNearestPosition();
    void setPlaySumStep(s32);
    void setEndWaitStep(s32);

private:
    bool* _108;
    CameraPoser** _110;
    CameraTicket** _118;
    s32 _120;
    s32 _124;
};

static_assert(sizeof(KeyMoveCameraObj) == 0x128);

}  // namespace al
