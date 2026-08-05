#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>
#include <prim/seadEnum.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
struct ActorInitInfo;
class LiveActor;
class MtxConnector;
}  // namespace al

SEAD_ENUM(JangoWaitTreeType, Find, Demo);

class JangoStateWaitTree : public al::ActorStateBase {
public:
    JangoStateWaitTree(al::LiveActor* actor, const al::ActorInitInfo& initInfo,
                       JangoWaitTreeType waitTreeType);

    void init() override;
    void appear() override;
    void control() override;

    bool isEndDemo() const;
    bool isEndFind() const;
    void exeWaitOnSwitch();
    void exeWaitPlayerOnGround();
    void exeEnd();

private:
    JangoWaitTreeType mWaitTreeType;
    al::MtxConnector* mMtxConnector = nullptr;
    sead::Quatf mQuat = sead::Quatf::unit;
    sead::Vector3f mTrans = sead::Vector3f::zero;
};

static_assert(sizeof(JangoStateWaitTree) == 0x50);
