#pragma once

#include <prim/seadSafeString.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class SensorMsg;
class HitSensor;
class LiveActor;
}  // namespace al

class NpcStateReactionParam {
public:
    NpcStateReactionParam();
    NpcStateReactionParam(const char* reactionAnimName, const char* capReactionAnimName);

    const sead::FixedSafeString<64>& getReactionAnimName() const { return mReactionAnimName; }

    const sead::FixedSafeString<64>& getCapReactionAnimName() const { return mCapReactionAnimName; }

private:
    sead::FixedSafeString<64> mReactionAnimName;
    sead::FixedSafeString<64> mCapReactionAnimName;
};

static_assert(sizeof(NpcStateReactionParam) == 0xb0);

class NpcStateReaction : public al::ActorStateBase {
public:
    NpcStateReaction(al::LiveActor* actor, bool isHackActor);

    static NpcStateReaction* create(al::LiveActor* actor, const NpcStateReactionParam* param);
    static NpcStateReaction* createForHuman(al::LiveActor* actor,
                                            const NpcStateReactionParam* param);
    static NpcStateReaction* createForHackActor(al::LiveActor* actor,
                                                const NpcStateReactionParam* param);

    void appear() override;
    void kill() override;

    virtual bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                            al::HitSensor* self);
    virtual bool receiveMsgWithoutTrample(const al::SensorMsg* msg, al::HitSensor* self,
                                          al::HitSensor* other);
    virtual bool receiveMsgNoReaction(const al::SensorMsg* msg, al::HitSensor* self,
                                      al::HitSensor* other);

    void setEnableCapReaction(bool enable) { mIsEnableCapReaction = enable; }

    void setStateReactionParam(NpcStateReactionParam* param) { mParam = param; }

    void setIsHuman(bool isHuman) { mIsHuman = isHuman; }

private:
    const NpcStateReactionParam* mParam;
    bool mIsHackActor;
    bool mIsEnableCapReaction;
    bool mIsHuman;
    bool mHasSavedClippingState;
};

static_assert(sizeof(NpcStateReaction) == 0x30);
