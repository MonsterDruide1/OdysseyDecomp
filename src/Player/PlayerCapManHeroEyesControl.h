#pragma once

#include "Library/Nerve/NerveExecutor.h"

namespace al {
class LiveActor;
}

class PlayerCapManHeroEyesControl : public al::NerveExecutor {
public:
    PlayerCapManHeroEyesControl(const char* name, al::LiveActor* puppetEye,
                                al::LiveActor* normal2DEye);

    void start();
    void fastStart();
    void end();
    void startPuppet();
    void endPuppet();
    void kill();
    void update();
    al::LiveActor* getPuppetEye() const;
    bool isAppear() const;
    bool isDisappear() const;
    bool isDemo() const;
    bool isDeadCap() const;
    void requestWaitAnimChange(const char* actionName);
    void exeAppear();
    void exeWait();
    void exeDisappear();
    void exeDead();
    void exeDemo();

private:
    al::LiveActor* mPuppetEye;
    al::LiveActor* mNormal2DEye;
};

static_assert(sizeof(PlayerCapManHeroEyesControl) == 0x20);
