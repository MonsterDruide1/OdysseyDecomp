#include "Boss/Koopa/KoopaStateDemoBattleStart.h"

#include "Library/Area/AreaObjUtil.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/LiveActorGroup.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Boss/BossUtil/BossUtil.h"
#include "Boss/Koopa/KoopaDemoExecutor.h"
#include "MapObj/CapAppearMapParts.h"
#include "Util/DemoUtil.h"
#include "Util/NpcEventFlowUtil.h"
#include "Util/PlayerUtil.h"

namespace {
IUseDemoSkip* getDemoSkip(KoopaStateDemoBattleStart* state) {
    return state;
}

NERVE_IMPL(KoopaStateDemoBattleStart, Prepare);
NERVE_IMPL(KoopaStateDemoBattleStart, Demo);
NERVE_IMPL(KoopaStateDemoBattleStart, Start);
NERVE_IMPL(KoopaStateDemoBattleStart, Skip);
NERVES_MAKE_NOSTRUCT(KoopaStateDemoBattleStart, Prepare, Demo, Start, Skip);
}  // namespace

KoopaStateDemoBattleStart::KoopaStateDemoBattleStart(al::LiveActor* actor,
                                                     KoopaDemoExecutor* demoExecutor, s32 level)
    : al::ActorStateBase("戦闘開始デモ", actor) {
    mBattleLevel = level + 1;
    mIsSkipped = false;
    mDemoStartAreaGroup = nullptr;
    mCapAppearMapPartsGroup = nullptr;
    mDemoExecutor = demoExecutor;
    initNerve(&Prepare, 0);
}

void KoopaStateDemoBattleStart::initDemoStartAreaGroup(const al::ActorInitInfo& info) {
    mDemoStartAreaGroup =
        al::createLinkAreaGroup(mActor, info, "DemoStartArea", "クッパ開始デモ起動エリアグループ",
                                "クッパ開始デモ起動エリア");
}

void KoopaStateDemoBattleStart::setCapAppearMapPartsGroup(
    al::DeriveActorGroup<CapAppearMapParts>* group) {
    mCapAppearMapPartsGroup = group;
}

void KoopaStateDemoBattleStart::appear() {
    NerveStateBase::appear();
    al::invalidateShadow(mActor);
    mIsSkipped = false;
    if (mDemoExecutor->isRunningDemo())
        al::setNerve(this, &Demo);
    else if (mDemoStartAreaGroup)
        al::setNerve(this, &Prepare);
    else
        al::setNerve(this, &Start);
}

void KoopaStateDemoBattleStart::kill() {
    NerveStateBase::kill();
    al::validateShadow(mActor);
    rs::saveShowDemoBossBattleStart(mActor, 6, mBattleLevel);
    if (mCapAppearMapPartsGroup)
        for (s32 i = 0; i < mCapAppearMapPartsGroup->getActorCount(); i++)
            mCapAppearMapPartsGroup->getDeriveActor(i)->killAll();
}

bool KoopaStateDemoBattleStart::isFirstDemo() const {
    return (rs::isAlreadyShowDemoBossBattleStart(mActor, 6, mBattleLevel) & 1) == 0;
}

bool KoopaStateDemoBattleStart::isEnableSkipDemo() const {
    return true;
}

void KoopaStateDemoBattleStart::skipDemo() {
    mIsSkipped = true;
    mDemoExecutor->skip();
    rs::endEventCutSceneDemoBySkip(mActor);
    al::setNerve(this, &Skip);
    kill();
}

void KoopaStateDemoBattleStart::exePrepare() {
    if (!al::isInAreaObj(mDemoStartAreaGroup, rs::getPlayerPos(mActor)))
        return;
    if (!rs::isPlayerOnGround(mActor))
        return;
    if (mDemoExecutor->tryStartBattleStartDemo(this))
        al::setNerve(this, &Demo);
    else
        al::setNerve(this, &Start);
}

void KoopaStateDemoBattleStart::exeStart() {
    if (mDemoExecutor->tryStartBattleStartDemo(getDemoSkip(this)))
        al::setNerve(this, &Demo);
}

void KoopaStateDemoBattleStart::exeDemo() {
    if (al::isFirstStep(this)) {
        if (mCapAppearMapPartsGroup) {
            for (s32 i = 0; i < mCapAppearMapPartsGroup->getActorCount(); i++) {
                rs::addDemoActor(mCapAppearMapPartsGroup->getDeriveActor(i), false);
                mCapAppearMapPartsGroup->getDeriveActor(i)->startWait();
            }
        }
    }
    if (mDemoExecutor->update())
        kill();
}

void KoopaStateDemoBattleStart::exeSkip() {}
