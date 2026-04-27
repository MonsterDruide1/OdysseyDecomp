#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class EventFlowEventData;
class HitSensor;
class SensorMsg;
}  // namespace al

class SessionMusicianWarpAgent;

class SessionMusicianNpc : public al::LiveActor {
public:
    void init(const al::ActorInitInfo& info) override;
    void startEvent();
    void appear() override;
    void kill() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    bool receiveEvent(const al::EventFlowEventData* data);
    bool judgeQuery(const char* query) const;
    void endClipped() override;
    void control() override;
    void forceControlForDance();
    void controlForDance();
    bool isJoined() const;
    bool isStateWarp() const;
    void doneWarp();
    bool isEnableMuteBgmTrack() const;
    void exeWaitNoEventFlowSabi();
    void exeWaitNoEventFlow();
    void exeWait();
    void exeWarpStart();
    void exeWarp();
    void exeWarpEnd();
    void exeReaction();
    void endReaction();

    SessionMusicianWarpAgent* getWarpAgent() const { return mWarpAgent; }

    s32 getDemoActorNum() const { return mDemoActors.size(); }

    u32 getDemoActorNumU() const { return mDemoActors.size(); }

    al::LiveActor* tryGetDemoActor(u64 index) const {
        if (getDemoActorNumU() <= index)
            return nullptr;

        return mDemoActors.data()[index];
    }

    const sead::Vector3f& getMoonGetDemoPlayerPos() const { return mMoonGetDemoPlayerPos; }

    const sead::Quatf& getMoonGetDemoPlayerPose() const { return mMoonGetDemoPlayerPose; }

private:
    u8 _108[0x28];
    sead::PtrArray<al::LiveActor> mDemoActors;
    SessionMusicianWarpAgent* mWarpAgent = nullptr;
    u8 _148[0x20];
    sead::Vector3f mMoonGetDemoPlayerPos;
    sead::Quatf mMoonGetDemoPlayerPose;
};
