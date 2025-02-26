#pragma once

#include "Library/LiveActor/LiveActor.h"

class ItemGenerator;

class TrampleBush : public al::LiveActor {
public:
    TrampleBush(const char* actorName);

    virtual void init(const al::ActorInitInfo& info) override;
    virtual void initAfterPlacement() override;

    virtual void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    virtual bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                            al::HitSensor* self) override;

    inline bool isSensorOnSwitch(const al::SensorMsg* message, al::HitSensor* other);

    void exeWait();
    void exeReaction();
    void exeTrample();

private:
    ItemGenerator* mItemGenerator = nullptr;
    sead::Vector3f mActorTrans;
};
