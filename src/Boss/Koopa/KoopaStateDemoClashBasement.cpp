#include "Boss/Koopa/KoopaStateDemoClashBasement.h"

#include "Library/Area/AreaObjGroup.h"
#include "Library/Area/AreaObjUtil.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "Boss/BossUtil/BossUtil.h"
#include "Boss/Koopa/KoopaDemoExecutor.h"
#include "Util/DemoUtil.h"
#include "Util/PlayerUtil.h"

namespace {
IUseDemoSkip* getDemoSkip(KoopaStateDemoClashBasement* state) {
    return state;
}

NERVE_IMPL(KoopaStateDemoClashBasement, Prepare);
NERVE_IMPL(KoopaStateDemoClashBasement, Start);
NERVE_IMPL(KoopaStateDemoClashBasement, Demo);
NERVE_IMPL(KoopaStateDemoClashBasement, Skip);
NERVE_IMPL(KoopaStateDemoClashBasement, End);
NERVES_MAKE_NOSTRUCT(KoopaStateDemoClashBasement, Prepare, Start, Demo, Skip, End);
}  // namespace

KoopaStateDemoClashBasement*
KoopaStateDemoClashBasement::tryCreate(al::LiveActor* actor, const al::ActorInitInfo& info,
                                       KoopaDemoExecutor* demoExecutor) {
    if (!al::isExistLinkChild(info, "DemoClashBasementArea", 0))
        return nullptr;

    al::AreaObjGroup* areaGroup =
        al::createLinkAreaGroup(actor, info, "DemoClashBasementArea",
                                "クッパ崩落デモ起動エリアグループ", "クッパ崩落デモ起動エリア");
    return new KoopaStateDemoClashBasement(actor, demoExecutor, areaGroup);
}

KoopaStateDemoClashBasement::KoopaStateDemoClashBasement(al::LiveActor* actor,
                                                         KoopaDemoExecutor* demoExecutor,
                                                         const al::AreaObjGroup* areaGroup)
    : al::ActorStateBase("崩落開始デモ", actor), mDemoExecutor(demoExecutor),
      mAreaGroup(areaGroup) {
    initNerve(&Prepare, 0);
}

void KoopaStateDemoClashBasement::appear() {
    NerveStateBase::appear();
    if (mAreaGroup) {
        al::setNerve(this, &Prepare);
        return;
    }

    al::setNerve(this, &Start);
}

void KoopaStateDemoClashBasement::kill() {
    NerveStateBase::kill();
    rs::saveShowDemoMoonBasementCollapse(mActor);
    al::onStageSwitch(mActor, "SwitchDemoClashBasementAfterOn");
}

bool KoopaStateDemoClashBasement::isFirstDemo() const {
    return !rs::isAlreadyShowDemoMoonBasementCollapse(mActor);
}

bool KoopaStateDemoClashBasement::isEnableSkipDemo() const {
    return true;
}

void KoopaStateDemoClashBasement::skipDemo() {
    mDemoExecutor->skip();
    rs::requestEndDemoWithPlayer(mActor);
    al::setNerve(this, &Skip);
    kill();
}

void KoopaStateDemoClashBasement::exePrepare() {
    if (!al::isInAreaObj(mAreaGroup, rs::getPlayerPos(mActor)))
        return;

    if (mDemoExecutor->tryStartClashBasementDemo(this)) {
        al::setNerve(this, &Demo);
        return;
    }

    al::setNerve(this, &Start);
}

void KoopaStateDemoClashBasement::exeStart() {
    if (mDemoExecutor->tryStartClashBasementDemo(getDemoSkip(this)))
        al::setNerve(this, &Demo);
}

void KoopaStateDemoClashBasement::exeDemo() {
    if (!mDemoExecutor->update())
        return;

    al::setNerve(this, &End);
    kill();
}

void KoopaStateDemoClashBasement::exeSkip() {}

void KoopaStateDemoClashBasement::exeEnd() {}
