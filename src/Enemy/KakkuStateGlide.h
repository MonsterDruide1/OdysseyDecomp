#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
struct ActorInitInfo;
}  // namespace al

class ExternalForceKeeper;
class Kakku;

class KakkuStateGlide : public al::HostStateBase<Kakku> {
public:
    KakkuStateGlide(Kakku* host, const al::ActorInitInfo& initInfo,
                    const ExternalForceKeeper* externalForceKeeper);

    void appear() override;
    void updateCameraTarget(f32 rate);
    void kill() override;
    void control() override;
    bool checkCollidedWall(bool isCheckFloor) const;
    void getStick(sead::Vector2f* stick) const;
    bool isEnableEndState() const;
    bool isEndByOnGround() const;
    bool tryCalcExternalVelocity(sead::Vector3f* velocity) const;

    void exeGlideStart();
    void exeGlide();
    void updatePoseByStick();
    void updateVelocitySide();
    void exeGlideJump();
    void updatePoseSideByStick();
    void exeGlideJumpEnd();

private:
    u8 _20[0xc8] = {};
};

static_assert(sizeof(KakkuStateGlide) == 0xe8);
