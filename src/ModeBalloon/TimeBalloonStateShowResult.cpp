#include "ModeBalloon/TimeBalloonStateShowResult.h"

#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "ModeBalloon/TimeBalloonAchievementLayout.h"
#include "Util/StageInputFunction.h"

namespace {
NERVE_IMPL(TimeBalloonStateShowResult, Appear)
NERVE_IMPL(TimeBalloonStateShowResult, Wait)
NERVE_IMPL(TimeBalloonStateShowResult, End)

NERVES_MAKE_NOSTRUCT(TimeBalloonStateShowResult, Appear, Wait, End)
}  // namespace

TimeBalloonStateShowResult::TimeBalloonStateShowResult(
    StageSceneStateTimeBalloon* timeBalloon, al::SimpleLayoutAppearWaitEnd* layout,
    const IBalloonFindMyAchievementHolder* achievementHolder, const al::LayoutInitInfo& info)
    : al::NerveStateBase("結果を見る(風船割りゲーム)"), mTimeBalloon(timeBalloon),
      mAchievementHolder(achievementHolder) {
    mAchievementLayout = new TimeBalloonAchievementLayout(info);
}

void TimeBalloonStateShowResult::init() {
    initNerve(&Appear, 0);
}

void TimeBalloonStateShowResult::appear() {
    al::NerveStateBase::appear();
    al::setNerve(this, &Appear);
}

void TimeBalloonStateShowResult::exeAppear() {
    if (al::isFirstStep(this)) {
        mAchievementLayout->setAchievement(mAchievementHolder);
        mAchievementLayout->appear();
    }
    mAchievementLayout->update();
    if (mAchievementLayout->isWait())
        al::setNerve(this, &Wait);
}

void TimeBalloonStateShowResult::exeWait() {
    mAchievementLayout->update();
    if (rs::isTriggerUiCancel(mAchievementLayout))
        al::setNerve(this, &End);
}

void TimeBalloonStateShowResult::exeEnd() {
    if (al::isFirstStep(this))
        mAchievementLayout->end();
    if (!mAchievementLayout->isAlive())
        kill();
}

void TimeBalloonStateShowResult::setNerveWait() {
    al::setNerve(this, &Wait);
}
