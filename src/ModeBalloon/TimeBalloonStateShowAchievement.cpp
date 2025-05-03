#include "ModeBalloon/TimeBalloonStateShowAchievement.h"

#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(TimeBalloonStateShowAchievement, Show)

NERVES_MAKE_NOSTRUCT(TimeBalloonStateShowAchievement, Show)
}  // namespace

TimeBalloonStateShowAchievement::TimeBalloonStateShowAchievement(const al::ActorInitInfo& info)
    : al::NerveStateBase("実績を見るステート") {
    initNerve(&Show, 0);
}

void TimeBalloonStateShowAchievement::appear() {
    al::NerveStateBase::appear();
    al::setNerve(this, &Show);
}

void TimeBalloonStateShowAchievement::exeShow() {}

void TimeBalloonStateShowAchievement::exeClose() {
    kill();
}
