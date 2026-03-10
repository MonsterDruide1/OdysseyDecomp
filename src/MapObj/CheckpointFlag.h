#pragma once

#include "Library/LiveActor/LiveActor.h"
#include "Library/Message/IUseMessageSystem.h"

namespace al {
class PlacementId;
class PlacementInfo;
}  // namespace al

class BirdMtxGlideCtrl;

class AirBubble;

class CheckpointFlag : public al::LiveActor, public al::IUseMessageSystem {
public:
    CheckpointFlag(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void control() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    const al::PlacementInfo* getPlayerRestartInfo() const;
    void setAfter();
    void startArriveByWarp();
    void initHomeFlag(const al::ActorInitInfo& info);

    void exeBefore();
    void exeGet();
    void exeAfter();
    void exeShake();

    const al::MessageSystem* getMessageSystem() const override { return mMessageSystem; };

protected:
    friend class CheckpointFlagWatcher;

private:
    al::MessageSystem* mMessageSystem = nullptr;
    al::ActorInitInfo* mInfo = nullptr;
    al::PlacementId* mPlaceId = nullptr;
    const char16* mFlagName = u"/////////////////";
    s32 mHitCooldown = 0;
    bool mIsGot = false;
    bool mIsHome = false;
    bool mIsZeroGravity = false;
    AirBubble* mAirBubble = nullptr;
    s32 mAirBubbleTimer = -1;
    BirdMtxGlideCtrl* mBirdMtxGlideCtrl = nullptr;
};

static_assert(sizeof(CheckpointFlag) == 0x150);
