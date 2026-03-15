#pragma once

#include "Library/LiveActor/LiveActor.h"

class AirBubble : public al::LiveActor {
public:
    AirBubble(const char*, bool);

    void init(const al::ActorInitInfo& info) override;
    void endClipped() override;
    void startClipped() override;
    void appear() override;
    void appearPopup();
    void control() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void exeCreate();
    void exeWait();
    void exeReaction();
    void exeMove();
    void exeGot();

    void getItem();

    void exeCapGet();
    void exeBreak();

private:
    char padding[0x20];
};

static_assert(sizeof(AirBubble) == 0x128);
