#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class CameraTicket;
class EventFlowEventData;
class EventFlowExecutor;
class LiveActor;
class PlacementInfo;
}  // namespace al

class AmiiboNpcStateChangeCostume : public al::ActorStateBase {
public:
    AmiiboNpcStateChangeCostume(al::LiveActor* actor, const al::PlacementInfo& placementInfo,
                                al::EventFlowExecutor* eventFlowExecutor);

    void init() override;
    void appear() override;
    void control() override;
    bool receiveEvent(const al::EventFlowEventData* eventData);

private:
    al::CameraTicket* mCameraTicket = nullptr;
    const al::PlacementInfo* mPlacementInfo = nullptr;
    al::EventFlowExecutor* mEventFlowExecutor = nullptr;
    sead::Vector3f mPlayerRestartTrans = sead::Vector3f::zero;
    sead::Quatf mPlayerRestartQuat = sead::Quatf::unit;
};

static_assert(sizeof(AmiiboNpcStateChangeCostume) == 0x58);
