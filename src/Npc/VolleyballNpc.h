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
class MessageTagDataHolder;
class SensorMsg;

template <class T>
class DeriveActorGroup;
}  // namespace al

class NpcEventStateScare;
class VolleyballBall;
class VolleyballBase;
class VolleyballLayout;
class Shine;

class VolleyballNpc : public al::LiveActor, public al::IEventFlowEventReceiver {
public:
    VolleyballNpc(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void control() override;
    void endGame();
    bool receiveEvent(const al::EventFlowEventData* event) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    void startGame(bool startEventFlow);
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void addSuccessCount();
    bool isEnableAppearLayout() const;
    bool isEnterGameArea() const;
    bool isEndMiss() const;
    bool isInGame() const;
    void startSmashReaction();
    void exeWait();
    void exeCapMessageWaitForThrough();
    void exeCapMessage();
    void exeLetsPlay();
    void exeWaitAfterGame();
    void exeScare();
    void exeWaitAttackSign();
    void exeAttackSign();
    bool tryChangeMiss();
    void exeAttack();
    void calcTargetTrans(sead::Vector3f* out) const;
    bool tryChangeAttack();
    bool tryChangeAttackSign();
    void exeAttackWait();
    void exeMiss();
    void exeWaitRetry();
    void exeReactionCap();
    void exeReactionTrample();

    s32 getCurrentScore() const { return mCurrentScore; }

    s32 getBestScore() const { return mBestScore; }

    const sead::Vector3f& getFrontDir() const { return mFrontDir; }

private:
    al::EventFlowExecutor* mEventFlowExecutor = nullptr;
    NpcEventStateScare* mNpcEventStateScare = nullptr;
    al::DeriveActorGroup<VolleyballBall>* mBallGroup = nullptr;
    VolleyballBase* mBase = nullptr;
    VolleyballLayout* mLayout = nullptr;
    al::AreaObj* mGameArea = nullptr;
    al::AreaObj* mLayoutArea = nullptr;
    al::AreaObj* mBallArea = nullptr;
    s32 _150 = 0;
    s32 mCurrentScore = 0;
    s32 mBestScore = 0;
    s32 mDayScore = 0;
    f32 mSpeed = 0.0f;
    s32 mLevelUpCount = 0;
    f32 mLevelUpSpeed = 0.0f;
    f32 mMaxSpeed = 0.0f;
    f32 mInitSpeed = 0.0f;
    s32 mLevel = 1;
    s32 mBallNumMax = 3;
    s32 mLevelUpBallCount = 0;
    VolleyballBall* mBall = nullptr;
    s32 mClearCount = 0;
    s32 mSuperClearCount = 0;
    sead::Vector3f mFrontDir = sead::Vector3f::zero;
    s32 mUpdateEventFlowCounter = 0;
    bool mIsShowCapMessage = false;
    Shine* mShineClear = nullptr;
    Shine* mShineSuperClear = nullptr;
};

static_assert(sizeof(VolleyballNpc) == 0x1b8);
