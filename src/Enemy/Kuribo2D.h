#pragma once

#include "Library/LiveActor/LiveActor.h"

#include "Util/IUseDimension.h"

namespace al {
class Nerve;
}

class Kuribo2D : public al::LiveActor, public IUseDimension {
public:
    Kuribo2D(const char* name);

    void init(const al::ActorInitInfo& initInfo) override;
    void startWalk();
    void reset();
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void control() override;
    void appearByGenerator(const sead::Vector3f& trans, const sead::Vector3f& front);
    bool isWait() const;

    void exeWalk();
    void exeWait();
    void exePressDown();
    void exeBlowDown();
    void exeFallAfterGenerate();

    ActorDimensionKeeper* getActorDimensionKeeper() const override { return mDimensionKeeper; }

private:
    ActorDimensionKeeper* mDimensionKeeper = nullptr;
    sead::Vector3f mInitTrans = sead::Vector3f::zero;
    sead::Vector3f mInitFront = sead::Vector3f::zero;

    inline bool receiveDefeatMsg(const al::SensorMsg* message, al::HitSensor* other,
                                 al::HitSensor* self, al::Nerve* nextNerve);
};

static_assert(sizeof(Kuribo2D) == 0x130);
