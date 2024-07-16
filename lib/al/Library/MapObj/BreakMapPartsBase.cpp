#include "Library/MapObj/BreakMapPartsBase.h"

#include "Library/Audio/System/SimpleAudioUser.h"
#include "Library/Base/StringUtil.h"
#include "Library/Collision/PartsConnector.h"
#include "Library/Effect/EffectKeeper.h"
#include "Library/Item/ItemUtil.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorResourceFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchKeeper.h"
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
BreakMapPartsBase::BreakMapPartsBase(const char* name) : LiveActor(name) {}

// TODO: Mismatch
void BreakMapPartsBase::init(const ActorInitInfo& info) {
    using LiveActorFunctor = FunctorV0M<LiveActor*, void (LiveActor::*)()>;

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

    listenStageSwitchOnOffKill(this != nullptr ? this : nullptr,  // ???
                               LiveActorFunctor(this, &LiveActor::makeActorDead),
                               LiveActorFunctor(this, &LiveActor::makeActorAlive));

    makeActorAlive();

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
    if (mMtxConnector == nullptr)
        return;

    connectPoseQT(this, mMtxConnector);
}
}  // namespace al
