#include "Layout/CounterLifeCtrl.h"

#include "Library/Camera/CameraUtil.h"
#include "Library/Draw/SubCameraRenderer.h"
#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Player/PlayerUtil.h"
#include "Library/Screen/ScreenFunction.h"
#include "Library/Se/SeFunction.h"

#include "Layout/CounterLife.h"
#include "System/GameDataFunction.h"
#include "System/GameDataUtil.h"
#include "Util/PlayerUtil.h"

namespace {
NERVE_IMPL(CounterLifeCtrl, Dead);
NERVE_IMPL(CounterLifeCtrl, CountStartFadeIn);
NERVE_IMPL(CounterLifeCtrl, DemoLifeMaxUpStartFadeIn);
NERVE_IMPL(CounterLifeCtrl, Appear);
NERVE_IMPL(CounterLifeCtrl, Wait);
NERVE_IMPL(CounterLifeCtrl, End);
NERVE_IMPL(CounterLifeCtrl, Count);
NERVE_IMPL(CounterLifeCtrl, CountEnd);
NERVE_IMPL(CounterLifeCtrl, DemoLifeMaxUpAddLife);
NERVE_IMPL(CounterLifeCtrl, DemoLifeMaxUpWaitForMove);
NERVE_IMPL(CounterLifeCtrl, DemoLifeMaxUpMove);
NERVE_IMPL(CounterLifeCtrl, DemoLifeMaxUpUnite);
NERVE_IMPL(CounterLifeCtrl, CountStartFadeOut);

NERVES_MAKE_NOSTRUCT(CounterLifeCtrl, Dead, CountStartFadeIn, End, DemoLifeMaxUpWaitForMove);
NERVES_MAKE_STRUCT(CounterLifeCtrl, DemoLifeMaxUpStartFadeIn, Appear, Wait, Count, CountEnd,
                   DemoLifeMaxUpAddLife, DemoLifeMaxUpMove, DemoLifeMaxUpUnite, CountStartFadeOut);
}  // namespace

CounterLifeCtrl::CounterLifeCtrl(const al::LayoutInitInfo& info,
                                 const al::PlayerHolder* player_holder,
                                 const al::SubCameraRenderer* sub_camera_renderer)
    : al::NerveExecutor("ライフカウンタ更新"), mPlayerHolder(player_holder),
      mSubCameraRenderer(sub_camera_renderer) {
    mCounterLife = new CounterLife("[ライフ]ライフカウンタ", "CounterLife", info);
    mCounterLife->kill();
    mCounterLifeKids = new CounterLife("[ライフ]キッズ用ライフカウンタ", "CounterLifeKids", info);
    mCounterLifeKids->kill();
    mCounterLifeUp = new CounterLife("[ライフ]最大ライフカウンタ", "CounterLifeUp", info);
    mCounterLifeUp->kill();
    initNerve(&Dead, 0);
    setCount(GameDataFunction::getPlayerHitPoint(mCounterLife));
    mCounterLifeInitTrans = al::getPaneLocalTrans(mCounterLife, "All");
    mCounterLifeKidsInitTrans = al::getPaneLocalTrans(mCounterLifeKids, "All");
}

static bool isDemoLifeMaxUp(CounterLifeCtrl* ctrl) {
    return al::isNerve(ctrl, &NrvCounterLifeCtrl.DemoLifeMaxUpStartFadeIn) ||
           al::isNerve(ctrl, &NrvCounterLifeCtrl.DemoLifeMaxUpAddLife) ||
           al::isNerve(ctrl, &NrvCounterLifeCtrl.DemoLifeMaxUpMove) ||
           al::isNerve(ctrl, &NrvCounterLifeCtrl.DemoLifeMaxUpUnite);
}

static s32 calcLifeUpCount(const al::IUseSceneObjHolder* game_data_holder, s32 count) {
    return sead::Mathi::clampMin(
        count - GameDataFunction::getPlayerHitPointMaxNormal(game_data_holder), 0);
}

