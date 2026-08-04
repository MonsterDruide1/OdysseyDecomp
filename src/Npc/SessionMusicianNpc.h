#pragma once

#include <container/seadPtrArray.h>
#include <math/seadVector.h>
#include <prim/seadEnum.h>
#include <string>

#include "Library/Event/IEventFlowEventReceiver.h"
#include "Library/Event/IEventFlowQueryJudge.h"
#include "Library/LiveActor/LiveActor.h"

#include "Npc/SessionMayorParam.h"
#include "Util/DemoUtil.h"

namespace sead {
class PtrArrayImpl;
}  // namespace sead

namespace al {
struct ActorInitInfo;
class ActorItemKeeper;
class EventFlowExecutor;
class EventReceiver;
class HitSensor;
class IEventFlowEventReceiver;
class IEventFlowQueryJudge;
class SensorMsg;
}  // namespace al

class BgmAnimeSynchronizer;
class CityManRhythmInfo;
class IUsePlayerPuppet;
class JointLookAtController;
class NpcJointLookAtController;
class NpcStateReaction;
class SessionMusicianWarpAgent;
class TalkNpcCap;
class TalkNpcParam;

class SessionMusicianNpc : public al::LiveActor,
                           public al::IEventFlowEventReceiver,
                           public al::IEventFlowQueryJudge {
public:
    SEAD_ENUM(EventType,
    Wait,
    Live,
    Ceremony,
    PowerPlant
    )

    SessionMusicianNpc(const char* name = nullptr) : al::LiveActor(name) {}

    void init(const al::ActorInitInfo& initInfo) override;

    void startEvent();

    void appear() override;
    void kill() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;

    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    bool receiveEvent(const al::EventFlowEventData* event) override;

    const char* judgeQuery(const char* query) const override;

    void endClipped() override;

    void forceControlForDance();
    void control() override;
    void controlForDance();

    bool isJoined() const;
    bool isStateWarp() const;
    bool isEnableMuteBgmTrack() const;

    void exeWait();
    void exeWaitNoEventFlowSabi();
    void exeWaitNoEventFlow();
    void exeReaction();
    void endReaction();

    void exeWarp();
    void exeWarpStart();
    void exeWarpEnd();
    void doneWarp();

    EventType getEventType() { return mEventType; }

    sead::PtrArray<al::LiveActor> getFanActors() { return mFanActors; }

    SessionMusicianWarpAgent* getWarpAgent() const { return mWarpAgent; };

    s32 getLinkedShineIndex() const { return mLinkedShineIndex; };

    const sead::Vector3f& getMoonGetDemoPlayerPos() const { return mMoonGetDemoPlayerPos; }

    const sead::Quatf& getMoonGetDemoPlayerPose() const { return mMoonGetDemoPlayerPose; }

    void addDemoActors() {
        for (s32 i = 0; i < mFanActors.size(); i++)
            rs::addDemoActor(mFanActors[i], true);
    }

private:
    EventType mEventType;
    al::EventFlowExecutor* mEventFlowExecutor;
    const MusicianCameraParams* mMusicianCameraParams;
    sead::PtrArray<al::LiveActor> mFanActors;
    SessionMusicianWarpAgent* mWarpAgent;
    NpcStateReaction* mNpcStateReaction;
    TalkNpcParam* mTalkNpcParam;
    NpcJointLookAtController* mNpcJoint;
    TalkNpcCap* mTalkNpcCap;
    sead::Vector3f mMoonGetDemoPlayerPos;
    sead::Quatf mMoonGetDemoPlayerPose;
    s32 mLinkedShineIndex;
    IUsePlayerPuppet* mPuppet;
    bool mIsJoined;
    std::string mWaitAnimName;
    std::string mSabiAnimName;
    CityManRhythmInfo* mRhythmInfo;
    BgmAnimeSynchronizer* mBgmSync;
    f32 mAnimBeatFrameOffset;
    bool mIsNeedRythmResync;
    bool mIsUseBgmTrackMute;
};

static_assert(sizeof(SessionMusicianNpc) == 0x1e0);
