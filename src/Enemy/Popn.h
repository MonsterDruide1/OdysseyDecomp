#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class EnemyStateBlowDown;
}

class Popn : public al::LiveActor {
public:
    Popn(const char* actorName);
    virtual void init(const al::ActorInitInfo& info) override;
    virtual void attackSensor(al::HitSensor* target, al::HitSensor* source) override;
    virtual bool receiveMsg(const al::SensorMsg* message, al::HitSensor* source,
                            al::HitSensor* target) override;
    virtual void control() override;

    void appearByGenerater(const sead::Vector3f& pos, s32 a2, bool a3, bool a4);
    void exeAppear();
    void exeWait();
    void exeWaitRove();
    void exeTurn();
    void exeMoveStart();
    void exeMove();
    void exeMoveEnd();
    void exeBlowDown();

private:
    al::EnemyStateBlowDown* mStateBlowDown = nullptr;
    f32 mAwakeDistance = 1500.0f;
    bool mIsGenerateItem = false;
    sead::Quatf mQuat = sead::Quatf::unit;
    const char* mArchiveName = "Popn";
    bool mIsAngry = false;
    bool mIsFollowGroundNormal = true;
};
