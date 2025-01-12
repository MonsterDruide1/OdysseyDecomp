#pragma once

#include "Library/LiveActor/LiveActor.h"

class PlayerSword : public al::LiveActor {
public:
    PlayerSword(const char* name);

    void initPartsMtx(al::LiveActor* other, const al::ActorInitInfo& info,
                      const sead::Matrix34f* mtx, const char* name);
    void makeActorAlive() override;
    void updatePose();
    void control() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;

private:
    al::LiveActor* mPlayer = nullptr;
    al::HitSensor* mPlayerBodySensor = nullptr;
    const sead::Matrix34f* mPlayerBaseMtx = nullptr;
    bool mIsInvisible = false;
};
