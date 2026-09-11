#pragma once

#include "Library/LiveActor/LiveActor.h"

#include "Demo/IUseDemoSkip.h"

class BirdCarryMeat;

class CarryMeat : public al::LiveActor, public IUseDemoSkip {
public:
    CarryMeat(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void validateHack();
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void endHack();
    void reset();
    void control() override;
    bool isFirstDemo() const override;
    bool isEnableSkipDemo() const override;
    void skipDemo() override;
    void endDemoAndBind(const sead::Matrix34f*);

    bool isWaiting() const;
    bool isBindable() const;
    bool isCarryReaction() const;

    void startDemo();
    void drop();
    void addDemoActor();

    void exeWaitSalt();
    void exeWait();
    void exeHackStart();
    void exeHackSalt();
    void exeHackSaltJump();
    void exeHack();
    void exeReactionJump();
    void exeReactionHipDrop();
    void exeHackMoveWait();
    void exeHackMove();
    void exeHackJump();
    void exeHackJumpEnd();
    void exeHackBindable();
    void exeDemoCarryMeat();
    void exeHackBind();
    void exeHackBindReaction();
    void exeHackFall();
    void exeSurf();

    void setParent(BirdCarryMeat* parent) { mParent = parent; }

private:
    BirdCarryMeat* mParent;
    char filler_118[0x90];
};

static_assert(sizeof(CarryMeat) == 0x1a8);

;
