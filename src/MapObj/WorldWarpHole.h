#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class CameraTicket;
class HitSensor;
class RippleCtrl;
class SensorMsg;
}  // namespace al
class CapMessageEnableChecker;
class IUsePlayerPuppet;

class WorldWarpHole : public al::LiveActor {
public:
    WorldWarpHole(const char*);
    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void control() override;
    void exeCloseWait();
    void exeOpenWait();
    void exeInvalidStart();
    void exeInvalid();
    void exeInvalidEnd();
    void exeBindStart();
    void exeBind();
    void exeIn();
    void exeInAfter();
    void exeOutBefore();
    void exeOut();
    void exeOutBindRequest();

private:
    IUsePlayerPuppet* mPlayerPuppet;
    const char* mChangeStageId;
    CapMessageEnableChecker* mCapMessageEnableChecker;
    sead::Vector3f _120;
    sead::Quatf _12c;
    sead::Vector3f _13c;
    bool _148;
    bool _149;
    s32 mWorldId;
    al::CameraTicket* mCameraTicket;
    sead::Vector3f _158;
    sead::Vector3f _164;
    sead::Vector3f _170;
    al::RippleCtrl* mRippleCtrl;
};

static_assert(sizeof(WorldWarpHole) == 0x188);
