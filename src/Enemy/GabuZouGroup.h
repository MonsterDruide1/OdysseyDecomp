#pragma once

#include "Library/Action/RootSercher.h"
#include "Library/LiveActor/LiveActor.h"

class GabuZou;

class GabuZouRootSercher : public al::RootSercher {
public:
    bool checkReach(s32, s32) const override;
};

class GabuZouGroup : public al::LiveActor {
public:
    GabuZouGroup(const char* name);

    void init(const al::ActorInitInfo& info) override;
    bool checkReach(const GabuZou*, const sead::Vector3f&) const;

private:
    char _108[0x110 - sizeof(al::LiveActor)];
};
