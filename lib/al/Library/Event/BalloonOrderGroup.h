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
    class Requester {
    public:
        Requester(const LiveActor* actor, const IUseEventFlowData* user, const ActorInitInfo& info)
            : mActor(actor), mUser(user) {
            getArg(&mBallonOrder, info, "BalloonOrder");
            tryGetArg(&mBallonShowStep, info, "BallonShowStep");
        }

        const LiveActor* getActor() const { return mActor; }

        const IUseEventFlowData* getUser() const { return mUser; }

        s32 getBallonOrder() const { return mBallonOrder; }

        s32 getBallonShowStep() const { return mBallonShowStep; }

    private:
        const LiveActor* mActor;
        const IUseEventFlowData* mUser;
        s32 mBallonOrder = 0;
        s32 mBallonShowStep = 180;
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
