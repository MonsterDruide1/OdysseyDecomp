#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"
#include "Library/Resource/Resource.h"

namespace al {
class ActionPadAndCameraInfo;

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
    void update(float, float, float, bool);
    void updateCamera(const ActionPadAndCameraInfo*);
    void updateCameraLoopShake(const ActionPadAndCameraInfo*);
    void updatePadRumbleSourcePos(ActionPadAndCameraInfo*);
};
}  // namespace al
