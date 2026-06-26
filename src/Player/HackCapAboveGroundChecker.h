#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
}  // namespace al

class IUsePlayerCollision;

class HackCapAboveGroundChecker {
public:
    HackCapAboveGroundChecker(const al::LiveActor*, const IUsePlayerCollision*);

    void update();
    bool isAboveGround() const;

private:
    const al::LiveActor* mActor;
    const IUsePlayerCollision* mCollision;
    bool mIsAboveGround;
};
