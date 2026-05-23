#pragma once

#include "Library/Action/RootSercher.h"
#include "Library/LiveActor/LiveActor.h"

namespace al {
class LiveActorGroup;
}

class GabuZou;

class GabuZouRootSercher : public al::RootSercher {
public:
    bool checkReach(s32, s32) const override;
};

static_assert(sizeof(GabuZouRootSercher) == 0x48);

class GabuZouGroup : public al::LiveActor {
public:
    GabuZouGroup(const char* name);

    void init(const al::ActorInitInfo& info) override;
    bool checkReach(const GabuZou*, const sead::Vector3f&) const;

private:
    al::LiveActorGroup* _108;
};

static_assert(sizeof(GabuZouGroup) == 0x110);
