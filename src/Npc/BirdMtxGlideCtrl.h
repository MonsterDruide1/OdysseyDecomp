#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveExecutor.h"

namespace al {
struct ActorInitInfo;
}  // namespace al

class Bird;

class BirdMtxGlideCtrl : public al::NerveExecutor {
public:
    static BirdMtxGlideCtrl* tryCreateAliveWaitByLinksBird(const sead::Matrix34f*,
                                                           const sead::Vector3f&,
                                                           const al::ActorInitInfo&, const char*);

    BirdMtxGlideCtrl(Bird*);

    void update();
    void makeActorDeadBirdIfAlive();

    void exeInvalid();
    void exeValid();

    void validateGlide();
    void invalidateGlide();
    bool isWaitBird() const;

private:
    char _10[0x8];
};

static_assert(sizeof(BirdMtxGlideCtrl) == 0x18);
