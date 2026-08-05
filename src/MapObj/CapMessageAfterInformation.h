#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class AreaObjGroup;
}  // namespace al

class CapMessageAfterInformation : public al::LiveActor {
public:
    CapMessageAfterInformation(const char* name);

    void makeActorDead() override;
    void init(const al::ActorInitInfo& info) override;

    void exeWait();
    bool isEnable() const;
    void exeShow();
    void endShow();

private:
    al::AreaObjGroup* mInfoAreaGroup = nullptr;
    const char* mLabel = nullptr;
    s32 mHackActorType = -2;
};

static_assert(sizeof(CapMessageAfterInformation) == 0x120);
