#pragma once

#include "Library/Action/RootSercher.h"
#include "Library/LiveActor/LiveActor.h"

class GabuZou;

class GabuZouRootSercher : al::RootSercher {
public:
    bool checkReach(s32, s32) const override;
};

static_assert(sizeof(GabuZouRootSercher) == 0x48);

class GabuZouGroup : public al::LiveActor {
public:
    GabuZouGroup(const char* name);

    void init(const al::ActorInitInfo&) override;
    bool checkReach(const GabuZou*, const sead::Vector3f&) const;

private:
    al::LiveActorGroup* _108;
};

static_assert(sizeof(GabuZouGroup) == 0x110);
