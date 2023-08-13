#pragma once

#include "Library/Obj/PartsModel.h"

class CapTargetParts : public al::PartsModel {
public:
    CapTargetParts(al::LiveActor* actor, const al::ActorInitInfo& initInfo);
    void startHack();
    void startNormal();
    void startSwoon();
};
