#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class MtxConnector;
}

class PackunTrace : public al::LiveActor {
public:
    PackunTrace(al::LiveActor* parentActor);

    void init(const al::ActorInitInfo& info);
    void initAfterPlacement();
    void control();
    void appear();

private:
    al::MtxConnector* mMtxConnector = nullptr;
};
