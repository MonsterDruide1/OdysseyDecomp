#pragma once

#include "Library/LiveActor/LiveActor.h"

class HackCap : public al::LiveActor {
public:
    void calcReturnTargetPos(sead::Vector3f*);

    void hide(bool);
    void forcePutOn();

    bool isFlying() const;

    bool isThrowTypeSpiral() const;

    unsigned char padding_to_118[0x10];
    al::LiveActor* mActorA;
    unsigned char padding_08[0x08];
    al::LiveActor* mPlayerActor;
};
