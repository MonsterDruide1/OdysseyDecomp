#include "Library/Obj/EffectObjInterval.h"

#include "Library/Collision/PartsConnector.h"
#include "Library/Effect/EffectKeeper.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Obj/EffectObjFunction.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Se/SeFunction.h"
#include "Library/Stage/StageSwitchKeeper.h"
#include "Library/Thread/FunctorV0M.h"

namespace al {
EffectObjInterval::EffectObjInterval(const char* name) : LiveActor(name) {}

void EffectObjInterval::init(const ActorInitInfo& info) {
    using EffectObjIntervalFunctor = FunctorV0M<EffectObjInterval*, void (EffectObjInterval::*)()>;

    EffectObjFunction::initActorEffectObj(this, info);
    trySyncStageSwitchAppear(this);
    tryListenStageSwitchKill(this);

    listenStageSwitchOnOff(this, "OnKillOffAppearSwitch",
                           EffectObjIntervalFunctor(this, &EffectObjInterval::kill),
                           EffectObjIntervalFunctor(this, &EffectObjInterval::appear));

    mMtxConnector = tryCreateMtxConnector(this, info);

    tryGetArg(&mEmitInterval, info, "EmitInterval");
    if (tryGetArg(&mFirstEmitFrame, info, "FirstEmitFrame"))
        mTimer = mFirstEmitFrame;
    else
        mTimer = 0;
}

void EffectObjInterval::initAfterPlacement() {
    if (mMtxConnector == nullptr)
        return;

    attachMtxConnectorToCollision(mMtxConnector, this, false);
}

void EffectObjInterval::control() {
    if (mMtxConnector != nullptr)
        connectPoseQT(this, mMtxConnector);

    makeMtxRT(&mBaseMtx, this);

    if (mTimer <= 0) {
        mIsStarted = false;
        emitEffect(this, "Wait", nullptr);
        alEffectFunction::emitEffectIfExist(this, "Wait2", nullptr);
        tryStartSe(this, "Wait");
        mIsStarted = true;

        mTimer = mEmitInterval;

        return;
    }

    mTimer--;
}

void EffectObjInterval::appear() {
    mIsStarted = false;

    LiveActor::appear();
}

void EffectObjInterval::kill() {
    mIsStarted = false;

    LiveActor::kill();
}

const sead::Matrix34f* EffectObjInterval::getBaseMtx() const {
    return &mBaseMtx;
}
}  // namespace al
