#pragma once

#include "Library/LiveActor/LiveActor.h"

class DisregardReceiver;

class CollectBgmSpeaker : public al::LiveActor {
public:
    CollectBgmSpeaker(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void exeWait();
    void exeOpen();
    void exeClose();
    void exePlayBgm();
    bool isOpen() const;
    void close();

private:
    DisregardReceiver* mDisregardReceiver = nullptr;
};

static_assert(sizeof(CollectBgmSpeaker) == 0x110);
