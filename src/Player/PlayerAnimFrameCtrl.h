#pragma once

#include <prim/seadSafeString.h>

namespace al {
class LiveActor;
}

class PlayerAnimFrameCtrl {
public:
    const char* getActionName() const;
    void startAction(al::LiveActor* actor, const sead::SafeString& actionName);
};