static f32 calcGaugeFrame(CounterLife* counter, s32 count, s32 max) {
    f32 max_frame = al::getActionFrameMax(counter, "Gauge", "Gauge");
    return max_frame - max_frame * (static_cast<f32>(count) / static_cast<f32>(max));
}

static void setGaugeCount(CounterLife* counter, s32 count, s32 max) {
    counter->setCount(calcGaugeFrame(counter, count, max));
}

void CounterLifeCtrl::setCount(s32 count) {
    mCount = count;
    mTargetCount = count;

    setGaugeCount(mCounterLife, mCount, GameDataFunction::getPlayerHitPointMaxNormal(mCounterLife));
    setGaugeCount(mCounterLifeUp, calcLifeUpCount(mCounterLifeUp, mCount), 3);
    setGaugeCount(mCounterLifeKids, sead::Mathi::clamp(mCount, 0, 6),
                  GameDataFunction::getPlayerHitPointMaxNormal(mCounterLifeKids));

    if (!isDemoLifeMaxUp(this)) {
        al::setPaneStringFormat(mCounterLife, "TxtLife", al::StringTmp<32>("%d", count).cstr());
        al::setPaneStringFormat(mCounterLifeUp, "TxtLife", al::StringTmp<32>("%d", count).cstr());
        al::setPaneStringFormat(mCounterLifeKids, "TxtLife", al::StringTmp<32>("%d", count).cstr());
    }
}

bool CounterLifeCtrl::tryUpdateCount(s32 count) {
    if (count == mTargetCount)
        return false;
    setCount(mTargetCount);
    mTargetCount = count;
    al::setNerve(this, &CountStartFadeIn);
    return true;
}

bool CounterLifeCtrl::tryStartDemoLifeUp(bool is_hack_koopa) {
    if (!GameDataFunction::isPlayerHitPointMaxWithItem(mCounterLife) ||
        mTargetCount == GameDataFunction::getPlayerHitPointMaxCurrent(mCounterLife))
        return false;
    mTargetCount = GameDataFunction::getPlayerHitPointMaxCurrent(mCounterLife);
    mIsHackKoopa = is_hack_koopa;
    if (is_hack_koopa) {
        s32 count = GameDataFunction::getPlayerHitPointMaxNormal(mCounterLife);
        mCount = count;
        setCount(count);
    }
    al::setNerve(this, &NrvCounterLifeCtrl.DemoLifeMaxUpStartFadeIn);
    return true;
}

void CounterLifeCtrl::appear() {
    if (al::isNerve(this, &NrvCounterLifeCtrl.Appear) ||
        al::isNerve(this, &NrvCounterLifeCtrl.Wait))
        return;
    al::setPaneLocalAlpha(mCounterLife, "Life", 255);
    al::setPaneLocalAlpha(mCounterLifeKids, "Life", 255);
    al::setNerve(this, &NrvCounterLifeCtrl.Appear);
}

void CounterLifeCtrl::end() {
    al::setNerve(this, &End);
}

void CounterLifeCtrl::kill() {
    killAllLayout();
    al::setNerve(this, &Dead);
}

void CounterLifeCtrl::killAllLayout() {
    al::killLayoutIfActive(mCounterLife);
    al::killLayoutIfActive(mCounterLifeUp);
    al::killLayoutIfActive(mCounterLifeKids);
}

bool CounterLifeCtrl::isEndLifeDemo() const {
    return al::isNerve(this, &NrvCounterLifeCtrl.Wait);
}

void CounterLifeCtrl::exeAppear() {
    if (al::isFirstStep(this)) {
        setTransAllLayout(getInitTrans());
        appearAllLayout();
        startAllLayout();
        setCount(GameDataFunction::getPlayerHitPoint(mCounterLife));
    }
    if (getCurrentLayout()->isWait())
        al::setNerve(this, &NrvCounterLifeCtrl.Wait);
}

