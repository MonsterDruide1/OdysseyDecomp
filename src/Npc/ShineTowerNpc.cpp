#include "Npc/ShineTowerNpc.h"

#include <prim/seadSafeString.h>

#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Message/MessageHolder.h"
#include "Library/Message/MessageTagDataHolder.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Player/PlayerUtil.h"

#include "System/GameDataFunction.h"
#include "Util/NpcEventFlowUtil.h"

namespace {
NERVE_IMPL(ShineTowerNpc, Wait);
NERVE_IMPL(ShineTowerNpc, GoKoopa);
NERVE_IMPL(ShineTowerNpc, NoBalloon);

NERVES_MAKE_STRUCT(ShineTowerNpc, Wait, GoKoopa);
NERVES_MAKE_NOSTRUCT(ShineTowerNpc, NoBalloon);
}  // namespace

ShineTowerNpc::ShineTowerNpc(const char* name) : al::LiveActor(name) {}

void ShineTowerNpc::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "NpcCap", nullptr);
    al::initNerve(this, &NrvShineTowerNpc.Wait, 0);
    mEventFlowExecutor = rs::initEventFlow(this, info, "HomeMechanicNpc", nullptr);
    al::MessageTagDataHolder* msgTagDataHolder = al::initMessageTagDataHolder(1);
    al::registerMessageTagDataScore(msgTagDataHolder, "Score", &mRemainingShineCount);
    rs::initEventMessageTagDataHolder(mEventFlowExecutor, msgTagDataHolder);

    makeActorAlive();
    al::hideModel(this);
    mRemainingShineCount = getRemainingShineCount(this);
    if (mRemainingShineCount == 0)
        al::setNerve(this, &NrvShineTowerNpc.GoKoopa);
}

s32 getRemainingShineCount(al::LiveActor* actor) {
    bool isGameClear = false;
    s32 unlockShine = GameDataFunction::findUnlockShineNum(&isGameClear, actor);
    s32 payShine = GameDataFunction::getPayShineNum(actor);
    if (isGameClear)
        payShine = GameDataFunction::getTotalPayShineNum(actor);

    return sead::Mathi::max(0, unlockShine - payShine);
}

void ShineTowerNpc::noBalloon() {
    al::setNerve(this, &NoBalloon);
}

void ShineTowerNpc::startBalloon() {
    mRemainingShineCount = getRemainingShineCount(this);
    if (mRemainingShineCount == 0)
        al::setNerve(this, &NrvShineTowerNpc.GoKoopa);
    else
        al::setNerve(this, &NrvShineTowerNpc.Wait);
}

void ShineTowerNpc::exeWait() {
    if (al::isFirstStep(this)) {
        sead::FixedSafeString<0x40> string;
        string.format("RestShineNum");
        rs::startEventFlow(mEventFlowExecutor, string.cstr());
    }

    al::turnToTarget(this, al::getPlayerActor(this, 0), 2.0f);
    mRemainingShineCount = getRemainingShineCount(this);
    if (mRemainingShineCount == 0) {
        al::setNerve(this, &NrvShineTowerNpc.GoKoopa);
        return;
    }
    rs::updateEventFlow(mEventFlowExecutor);
}

void ShineTowerNpc::exeGoKoopa() {
    if (al::isFirstStep(this))
        rs::startEventFlow(mEventFlowExecutor, "GoKoopa");

    al::turnToTarget(this, al::getPlayerActor(this, 0), 2.0f);
    rs::updateEventFlow(mEventFlowExecutor);
}

void ShineTowerNpc::exeNoBalloon() {
    mRemainingShineCount = getRemainingShineCount(this);
}
