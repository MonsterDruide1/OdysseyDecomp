#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class EnemyStateBlowDown;
}

class Popn : public al::LiveActor {
public:
    Popn(const char* actorName);
    void init(const al::ActorInitInfo& info) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void control() override;

    void appearByGenerater(const sead::Vector3f& pos, s32 color, bool isAppear,
                           bool isGenerateItem);
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
