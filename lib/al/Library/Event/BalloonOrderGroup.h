#pragma once

#include <math/seadVector.h>

#include "Library/Placement/PlacementFunction.h"

namespace al {
struct ActorInitInfo;
class PlacementId;
class IUseEventFlowData;
class LiveActor;

class BalloonOrderGroup {
public:
    struct Requester {
        Requester(const LiveActor* actor, const IUseEventFlowData* user, const ActorInitInfo& info)
            : actor(actor), user(user) {
            getArg(&ballonOrder, info, "BalloonOrder");
            tryGetArg(&ballonShowStep, info, "BallonShowStep");
        }

        const LiveActor* actor;
        const IUseEventFlowData* user;
        s32 ballonOrder = 0;
        s32 ballonShowStep = 180;
    };

    BalloonOrderGroup(const PlacementInfo& info);

    bool isEqual(const PlacementInfo& info) const;
    void registerRequester(const LiveActor* actor, const IUseEventFlowData* user,
                           const ActorInitInfo& info);
    bool isEnableAppearBalloon(const LiveActor* actor) const;
    void update(const sead::Vector3f& position);

private:
    PlacementId* mPlacementId = nullptr;
    bool mIsActive = false;
    s32 mShowStep = 0;
    s32 mRequesterCount = 0;
    s32 mActiveRequesterIdx = 0;
    Requester** mRequesters = nullptr;
};

static_assert(sizeof(BalloonOrderGroup) == 0x20);
static_assert(sizeof(BalloonOrderGroup::Requester) == 0x18);
}  // namespace al