void CounterLifeCtrl::setTransAllLayout(const sead::Vector3f& trans) {
    al::setPaneLocalTrans(mCounterLife, "All", trans);
    al::setPaneLocalTrans(mCounterLifeUp, "All", trans);
    al::setPaneLocalTrans(mCounterLifeKids, "All", trans);
}

const sead::Vector3f& CounterLifeCtrl::getInitTrans() const {
    return rs::isKidsMode(mCounterLife) ? mCounterLifeKidsInitTrans : mCounterLifeInitTrans;
}

static bool isActiveCounterLifeUp(const al::IUseSceneObjHolder* game_data_holder) {
    return GameDataFunction::getPlayerHitPoint(game_data_holder) >
           GameDataFunction::getPlayerHitPointMaxNormal(game_data_holder);
}

void CounterLifeCtrl::appearAllLayout() {
    if (rs::isKidsMode(mCounterLifeKids))
        mCounterLifeKids->appear();
    else
        mCounterLife->appear();
    if (isActiveCounterLifeUp(mCounterLifeUp))
        mCounterLifeUp->appear();
}

void CounterLifeCtrl::startAllLayout() {
    if (rs::isKidsMode(mCounterLifeKids))
        mCounterLifeKids->start();
    else
        mCounterLife->start();
    if (isActiveCounterLifeUp(mCounterLifeUp))
        mCounterLifeUp->start();
}

CounterLife* CounterLifeCtrl::getCurrentLayout() const {
    return rs::isKidsMode(mCounterLife) ? mCounterLifeKids : mCounterLife;
}

void CounterLifeCtrl::exeWait() {
    if (al::isFirstStep(this))
        waitAllLayout();
    if (rs::isEmptyPlayerOxygen(al::getPlayerActor(mPlayerHolder, 0)))
        updateTransAllLayout();
    else
        setTransAllLayout(getInitTrans());
    if (rs::isPlayerCameraSubjective(al::getPlayerActor(mPlayerHolder, 0)))
        mSubjectiveCameraWaitTime = 5;
    else
        mSubjectiveCameraWaitTime = sead::Mathi::clampMin(mSubjectiveCameraWaitTime - 1, 0);
    tryChangeCount();
}

void CounterLifeCtrl::waitAllLayout() {
    if (rs::isKidsMode(mCounterLifeKids))
        mCounterLifeKids->wait();
    else
        mCounterLife->wait();
    if (isActiveCounterLifeUp(mCounterLifeUp))
        mCounterLifeUp->wait();
    else if (al::isActive(mCounterLifeUp) && !al::isActionPlaying(mCounterLifeUp, "Break", "Life"))
        al::startAction(mCounterLifeUp, "Break", "Life");
    setCount(GameDataFunction::getPlayerHitPoint(mCounterLife));
}

void CounterLifeCtrl::updateTransAllLayout() {
    updateTrans(mCounterLife);
    updateTrans(mCounterLifeUp);
    updateTrans(mCounterLifeKids);
}

bool CounterLifeCtrl::tryChangeCount() {
    if (GameDataFunction::getPlayerHitPoint(mCounterLife) == mTargetCount)
        return false;
    mTargetCount = GameDataFunction::getPlayerHitPoint(mCounterLife);
    if (rs::isEmptyPlayerOxygen(al::getPlayerActor(mPlayerHolder, 0)))
        al::setNerve(this, &NrvCounterLifeCtrl.Count);
    else
        al::setNerve(this, &NrvCounterLifeCtrl.CountStartFadeOut);
    return true;
}

void CounterLifeCtrl::exeEnd() {
    if (al::isFirstStep(this)) {
        mCounterLife->end();
        mCounterLifeKids->end();
        mCounterLifeUp->end();
    }
    if (al::isDead(getCurrentLayout()))
        al::setNerve(this, &Dead);
}

void CounterLifeCtrl::exeDead() {
    if (al::isFirstStep(this))
        killAllLayout();
}

void CounterLifeCtrl::exeCountStartFadeOut() {
    if (al::isFirstStep(this)) {
        appearAllLayout();
        startActionAllLayout("FadeOut", nullptr);
    }
    if (al::isActionEnd(getCurrentLayout(), nullptr))
        al::setNerve(this, &CountStartFadeIn);
}

