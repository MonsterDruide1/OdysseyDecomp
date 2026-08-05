#include "Item/YoshiFruitWatcher.h"

#include <math/seadVector.h>

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActor.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Scene/SceneObjUtil.h"
#include "Library/Screen/ScreenFunction.h"

#include "Item/Shine.h"
#include "Item/YoshiFruitShineHolder.h"
#include "Player/Yoshi.h"
#include "System/GameDataUtil.h"
#include "System/SaveObjInfo.h"
#include "Util/DemoUtil.h"
#include "Util/PlayerUtil.h"

namespace {
NERVE_IMPL(YoshiFruitWatcher, Wait);
NERVE_IMPL(YoshiFruitWatcher, DemoRequest);
NERVE_IMPL(YoshiFruitWatcher, GaugeAppear);
NERVE_IMPL(YoshiFruitWatcher, GaugeWait);
NERVE_IMPL(YoshiFruitWatcher, GaugeEnd);
NERVE_IMPL(YoshiFruitWatcher, DemoGauge);
NERVE_IMPL(YoshiFruitWatcher, DemoShine);

NERVES_MAKE_NOSTRUCT(YoshiFruitWatcher, DemoRequest, DemoGauge, DemoShine, Wait, GaugeAppear,
                     GaugeEnd, GaugeWait);

}  // namespace

YoshiFruitWatcher::YoshiFruitWatcher() : al::LiveActor("ヨッシーフルーツ監視者") {
    mFruits.allocBuffer(100, nullptr);
    mGetFruitSaveInfos.allocBuffer(100, nullptr);
}

void YoshiFruitWatcher::initAfterPlacementSceneObj(const al::ActorInitInfo& info) {
    al::initActorSceneInfo(this, info);
    al::initExecutorWatchObj(this, info);
    al::initNerve(this, &Wait, 0);
    // The original dispatches the scene-name virtual here before creating the gauge layout.
    getSceneObjName();

    mGaugeLayout = new al::LayoutActor("ヨッシーフルーツゲージ");
    al::initLayoutActor(mGaugeLayout, al::getLayoutInitInfo(info), "GaugeYoshi", nullptr);

    if (mCurrentHackYoshi)
        mShineHolder->updateHintPos(al::getTrans(mCurrentHackYoshi));

    makeActorAlive();
}

const char* YoshiFruitWatcher::getSceneObjName() const {
    return "ヨッシーフルーツ監視者";
}

void YoshiFruitWatcher::registerShineHolder(YoshiFruitShineHolder* shineHolder) {
    mShineHolder = shineHolder;
}

bool YoshiFruitWatcher::registerFruit(al::LiveActor* fruit, SaveObjInfo* saveObjInfo) {
    mFruits.pushBack(fruit);

    if (rs::isGetYoshiFruit(saveObjInfo)) {
        mGetFruitNum++;
        return false;
    }

    return true;
}

void YoshiFruitWatcher::noticeCurrentHackYoshi(Yoshi* yoshi) {
    mCurrentHackYoshi = yoshi;
}

void YoshiFruitWatcher::noticeGetFruit(al::LiveActor* fruit, SaveObjInfo* saveObjInfo) {
    mGetFruitNum++;
    mGetFruitSaveInfos.pushBack(saveObjInfo);

    if (mShineHolder->getGotShineNum() < mGetFruitNum / 10) {
        al::startFreezeGaugeAction(mGaugeLayout, 10.0f, 0.0f, 10.0f, "Gauge", "Gauge");
        al::setNerve(this, &GaugeAppear);
        return;
    }

    if (al::isNerve(this, &GaugeAppear))
        return;

    al::startFreezeGaugeAction(mGaugeLayout, mGetFruitNum % 10, 0.0f, 10.0f, "Gauge", "Gauge");
    saveGetFruit();

    if (al::isNerve(this, &GaugeEnd))
        return;

    if (al::isNerve(this, &GaugeWait)) {
        al::setNerve(this, &GaugeWait);
        return;
    }

    al::setNerve(this, &GaugeEnd);
}

void YoshiFruitWatcher::saveGetFruit() {
    s32 lastIndex = mGetFruitSaveInfos.size() - 1;
    if (lastIndex >= 0)
        for (u64 i = 0;; i++) {
            SaveObjInfo* saveObjInfo = static_cast<u32>(mGetFruitSaveInfos.size()) <= i ?
                                           nullptr :
                                           mGetFruitSaveInfos.data()[i];
            rs::getYoshiFruit(saveObjInfo);
            if (lastIndex == static_cast<s32>(i))
                break;
        }

    mGetFruitSaveInfos.clear();
}

void YoshiFruitWatcher::control() {
    if (mCurrentHackYoshi)
        mShineHolder->updateHintPos(al::getTrans(mCurrentHackYoshi));
}

void YoshiFruitWatcher::exeWait() {}

void YoshiFruitWatcher::exeGaugeAppear() {
    if (al::isFirstStep(this)) {
        al::startAction(mGaugeLayout, "Appear", nullptr);
        al::appearLayoutIfDead(mGaugeLayout);
    }

    al::LayoutActor* gaugeLayout = mGaugeLayout;
    const al::LiveActor* hostActor = this;
    sead::Vector3f worldPos = {0.0f, 0.0f, 0.0f};
    rs::calcPlayerFollowLayoutWorldPos(&worldPos, hostActor);
    sead::Vector2f layoutPos = {0.0f, 0.0f};
    al::calcLayoutPosFromWorldPos(&layoutPos, hostActor, worldPos);
    al::setLocalTrans(gaugeLayout, layoutPos);

    if (al::isActionEnd(mGaugeLayout, nullptr))
        al::setNerve(this, &GaugeWait);
}

