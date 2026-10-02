#include "MapObj/CageShine.h"

#include "Library/Audio/System/SimpleAudioUser.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/LiveActorFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Se/SeFunction.h"
#include "Library/Shadow/ActorShadowUtil.h"

#include "Util/DemoUtil.h"

namespace {
NERVE_IMPL(CageShine, Wait)
NERVE_IMPL(CageShine, Break)

NERVES_MAKE_NOSTRUCT(CageShine, Wait, Break)
}  // namespace

CageShine::CageShine(const char* name) : al::LiveActor(name) {}

void CageShine::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "CageShine", nullptr);

    bool isPlaySuccessSe = false;
    al::tryGetArg(&isPlaySuccessSe, info, "IsPlaySuccessSe");
    if (isPlaySuccessSe)
        mSuccessSeObj = new al::SimpleAudioUser("SuccessSeObj", info);

    al::initNerve(this, &Wait, 0);
    makeActorAlive();
    mBreakModel = al::tryGetSubActor(this, "壊れモデル");
}

void CageShine::addDemoActor() {
    rs::addDemoActor(this, false);
    if (mBreakModel)
        rs::addDemoActor(mBreakModel, false);
}

void CageShine::startBreak() {
    al::invalidateClipping(this);
    al::setNerve(this, &Break);
}

void CageShine::exeWait() {}

void CageShine::exeBreak() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Break");

        if (mBreakModel)
            mBreakModel->appear();

        if (mSuccessSeObj)
            al::startSe(mSuccessSeObj, "Riddle");

        al::startHitReaction(this, "破壊[仮]");
    }

    if (al::isStep(this, 60) && al::isExistShadowMaskCtrl(this))
        al::hideShadowMask(this);

    if (al::isActionEnd(this)) {
        if (!mBreakModel || al::isDead(mBreakModel))
            kill();
    }
}
