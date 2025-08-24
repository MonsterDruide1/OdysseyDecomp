#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

class Bubble;

class BubbleStateInLauncher : public al::NerveStateBase {
public:
    BubbleStateInLauncher(Bubble*);

    void appear() override;
    void kill() override;

    bool isWaiting() const;
    bool isFlying() const;
    void shoot(const sead::Vector3f&, f32);
    void calcLaunchPos(sead::Vector3f*, const sead::Vector3f&, f32, s32) const;

    void exeEnter();
    void exeWait();
    void exeShoot();

private:
    char filler[0x40];
};
