#include "Library/MapObj/WheelMapParts.h"

#include "Library/Area/SwitchKeepOnAreaGroup.h"
#include "Library/Area/SwitchOnAreaGroup.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/MapObj/ChildStep.h"
#include "Library/Math/VectorUtil.h"
#include "Library/Movement/WheelMovement.h"
#include "Library/Nerve/NerveSetupUtil.h"

namespace {
using namespace al;

NERVE_ACTION_IMPL(WheelMapParts, Wait)
NERVE_ACTION_IMPL(WheelMapParts, Move)
NERVE_ACTION_IMPL(WheelMapParts, AssistStop)

NERVE_ACTIONS_MAKE_STRUCT(WheelMapParts, Wait, Move, AssistStop)
}  // namespace

namespace al {
WheelMapParts::WheelMapParts(const char* name) : LiveActor(name) {}

void WheelMapParts::init(const ActorInitInfo& info) {
    tryInitSubActorKeeperChildStep(this, info);
    initNerveAction(this, "Wait", &NrvWheelMapParts.mCollector, 0);
    initMapPartsActor(this, info, nullptr);
    tryGetQuatPtr(this);
    registerAreaHostMtx(this, info);

    mWheelMovement = new WheelMovement(this, info);

    initMaterialCode(this, info);
    trySetEffectNamedMtxPtr(this, "Surface", &mSurfaceEffectMtx);
    createChildStep(info, this, true);

    mSwitchKeepOnAreaGroup = tryCreateSwitchKeepOnAreaGroup(this, info);
    mSwitchOnAreaGroup = tryCreateSwitchOnAreaGroup(this, info);

    trySyncStageSwitchAppear(this);
}

void WheelMapParts::control() {
    sead::Vector3f moveDir = mWheelMovement->getMoveDir();
    if (mWheelMovement->get_50() < 0.0f)
        moveDir *= -1;

    makeMtxUpFrontPos(&mSurfaceEffectMtx, sead::Vector3f::ey, moveDir, getTrans(this));

    if (mSwitchKeepOnAreaGroup != nullptr)
        mSwitchKeepOnAreaGroup->update(getTrans(this));

    if (mSwitchOnAreaGroup != nullptr)
        mSwitchOnAreaGroup->update(getTrans(this));
}
}  // namespace al