void CounterLifeCtrl::exeCountStartFadeIn() {
    updateTransAllLayout();
    if (al::isFirstStep(this)) {
        al::startAction(mCounterLife, "FadeIn", nullptr);
        al::startAction(mCounterLifeKids, "FadeIn", nullptr);
        al::startAction(mCounterLifeUp, "FadeIn", nullptr);
        appearAllLayout();
    }
    if (al::isActionEnd(getCurrentLayout(), nullptr))
        al::setNerve(this, &NrvCounterLifeCtrl.Count);
}

void CounterLifeCtrl::exeCount() {
    if (al::isFirstStep(this))
        startCountAnim(mTargetCount);
    if (mTargetCount == mCount) {
        if (mTargetCount != GameDataFunction::getPlayerHitPoint(mCounterLife)) {
            mTargetCount = GameDataFunction::getPlayerHitPoint(mCounterLife);
            al::setNerve(this, &NrvCounterLifeCtrl.Count);
            return;
        }
        getCurrentLayout();
        if (al::isGreaterEqualStep(this, 80)) {
            if (rs::isEmptyPlayerOxygen(al::getPlayerActor(mPlayerHolder, 0)))
                al::setNerve(this, &NrvCounterLifeCtrl.Wait);
            else
                al::setNerve(this, &NrvCounterLifeCtrl.CountEnd);
        }
        return;
    }
    if (al::isGreaterEqualStep(this, 12))
        al::setNerve(this, &NrvCounterLifeCtrl.Count);
}

static void startGaugeAnim(CounterLifeCtrl* ctrl, CounterLife* counter, s32 count, s32 max) {
    f32 frame = calcGaugeFrame(counter, count, max);
    if (isDemoLifeMaxUp(ctrl))
        counter->startGaugeWithFrame(frame, al::getActionFrameMax(counter, "AddMax", "Life"));
    else
        counter->startGauge(frame);
}

