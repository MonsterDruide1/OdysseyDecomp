#include "Library/MapObj/BreakMapPartsBase.h"

#include "Library/Audio/System/SimpleAudioUser.h"
#include "Library/Base/StringUtil.h"
#include "Library/Collision/PartsConnector.h"
#include "Library/Effect/EffectKeeper.h"
#include "Library/Item/ItemUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorResourceFunction.h"
#include "Library/LiveActor/SubActorKeeper.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Obj/PartsFunction.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Se/SeFunction.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"
#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/ByamlUtil.h"

namespace {
using namespace al;

NERVE_IMPL(BreakMapPartsBase, Wait)
NERVE_IMPL(BreakMapPartsBase, Break)

NERVES_MAKE_STRUCT(BreakMapPartsBase, Wait, Break)
}  // namespace

namespace al {
static void syncStageSwitchMakeActorDead(LiveActor* actor) {
    using LiveActorFunctor = FunctorV0M<LiveActor*, void (LiveActor::*)()>;

    listenStageSwitchOnOffKill(actor, LiveActorFunctor(actor, &LiveActor::makeActorDead),
                               LiveActorFunctor(actor, &LiveActor::makeActorAlive));
    actor->makeActorAlive();
}

BreakMapPartsBase::BreakMapPartsBase(const char* name) : LiveActor(name) {}

void BreakMapPartsBase::init(const ActorInitInfo& info) {
    const char* suffix = nullptr;
    tryGetStringArg(&suffix, info, "SuffixName");

    if (suffix != nullptr && isEqualString(suffix, "None"))
        suffix = nullptr;

    initMapPartsActor(this, info, suffix);
    initNerve(this, &NrvBreakMapPartsBase.Wait, 0);

    ByamlIter iter;
    bool isExistInitFile = tryGetActorInitFileIter(&iter, this, "InitBreakMapParts", nullptr);
    const char* breakType = nullptr;
    if (isExistInitFile) {
        breakType = tryGetByamlKeyStringOrNULL(iter, "BreakType");
        tryGetByamlV3f(&mItemOffset, iter, "ItemOffset");
        tryGetByamlBool(&mIsExistHitReactionBreak, iter, "IsExistHitReactionBreak");
    }

    mJudgeFunction = getJudgeFunction(breakType);
    mMtxConnector = tryCreateMtxConnector(this, info);

    syncStageSwitchMakeActorDead(this);

    if (isExistItemKeeper(this))
        setAppearItemOffset(this, mItemOffset);

    bool isPlaySuccessSe = false;
    if (tryGetArg(&isPlaySuccessSe, info, "IsPlaySuccessSe") && isPlaySuccessSe)
        mAudioKeeper = new SimpleAudioUser("SuccessSeObj", info);
}

void BreakMapPartsBase::initAfterPlacement() {
    if (mMtxConnector == nullptr)
        return;

    attachMtxConnectorToCollision(mMtxConnector, this, false);
}

void BreakMapPartsBase::kill() {
    LiveActor::kill();

    tryOnSwitchDeadOn(this);
}

void BreakMapPartsBase::movement() {
    if (!isNerve(this, &NrvBreakMapPartsBase.Wait) || mMtxConnector != nullptr) {
        LiveActor::movement();

        return;
    }

    if (getEffectKeeper() != nullptr && getEffectKeeper()->get_21())
        getEffectKeeper()->update();
}

void BreakMapPartsBase::calcAnim() {
    if (isNerve(this, &NrvBreakMapPartsBase.Wait) && mMtxConnector == nullptr)
        return;

    LiveActor::calcAnim();
}

void BreakMapPartsBase::exeWait() {
    if (mMtxConnector != nullptr)
        connectPoseQT(this, mMtxConnector);
}

void BreakMapPartsBase::exeBreak() {
    if (isFirstStep(this)) {
        LiveActor* subActor = tryGetSubActor(this, "壊れモデル");
        if (subActor != nullptr)
            subActor->appear();

        subActor = tryGetSubActor(this, "残留モデル");
        if (subActor != nullptr) {
            subActor->appear();

            if (isTraceModelRandomRotate(subActor))
                addRotateAndRepeatY(subActor, getRandomDegree());
        }

        if (mIsExistHitReactionBreak)
            startHitReaction(this, "破壊");

        if (mAudioKeeper != nullptr)
            startSe(mAudioKeeper, "Riddle");

        if (!isExistAction(this, "Break")) {
            kill();
        } else {
            startAction(this, "Break");
            hideModelIfShow(this);
        }

        return;
    }

    kill();
}

void BreakMapPartsBase::startBreakByProgram() {
    invalidateClipping(this);
    invalidateCollisionParts(this);

    setNerve(this, &NrvBreakMapPartsBase.Break);
}

bool BreakMapPartsBase::receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) {
    if (isNerve(this, &NrvBreakMapPartsBase.Wait) && mJudgeFunction(message, other, self)) {
        startBreakByProgram();

        return true;
    }

    return false;
}

JudgeFuncPtr BreakMapPartsBase::getJudgeFunction(const char* name) const {
    return nullptr;
}
}  // namespace al
