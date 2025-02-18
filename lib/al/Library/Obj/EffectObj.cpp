#include "Library/Obj/EffectObj.h"

#include "Library/Base/StringUtil.h"
#include "Library/Collision/PartsConnector.h"
#include "Library/Effect/EffectKeeper.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Matrix/MatrixUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Obj/EffectObjFunction.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Rail/RailUtil.h"
#include "Library/Se/SeFunction.h"
#include "Library/Stage/StageSwitchKeeper.h"
#include "Library/Thread/FunctorV0M.h"

namespace al {
EffectObj::EffectObj(const char* name) : LiveActor(name) {}

void EffectObj::init(const ActorInitInfo& info) {
    using EffectObjFunctor = FunctorV0M<EffectObj*, void (EffectObj::*)()>;

    EffectObjFunction::initActorEffectObj(this, info);

    const char* className = nullptr;
    getClassName(&className, info);

    if (isEqualString("EffectObjScale", className)) {
        mIsEffectObjScale = true;

        setEffectEmitterVolumeScale(this, "Wait", getScale(this));
    }

    trySyncStageSwitchAppear(this);
    tryListenStageSwitchKill(this);

    listenStageSwitchOnOff(this, "OnKillOffAppearSwitch", EffectObjFunctor(this, &EffectObj::kill),
                           EffectObjFunctor(this, &EffectObj::appear));

    if (isExistRail(info, "Rail")) {
        tryGetArg((s32*)&mMoveType, info, "MoveType");
        tryGetArg(&mRailMoveSpeed, info, "RailMoveSpeed");
        setRailPosToStart(this);
        setRailClippingInfo(&mRailPos, this, 100.0f, 100.0f);
        tryGetArg(&mIsSyncRailPose, info, "IsSyncRailPose");
    }

    mMtxConnector = tryCreateMtxConnector(this, info);
}

void EffectObj::initAfterPlacement() {
    if (mMtxConnector == nullptr)
        return;

    attachMtxConnectorToCollision(mMtxConnector, this, false);
}

void EffectObj::control() {
    if (isExistRail(this)) {
        switch (mMoveType) {
        case MoveType::Turn:
            moveSyncRailTurn(this, mRailMoveSpeed);
            break;
        case MoveType::Loop:
            moveSyncRailLoop(this, mRailMoveSpeed);
            break;
        default:
            moveSyncRail(this, mRailMoveSpeed);
            break;
        }

        if (mIsSyncRailPose) {
            sead::Vector3f railUp;
            sead::Matrix34f poseMtx;

            const sead::Vector3f& railDir = getRailDir(this);
            calcRailUp(&railUp, this);
            const sead::Vector3f& railTrans = getTrans(this);
            makeMtxFrontUpPos(&poseMtx, railDir, railUp, railTrans);
            updatePoseMtx(this, &poseMtx);
        }
    } else if (mMtxConnector != nullptr)
        connectPoseQT(this, mMtxConnector);

    makeMtxRT(&mBaseMtx, this);
    tryEmitStart();
}

void EffectObj::appear() {
    mIsStarted = false;

    LiveActor::appear();
}

void EffectObj::kill() {
    mIsStarted = false;

    LiveActor::kill();
}

bool EffectObj::tryEmitStart() {
    if (mIsStarted)
        return false;

    emitEffect(this, "Wait", nullptr);
    alEffectFunction::emitEffectIfExist(this, "Wait2", nullptr);
    tryStartSe(this, "Wait");
    mIsStarted = true;

    return true;
}
}  // namespace al
