#pragma once

#include <math/seadVector.h>

namespace al {
class ActionPadAndCameraInfo;
class ActorResource;
class LiveActor;

class ActionPadAndCameraCtrl {
public:
    static ActionPadAndCameraCtrl* tryCreate(const LiveActor*, const ActorResource*,
                                             const sead::Vector3f*, const char*);

    ActionPadAndCameraCtrl(const LiveActor*, const ActorResource*, const sead::Vector3f*,
                           const char*);
    void notifyActorDead();
    void notifyActorEndClipped();
    void notifyActorStartClipped();
    void startAction(const char*);
    void startPadRumble(const ActionPadAndCameraInfo*);
    void stopPadRumble(const ActionPadAndCameraInfo*);
    void update(f32, f32, f32, bool);
    void updateCamera(const ActionPadAndCameraInfo*);
    void updateCameraLoopShake(const ActionPadAndCameraInfo*);
    void updatePadRumbleSourcePos(ActionPadAndCameraInfo*);
};
}  // namespace al
