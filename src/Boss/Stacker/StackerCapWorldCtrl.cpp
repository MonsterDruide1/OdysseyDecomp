#include "Boss/Stacker/StackerCapWorldCtrl.h"

#include "Library/Camera/CameraUtil.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/LiveActorGroup.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"

#include "Boss/Stacker/Stacker.h"
#include "MapObj/ElectricWire/ElectricWire.h"
#include "MapObj/ElectricWire/ElectricWireRailKeeper.h"
#include "Util/DemoUtil.h"

namespace {
NERVE_IMPL(StackerCapWorldCtrl, Battle)
NERVE_IMPL(StackerCapWorldCtrl, DemoCamera)
NERVE_IMPL(StackerCapWorldCtrl, ObjAppear)

NERVES_MAKE_STRUCT(StackerCapWorldCtrl, Battle, DemoCamera, ObjAppear)
}  // namespace

StackerCapWorldCtrl::StackerCapWorldCtrl(const char* name) : LiveActor(name) {}

void StackerCapWorldCtrl::init(const al::ActorInitInfo& actorInitInfo) {
    al::initActorSceneInfo(this, actorInitInfo);
    al::initNerve(this, &NrvStackerCapWorldCtrl.Battle, 0);
    al::initActorPoseTQSV(this);
    al::initActorSRT(this, actorInitInfo);
    al::initExecutorEnemyMovement(this, actorInitInfo);
    al::initStageSwitch(this, actorInitInfo);
    al::initActorClipping(this, actorInitInfo);
    al::setClippingInfo(this, 1000.0f, nullptr);
    al::tryListenStageSwitchAppear(this);

    mStacker = new Stacker("スタッカー", nullptr);
    al::initLinksActor(mStacker, actorInitInfo, "Stacker", 0);
    mStacker->makeActorDead();
    s32 linkChildNum = al::calcLinkChildNum(actorInitInfo, "BattleEndAppearObj");
    mBattleEndGroup = new al::LiveActorGroup("スタッカー戦後出現オブジェ", linkChildNum);

    for (s32 i = 0; i < linkChildNum; ++i) {
        LiveActor* actor = al::createLinksActorFromFactory(actorInitInfo, "BattleEndAppearObj", i);
        actor->makeActorDead();
        mBattleEndGroup->registerActor(actor);
    }
    linkChildNum = al::calcLinkChildNum(actorInitInfo, "ElectricWire");
    if (linkChildNum > 0) {
        mElectricWire = new ElectricWire("電線");
        al::initLinksActor(mElectricWire, actorInitInfo, "ElectricWire", 0);
        for (s32 i = 0; i < mElectricWire->get_110(); ++i)
            mElectricWire->getElectricWireRailKeepers()[i]->kill();
    }
    mCameraTicket = al::initObjectCamera(this, actorInitInfo, nullptr, nullptr);
    makeActorDead();
}

void StackerCapWorldCtrl::appear() {
    LiveActor::appear();
    mStacker->appear();
    al::hideModel(mStacker);
    mStacker->startBattleStartDemo();
}

void StackerCapWorldCtrl::exeBattle() {
    if (al::isGreaterEqualStep(this, 5) && al::isDead(mStacker)) {
        al::startCamera(this, mCameraTicket);
        al::setNerve(this, &NrvStackerCapWorldCtrl.Battle);
    }
}

void StackerCapWorldCtrl::exeDemoCamera() {
    if (al::isFirstStep(this)) {
        if (!rs::requestStartDemoWithPlayer(this, false)) {
            al::setNerve(this, &NrvStackerCapWorldCtrl.Battle);

            return;
        }

        for (s32 i = 0; i < mBattleEndGroup->getActorCount(); i++)
            rs::addDemoActor(mBattleEndGroup->getActor(i), false);

        if (mElectricWire) {
            rs::addDemoActor(mElectricWire, false);
            mElectricWire->addDemoActorElectricWirePartsAll();

            for (s32 i = 0; i < mElectricWire->get_110(); i++)
                if ((u32)mElectricWire->get_110() <= (u32)i)
                    rs::addDemoActor(nullptr, false);
                else
                    rs::addDemoActor(mElectricWire->getElectricWireRailKeepers()[i], false);
        }

        rs::addDemoActor(mStacker, false);
    }

    if (al::isGreaterEqualStep(this, 90))
        al::setNerve(this, &NrvStackerCapWorldCtrl.DemoCamera);
}

void StackerCapWorldCtrl::exeObjAppear() {
    if (al::isFirstStep(this)) {
        for (s32 i = 0; i < mBattleEndGroup->getActorCount(); ++i)
            mBattleEndGroup->getActor(i)->appear();
        if (mElectricWire)
            for (s32 i = 0; i < mElectricWire->get_110(); ++i)
                mElectricWire->getElectricWireRailKeepers()[i]->appear();
    }
    if (al::isGreaterEqualStep(this, 90)) {
        rs::requestEndDemoWithPlayer(this);
        if (al::isActiveCamera(mCameraTicket))
            al::endCamera(this, mCameraTicket, -1, false);
        kill();
    }
}
