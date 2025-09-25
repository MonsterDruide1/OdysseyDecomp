#include "Layout/PlayGuideBgm.h"

#include "Library/Bgm/BgmLineFunction.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Play/Layout/SimpleLayoutAppearWaitEnd.h"

namespace {
NERVE_IMPL(PlayGuideBgm, Hide);
NERVE_IMPL(PlayGuideBgm, End);
NERVE_IMPL(PlayGuideBgm, EndForDead);
NERVE_IMPL(PlayGuideBgm, Show);

NERVES_MAKE_NOSTRUCT(PlayGuideBgm, Show);
NERVES_MAKE_STRUCT(PlayGuideBgm, Hide, End, EndForDead);
}  // namespace

PlayGuideBgm::PlayGuideBgm(const char* name, const al::LayoutInitInfo& info)
    : al::NerveStateBase(name) {
    mLayout = new al::SimpleLayoutAppearWaitEnd("[シーン情報]BGM再生", "PlayGuideBgm", info,
                                                nullptr, false);
    al::killLayoutIfActive(mLayout);
    initNerve(&NrvPlayGuideBgm.Hide);
    appear();
}

void PlayGuideBgm::start() {
    appear();
    if (al::isNerve(this, &NrvPlayGuideBgm.End) || al::isNerve(this, &NrvPlayGuideBgm.EndForDead))
        al::setNerve(this, &NrvPlayGuideBgm.Hide);
}

void PlayGuideBgm::end() {
    if (!al::isNerve(this, &NrvPlayGuideBgm.EndForDead))
        al::setNerve(this, &NrvPlayGuideBgm.EndForDead);
}

void PlayGuideBgm::endImmediate() {
    al::killLayoutIfActive(mLayout);
    al::setNerve(this, &NrvPlayGuideBgm.EndForDead);
    kill();
}

void PlayGuideBgm::exeHide() {
    if (al::isRunningBgm(mLayout, "CollectBgm"))
        al::setNerve(this, &Show);
}

void PlayGuideBgm::exeShow() {
    if (al::isFirstStep(this))
        mLayout->appear();

    if (!al::isRunningBgm(mLayout, "CollectBgm"))
        al::setNerve(this, &NrvPlayGuideBgm.End);
}

void PlayGuideBgm::exeEnd() {
    if (al::isFirstStep(this))
        mLayout->end();

    if (al::isDead(mLayout))
        al::setNerve(this, &NrvPlayGuideBgm.Hide);
}

void PlayGuideBgm::exeEndForDead() {
    if (al::isFirstStep(this) && mLayout->isAppearOrWait())
        mLayout->end();

    if (al::isDead(mLayout))
        kill();
}