void CounterLifeCtrl::startCountAnim(s32 target) {
    s32 count = mCount;
    if (count == target) {
        startGaugeAnim(this, mCounterLife, mCount,
                       GameDataFunction::getPlayerHitPointMaxNormal(mCounterLife));
        startGaugeAnim(this, mCounterLifeUp, calcLifeUpCount(mCounterLifeUp, mCount), 3);
        startGaugeAnim(this, mCounterLifeKids, sead::Mathi::clamp(mCount, 0, 6),
                       GameDataFunction::getPlayerHitPointMaxNormal(mCounterLifeKids));
        return;
    }

    mCount += target < mCount ? -1 : 1;
    s32 new_count = mCount;
    startGaugeAnim(this, mCounterLife, new_count,
                   GameDataFunction::getPlayerHitPointMaxNormal(mCounterLife));
    startGaugeAnim(this, mCounterLifeUp, calcLifeUpCount(mCounterLifeUp, new_count), 3);
    startGaugeAnim(this, mCounterLifeKids, sead::Mathi::clamp(new_count, 0, 6),
                   GameDataFunction::getPlayerHitPointMaxNormal(mCounterLifeKids));

    bool is_add_max = al::isNerve(this, &NrvCounterLifeCtrl.DemoLifeMaxUpAddLife);
    if (count > target) {
        if ((mCount == 3 && !rs::isKidsMode(mCounterLife)) || mCount == 6) {
            al::startAction(mCounterLifeUp, "Break", "Life");
        } else if (mCount < GameDataFunction::getPlayerHitPointMaxNormal(mCounterLife)) {
            al::startAction(getCurrentLayout(), "Damage", "Life");
            if (mCount == 1) {
                CounterLife* counter =
                    rs::isKidsMode(mCounterLifeKids) ? mCounterLifeKids : mCounterLife;
                al::startHitReaction(counter, "ライフ1警告", nullptr);
            }
        } else {
            al::startAction(mCounterLifeUp, "Damage", "Life");
        }
    } else {
        const char* action = "Add";
        if (is_add_max && !mIsHackKoopa)
            action = "AddMax";
        if (mCount > GameDataFunction::getPlayerHitPointMaxNormal(mCounterLife)) {
            al::startAction(mCounterLifeUp, action, "Life");
        } else {
            al::startAction(mCounterLife, action, "Life");
            al::startAction(mCounterLifeKids, action, "Life");
        }
        al::tryStopSe(mCounterLife, "LifeAlert", -1, nullptr);
    }

    if (isDemoLifeMaxUp(this)) {
        al::setPaneStringFormat(
            mCounterLife, "TxtLife",
            al::StringTmp<32>(
                "%d", sead::Mathi::min(mCount,
                                       GameDataFunction::getPlayerHitPointMaxNormal(mCounterLife)))
                .cstr());
        al::setPaneStringFormat(
            mCounterLifeKids, "TxtLife",
            al::StringTmp<32>(
                "%d", sead::Mathi::min(mCount,
                                       GameDataFunction::getPlayerHitPointMaxNormal(mCounterLife)))
                .cstr());
        al::setPaneStringFormat(
            mCounterLifeUp, "TxtLife",
            al::StringTmp<32>(
                "%d", sead::Mathi::clampMin(
                          mCount - GameDataFunction::getPlayerHitPointMaxNormal(mCounterLife), 0))
                .cstr());
    } else {
        al::setPaneStringFormat(mCounterLife, "TxtLife", al::StringTmp<32>("%d", mCount).cstr());
        al::setPaneStringFormat(mCounterLifeUp, "TxtLife", al::StringTmp<32>("%d", mCount).cstr());
        al::setPaneStringFormat(mCounterLifeKids, "TxtLife",
                                al::StringTmp<32>("%d", mCount).cstr());
    }

    if (GameDataFunction::getPlayerHitPoint(mCounterLife) > 1) {
        al::startFreezeAction(mCounterLife, "Loop", 0, "Loop");
        al::startFreezeAction(mCounterLifeKids, "Loop", 0, "Loop");
    }
}

void CounterLifeCtrl::exeCountEnd() {
    if (al::isFirstStep(this))
        mCountEndTrans = al::getPaneLocalTrans(mCounterLife, "All");
    sead::Vector3f pos = sead::Vector3f::zero;
    al::lerpVec(&pos, mCountEndTrans, getInitTrans(), al::calcNerveEaseInRate(this, 27));
    setTransAllLayout(pos);
    if (al::isGreaterEqualStep(this, 27)) {
        setTransAllLayout(getInitTrans());
        al::setNerve(this, &NrvCounterLifeCtrl.Wait);
    }
}

void CounterLifeCtrl::exeDemoLifeMaxUpStartFadeIn() {
    CounterLife* counter = mCounterLifeUp;
    if (al::isFirstStep(this)) {
        appearAllLayout();
        getCurrentLayout()->start();
        al::setPaneLocalTrans(getCurrentLayout(), "All", mCounterLifeInitTrans);
        counter->setEmpty();
        al::startAction(counter, "FadeIn", nullptr);
        startGaugeAnim(this, counter, 0, 3);
        updateTrans(counter);
        al::setPaneStringFormat(mCounterLifeUp, "TxtLife", al::StringTmp<32>("3").cstr());
    }
    if (al::isActionEnd(counter, nullptr))
        al::setNerve(this, &NrvCounterLifeCtrl.DemoLifeMaxUpAddLife);
}

void CounterLifeCtrl::updateTrans(al::LayoutActor* layout) {
    sead::Vector3f pos = sead::Vector3f::zero;
    calcLayoutTransByPlayer(&pos);
    sead::Vector2f pos2 = {pos.x, pos.y};
    al::setPaneLocalTrans(layout, "All", pos2);
}

