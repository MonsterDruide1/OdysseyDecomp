#pragma once

#include "Library/LiveActor/LiveActor.h"
#include "Library/Movement/MoveType.h"

class FukankunZoomObj : public al::LiveActor {
public:
    FukankunZoomObj(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void onAppear();
    void onKill();
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void exeWait();

private:
    f32 mRailMoveSpeed = 20.0f;
    al::MoveType mMoveType = al::MoveType::Loop;
};

static_assert(sizeof(FukankunZoomObj) == 0x110);
