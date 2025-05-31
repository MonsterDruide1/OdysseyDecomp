#pragma once

#include <math/seadVector.h>

#include "Library/Nerve/NerveExecutor.h"

namespace al {
class LayoutActor;
class LayoutInitInfo;
class PlayerHolder;
class SubCameraRenderer;
}  // namespace al
class CounterLife;

class CounterLifeCtrl : public al::NerveExecutor {
public:
    CounterLifeCtrl(const al::LayoutInitInfo& info, const al::PlayerHolder* player_holder,
                    const al::SubCameraRenderer* sub_camera_renderer);

    void setCount(s32 count);
    bool tryUpdateCount(s32 count);
    bool tryStartDemoLifeUp(bool is_hack_koopa);
    void appear();
    void end();
    void kill();
    void killAllLayout();
    bool isEndLifeDemo() const;

    void exeAppear();
    void setTransAllLayout(const sead::Vector3f& trans);
    const sead::Vector3f& getInitTrans() const;
    void appearAllLayout();
    void startAllLayout();
    CounterLife* getCurrentLayout() const;
    void exeWait();
    void waitAllLayout();
    void updateTransAllLayout();
    bool tryChangeCount();
    void exeEnd();
    void exeDead();
    void exeCountStartFadeOut();
    void exeCountStartFadeIn();
    void exeCount();
    void startCountAnim(s32 target);
    void exeCountEnd();
    void exeDemoLifeMaxUpStartFadeIn();
    void updateTrans(al::LayoutActor* layout);
    void exeDemoLifeMaxUpAddLife();
    void exeDemoLifeMaxUpWaitForMove();
    void exeDemoLifeMaxUpMove();
    void calcLayoutTransByPlayer(sead::Vector3f* out);
    void exeDemoLifeMaxUpUnite();
    void startActionAllLayout(const char* action_name, const char* pane_name);

private:
    const al::PlayerHolder* mPlayerHolder;
    CounterLife* mCounterLife = nullptr;
    CounterLife* mCounterLifeUp = nullptr;
    CounterLife* mCounterLifeKids = nullptr;
    sead::Vector3f mCounterLifeInitTrans = sead::Vector3f::zero;
    sead::Vector3f mCounterLifeKidsInitTrans = sead::Vector3f::zero;
    sead::Vector3f mCountEndTrans = sead::Vector3f::zero;
    s32 mCount = 0;
    s32 mTargetCount = 0;
    const al::SubCameraRenderer* mSubCameraRenderer;
    s32 mSubjectiveCameraWaitTime = 0;
    bool mIsHackKoopa = false;
};

static_assert(sizeof(CounterLifeCtrl) == 0x70);