void CounterLifeCtrl::exeDemoLifeMaxUpAddLife() {
    if (al::isFirstStep(this))
        mTargetCount = GameDataFunction::getPlayerHitPointMaxCurrent(mCounterLife);
    if (al::isIntervalStep(this, 12, 0))
        startCountAnim(mTargetCount);
    if (mTargetCount == mCount)
        al::setNerve(this, &DemoLifeMaxUpWaitForMove);
}

void CounterLifeCtrl::exeDemoLifeMaxUpWaitForMove() {
    if (al::isGreaterEqualStep(this, 30))
        al::setNerve(this, &NrvCounterLifeCtrl.DemoLifeMaxUpMove);
}

void CounterLifeCtrl::exeDemoLifeMaxUpMove() {
    sead::Vector3f pos = sead::Vector3f::zero;
    calcLayoutTransByPlayer(&pos);
    al::lerpVec(&pos, pos, getInitTrans(), al::calcNerveEaseInRate(this, 27));
    al::setPaneLocalTrans(mCounterLifeUp, "All", pos);
    if (al::isGreaterEqualStep(this, 27))
        al::setNerve(this, &NrvCounterLifeCtrl.DemoLifeMaxUpUnite);
}

void CounterLifeCtrl::calcLayoutTransByPlayer(sead::Vector3f* out) {
    al::LiveActor* player = al::getPlayerActor(mPlayerHolder, 0);
    if (rs::isPlayerCameraSubjective(player) || mSubjectiveCameraWaitTime > 0) {
        out->set(getInitTrans());
        return;
    }
    sead::Vector3f pos = sead::Vector3f::zero;
    rs::calcPlayerFollowLayoutWorldPos(&pos, player);
    if (rs::isPlayer2D(player) && al::isValidView(al::getSceneCameraInfo(player), 1))
        mSubCameraRenderer->calcOnScreenPos(&pos, pos);
    sead::Vector3f side = {0, 0, 0};
    al::calcCameraSideDir(&side, player, 0);
    sead::Vector3f up = {0, 0, 0};
    al::calcCameraUpDir(&up, player, 0);
    sead::Vector2f layout_pos = {0, 0};
    al::calcLayoutPosFromWorldPos(&layout_pos, player, pos + side * 30 + up * 25);
    sead::Vector2f layout_offset = {0, 24};
    out->x = layout_pos.x + layout_offset.x;
    out->y = layout_pos.y + layout_offset.y;
    f32 min_x = -0.5f * al::getLayoutDisplayWidth() + 100;
    f32 max_x = 0.5f * al::getLayoutDisplayWidth() - 100;
    out->x = sead::Mathf::clamp(out->x, min_x, max_x);
    f32 min_y = -0.5f * al::getLayoutDisplayHeight() + 100;
    f32 max_y = 0.5f * al::getLayoutDisplayHeight() - 100;
    out->y = sead::Mathf::clamp(out->y, min_y, max_y);
    out->z = getInitTrans().z;
}

void CounterLifeCtrl::exeDemoLifeMaxUpUnite() {
    if (al::isFirstStep(this)) {
        al::startAction(mCounterLifeUp, "Unite", "Life");
        al::startAction(mCounterLife, "Unite", "Life");
        al::startAction(mCounterLifeKids, "Unite", "Life");
        al::setPaneStringFormat(
            mCounterLifeUp, "TxtLife",
            al::StringTmp<32>("%d", GameDataFunction::getPlayerHitPointMaxCurrent(mCounterLife))
                .cstr());
        if (mIsHackKoopa)
            al::startHitReaction(getCurrentLayout(), "合体[クッパ]", nullptr);
        else
            al::startHitReaction(getCurrentLayout(), "合体[通常]", nullptr);
        return;
    }
    if (al::isActionEnd(mCounterLifeUp, nullptr))
        al::setNerve(this, &NrvCounterLifeCtrl.Wait);
}

void CounterLifeCtrl::startActionAllLayout(const char* action_name, const char* pane_name) {
    al::startAction(mCounterLife, action_name, pane_name);
    al::startAction(mCounterLifeUp, action_name, pane_name);
    al::startAction(mCounterLifeKids, action_name, pane_name);
}
