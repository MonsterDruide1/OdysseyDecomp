#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class MtxConnector;
class SensorMsg;
}  // namespace al

class LinkTargetPoint : public al::LiveActor {
public:
    LinkTargetPoint();

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void control() override;
    void calcTargetCenterPos(sead::Vector3f* pos) const;
    bool receiveMsgInitTouchTargetInfo(const al::SensorMsg* msg);

private:
    al::MtxConnector* mMtxConnector = nullptr;
    sead::Vector3f mNormal = sead::Vector3f::ey;
};

static_assert(sizeof(LinkTargetPoint) == 0x120);
