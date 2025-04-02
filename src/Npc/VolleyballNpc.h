#pragma once

#include <math/seadVector.h>

#include "Library/Event/IEventFlowEventReceiver.h"
#include "Library/LiveActor/LiveActor.h"

namespace al {

struct ActorInitInfo;
class AreaObj;
class EventFlowEventData;
class EventFlowExecutor;
class HitSensor;
class LiveActorGroup;
class SensorMsg;
}  // namespace al

class NpcEventStateScare;
class VolleyballBall;
class VolleyballBase;
class VolleyballLayout;
class Shine;

class VolleyballNpc : public al::LiveActor, public al::IEventFlowEventReceiver {
public:
    VolleyballNpc(const char* name);

    void init(const al::ActorInitInfo& initInfo) override;
    void control() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveEvent(const al::EventFlowEventData* event) override;

    void endGame();
    void startGame(bool);
    void addSuccessCount();
    bool isEnableAppearLayout() const;
    bool isEnterGameArea() const;
    bool isEndMiss() const;
    bool isInGame() const;
    void startSmashReaction();
    void tryChangeMiss();
    void calcTargetTrans(sead::Vector3f*) const;
    bool tryChangeAttack();
    bool tryChangeAttackSign();

    void exeWait();
    void exeCapMessageWaitForThrough();
    void exeCapMessage();
    void exeLetsPlay();
    void exeWaitAfterGame();
    void exeScare();
    void exeWaitAttackSign();
    void exeAttackSign();
    void exeAttack();
    void exeAttackWait();
    void exeMiss();
    void exeWaitRetry();
    void exeReactionCap();
    void exeReactionTrample();

    const sead::Vector3f& getFrontDir() { return mFrontDir; }

private:
    al::EventFlowExecutor* mEventFlowExecutor;
    NpcEventStateScare* mNpcEventStateScare;
    al::LiveActorGroup* mLiveActorGroup;
    VolleyballBase* mBase;
    VolleyballLayout* mLayout;
    al::AreaObj* mGameArea;
    al::AreaObj* _140;
    al::AreaObj* _148;
    s32 _150;
    s32 _154;
    s32 mBestCount;
    s32 mDayCount;
    f32 mSpeed;
    s32 mLevelUpCount;
    f32 mLevelUpSpeed;
    f32 mMaxSpeed;
    f32 mInitSpeed;
    s32 _174;
    s32 mBallNumMax;
    s32 mLevelUpBallCount;
    VolleyballBall* mBall;
    s32 mClearCount;
    s32 mSuperClearCount;
    sead::Vector3f mFrontDir;
    s32 _19c;
    void* _1a0;
    Shine* _1a8;
    Shine* _1b0;
};

static_assert(sizeof(VolleyballNpc) == 0x1b8);
