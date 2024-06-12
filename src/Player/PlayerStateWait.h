#pragma once

#include "Library/Nerve/NerveStateBase.h"
#include "PlayerUtil.h"

class PlayerStateWait : public al::ActorStateBase {
public:
    PlayerStateWait(al::LiveActor *,PlayerConst const*,IUsePlayerCollision const*,PlayerModelChangerHakoniwa const*,PlayerJointControlKeeper const*,al::WaterSurfaceFinder const*,IUsePlayerHeightCheck const*,IJudge const*,PlayerAnimator *,PlayerTrigger *,PlayerCapManHeroEyesControl *,PlayerJointParamCenterDynamics *,PlayerJointParamGroundPose *);
    void initSceneStartAnim();
    bool isLandStain();
    bool isEnableCancelHipDropJump();
    bool tryConnectWait();
    bool tryClearIgnoreSwitchOnAreaAnim();
    bool isEnableCancelAction() const;

private:
    u8 padding[0xE8 - sizeof(al::ActorStateBase)];
};
