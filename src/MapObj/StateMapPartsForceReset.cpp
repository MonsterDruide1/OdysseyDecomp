#include "MapObj/StateMapPartsForceReset.h"

#include "Library/Area/AreaObjUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nerve/Nerve.h"
#include "Library/Nerve/NerveKeeper.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"

#include "Util/PlayerUtil.h"

namespace {
NERVE_IMPL(StateMapPartsForceReset, Reset)

class StateMapPartsForceResetNrvResetEnd : public al::Nerve {
public:
    void execute(al::NerveKeeper* keeper) const override {}
};

NERVES_MAKE_NOSTRUCT(StateMapPartsForceReset, Reset, ResetEnd)
}  // namespace

StateMapPartsForceReset::StateMapPartsForceReset(al::LiveActor* actor,
                                                 const al::ActorInitInfo& initInfo)
    : al::ActorStateBase("マップパーツ強制リセット", actor) {
    mEnableResetDist = 2000.0f;
    mIsInvalidClipping = false;
    mInitTrans.z = 0.0f;
    mLinkResetAreaGroup = nullptr;
    mInitTrans.x = 0.0f;
    mInitTrans.y = 0.0f;

    initNerve(&Reset, 0);
    mLinkResetAreaGroup =
        al::createLinkAreaGroup(actor, initInfo, "LinkResetArea",
                                "リフト位置リセットエリアグループ", "リフト位置リセットエリア");
    al::tryGetArg(&mEnableResetDist, initInfo, "EnableResetDist");

    const sead::Vector3f& trans = al::getTrans(actor);
    mInitTrans.e = trans.e;
    mInitQuat = al::getQuat(actor);
}

void StateMapPartsForceReset::appear() {
    al::NerveStateBase::appear();
    al::setNerve(this, &ResetEnd);
    mIsInvalidClipping = al::isInvalidClipping(mActor);
    al::invalidateClipping(mActor);
}

bool StateMapPartsForceReset::isEnableForceReset() const {
    if (!mLinkResetAreaGroup)
        return false;

    if (!al::isInAreaObj(mLinkResetAreaGroup, rs::getPlayerPos(mActor)))
        return false;

    const sead::Vector3f& trans = al::getTrans(mActor);
    return (trans - mInitTrans).length() > mEnableResetDist;
}

void StateMapPartsForceReset::exeReset() {
    if (al::isFirstStep(this)) {
        al::startHitReaction(mActor, "消滅");
        if (al::isExistAction(mActor, "Reset"))
            al::tryStartAction(mActor, "Reset");
        if (!mIsInvalidClipping)
            al::validateClipping(mActor);
    }

    al::setQuat(mActor, mInitQuat);
    al::setTrans(mActor, mInitTrans);
    al::resetPosition(mActor);
    al::startHitReaction(mActor, "出現");
    al::setNerve(this, &Reset);
    kill();
}

void StateMapPartsForceReset::exeResetEnd() {}
