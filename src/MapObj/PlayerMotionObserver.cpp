#include "MapObj/PlayerMotionObserver.h"

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Player/PlayerUtil.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "System/GameDataUtil.h"
#include "Util/PlayerUtil.h"

namespace {
NERVE_IMPL(PlayerMotionObserver, Wait)
NERVE_IMPL(PlayerMotionObserver, CheckLand)
NERVE_IMPL(PlayerMotionObserver, WaitForSwitchOn)

NERVES_MAKE_NOSTRUCT(PlayerMotionObserver, Wait, CheckLand, WaitForSwitchOn)
}  // namespace

PlayerMotionObserver::PlayerMotionObserver(const char* name) : al::LiveActor(name) {}

void PlayerMotionObserver::init(const al::ActorInitInfo& info) {
    al::initActorSceneInfo(this, info);
    al::initExecutorUpdate(this, info, "監視オブジェ");
    al::initStageSwitch(this, info);
    al::initActorPoseTRSV(this);
    al::initActorClipping(this, info);
    al::invalidateClipping(this);

    mPlayerHolder = info.actorSceneInfo.playerHolder;

    const char* observationTargetMotion = nullptr;
    al::tryGetStringArg(&observationTargetMotion, info, "ObservationTargetMotion");
    if (!observationTargetMotion) {
        kill();
        return;
    }

    if (al::isEqualString(observationTargetMotion, "Land")) {
        if (al::isValidStageSwitch(this, "SwitchStart")) {
            al::initNerve(this, &Wait, 0);
            mIsUseSwitchStart = true;
        } else {
            al::initNerve(this, &CheckLand, 0);
        }
    }

    al::tryGetArg(&mSwitchOnDelayFrameNum, info, "SwitchOnDelayFrameNum");
    mSaveObjInfo = rs::createSaveObjInfoNoWriteSaveDataInSameScenario(info);
    if (rs::isOnSaveObjInfo(mSaveObjInfo)) {
        al::onStageSwitch(this, "SwitchSpecifiedPlayerMotionOn");
        makeActorDead();
        return;
    }

    makeActorAlive();
}

void PlayerMotionObserver::exeWait() {
    if (al::isOnStageSwitch(this, "SwitchStart"))
        al::setNerve(this, &CheckLand);
}

void PlayerMotionObserver::exeCheckLand() {
    if (mIsUseSwitchStart && !al::isOnStageSwitch(this, "SwitchStart")) {
        al::setNerve(this, &Wait);
        return;
    }

    if (rs::isPlayerOnGround(al::getPlayerActor(this, 0)))
        al::setNerve(this, &WaitForSwitchOn);
}

void PlayerMotionObserver::exeWaitForSwitchOn() {
    if (al::isFirstStep(this))
        mSwitchOnDelayFrame = mSwitchOnDelayFrameNum;

    if (mSwitchOnDelayFrame-- == 0) {
        al::onStageSwitch(this, "SwitchSpecifiedPlayerMotionOn");
        rs::onSaveObjInfo(mSaveObjInfo);
        makeActorDead();
    }
}
