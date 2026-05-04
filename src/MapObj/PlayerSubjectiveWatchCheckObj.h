#pragma once

#include "Library/LiveActor/LiveActor.h"

class PlayerSubjectiveWatchCheckObj : public al::LiveActor {
public:
    PlayerSubjectiveWatchCheckObj(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void control() override;
    void appear() override;
    void kill() override;
    void endClipped() override;
    void exeNotSubjective();
    void exeIn();
    void exeOut();
};

static_assert(sizeof(PlayerSubjectiveWatchCheckObj) == 0x108);
