#pragma once

#include "Library/LiveActor/LiveActor.h"

#define HACKSIZE sizeof(al::LiveActor)

class HackCap : public al::LiveActor {
public:
    void calcReturnTargetPos(sead::Vector3f*);

    void hide(bool);
    void forcePutOn(void);

    bool isFlying(void) const;

    bool isThrowTypeSpiral(void) const;

    unsigned char padding_118[0x118 - HACKSIZE];
    al::LiveActor* mActorA;
    unsigned char padding_08[0x08];
    al::LiveActor* mPlayerActor;
};
