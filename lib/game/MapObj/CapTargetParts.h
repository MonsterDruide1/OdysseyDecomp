#pragma once

#include "al/Library/Obj/PartsModel.h"

class CapTargetParts : public al::PartsModel {
public:
    CapTargetParts(al::LiveActor*, const al::ActorInitInfo&);
    void startHack();
    void startNormal();
    void startSwoon();
};
