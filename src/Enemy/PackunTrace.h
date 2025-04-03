#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class MtxConnector;
}

class PackunTrace : public al::LiveActor {
public:
    PackunTrace(al::LiveActor* parentActor);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void control() override;
    void appear() override;

private:
    al::MtxConnector* mMtxConnector = nullptr;
};
