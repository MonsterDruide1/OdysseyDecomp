#pragma once

#include "Library/Nerve/NerveStateBase.h"

class IUsePlayerHack;

class HackerStateBase : public al::NerveStateBase {
public:
    HackerStateBase(const char* name, al::LiveActor* actor, IUsePlayerHack* hackUser);
    ~HackerStateBase() override;

    al::LiveActor* getActor() const { return mActor; }
    IUsePlayerHack* getHackUser() const { return mHackUser; }

private:
    al::LiveActor* mActor = nullptr;
    IUsePlayerHack* mHackUser = nullptr;
};