void YoshiFruitWatcher::exeGaugeWait() {
    if (al::isFirstStep(this))
        al::startAction(mGaugeLayout, "Wait", nullptr);

    al::LayoutActor* gaugeLayout = mGaugeLayout;
    const al::LiveActor* hostActor = this;
    sead::Vector3f worldPos = {0.0f, 0.0f, 0.0f};
    rs::calcPlayerFollowLayoutWorldPos(&worldPos, hostActor);
    sead::Vector2f layoutPos = {0.0f, 0.0f};
    al::calcLayoutPosFromWorldPos(&layoutPos, hostActor, worldPos);
    al::setLocalTrans(gaugeLayout, layoutPos);

    if (!al::isLessStep(this, 120))
        al::setNerve(this, &DemoRequest);
}

void YoshiFruitWatcher::exeGaugeEnd() {
    if (al::isFirstStep(this))
        al::startAction(mGaugeLayout, "End", nullptr);

    al::LayoutActor* gaugeLayout = mGaugeLayout;
    const al::LiveActor* hostActor = this;
    sead::Vector3f worldPos = {0.0f, 0.0f, 0.0f};
    rs::calcPlayerFollowLayoutWorldPos(&worldPos, hostActor);
    sead::Vector2f layoutPos = {0.0f, 0.0f};
    al::calcLayoutPosFromWorldPos(&layoutPos, hostActor, worldPos);
    al::setLocalTrans(gaugeLayout, layoutPos);

    if (al::isActionEnd(mGaugeLayout, nullptr)) {
        mGaugeLayout->kill();
        al::setNerve(this, &Wait);
    }
}

void YoshiFruitWatcher::exeDemoRequest() {
    if (rs::requestStartDemoNormal(this, false))
        al::setNerve(this, &DemoGauge);
}

void YoshiFruitWatcher::exeDemoGauge() {
    if (al::isFirstStep(this)) {
        al::startAction(mGaugeLayout, "Complete", nullptr);
        al::appearLayoutIfDead(mGaugeLayout);

        if (mCurrentHackYoshi)
            mCurrentHackYoshi->startFruitShineGetDemo();
    }

    {
        const al::LiveActor* hostActor = this;
        al::LayoutActor* gaugeLayout = mGaugeLayout;
        sead::Vector3f worldPos = {0.0f, 0.0f, 0.0f};
        rs::calcPlayerFollowLayoutWorldPos(&worldPos, this);
        sead::Vector2f layoutPos = {0.0f, 0.0f};
        al::calcLayoutPosFromWorldPos(&layoutPos, hostActor, worldPos);
        al::setLocalTrans(gaugeLayout, layoutPos);
    }

    if (!al::isActionEnd(mGaugeLayout, nullptr))
        return;

    sead::Vector3f playerPos = rs::getPlayerPos(this);
    sead::Vector2f paneTrans = {0.0f, 0.0f};
    al::calcPaneTrans(&paneTrans, mGaugeLayout, "PicGauge00");

    sead::Vector3f shinePos = {0.0f, 0.0f, 0.0f};
    sead::Vector3f playerPosOffset = playerPos + sead::Vector3f::ey * 200.0f;
    al::calcWorldPosFromLayoutPos(&shinePos, this, paneTrans, playerPosOffset);

    mDemoShine = mShineHolder->appearShineFromFruit(shinePos);
    mGaugeLayout->kill();
    rs::addDemoActor(mDemoShine, false);
    al::setNerve(this, &DemoShine);
}

void YoshiFruitWatcher::exeDemoShine() {
    if (!mDemoShine->isEndAppear())
        return;

    if (!al::isGreaterEqualStep(this, 120))
        return;

    saveGetFruit();
    rs::requestEndDemoNormal(this);
    al::startFreezeGaugeAction(mGaugeLayout, mGetFruitNum % 10, 0.0f, 10.0f, "Gauge", "Gauge");
    mDemoShine->getDirectWithDemo();

    if (mGetFruitNum % 10 != 0)
        al::setNerve(this, &GaugeEnd);
    else
        al::setNerve(this, &Wait);
}

namespace rs {
bool registerFruit(al::LiveActor* fruit, SaveObjInfo* saveObjInfo) {
    YoshiFruitWatcher* watcher = al::createSceneObj<YoshiFruitWatcher>(fruit);
    return watcher->registerFruit(fruit, saveObjInfo);
}

void registerFruitShineHolder(YoshiFruitShineHolder* shineHolder) {
    YoshiFruitWatcher* watcher = al::createSceneObj<YoshiFruitWatcher>(shineHolder);
    watcher->registerShineHolder(shineHolder);
}

void noticeCurrentHackYoshi(Yoshi* yoshi) {
    if (al::isExistSceneObj<YoshiFruitWatcher>(yoshi))
        al::getSceneObj<YoshiFruitWatcher>(yoshi)->noticeCurrentHackYoshi(yoshi);
}

void noticeGetFruit(al::LiveActor* fruit, SaveObjInfo* saveObjInfo) {
    al::getSceneObj<YoshiFruitWatcher>(fruit)->noticeGetFruit(fruit, saveObjInfo);
}
}  // namespace rs
