#include "Layout/PlayGuideMap.h"

#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Play/Layout/SimpleLayoutAppearWaitEnd.h"

#include "Layout/MapLayout.h"

namespace {
NERVE_IMPL(PlayGuideMap, Hide);
NERVE_IMPL(PlayGuideMap, End);
NERVE_IMPL(PlayGuideMap, Show);

NERVES_MAKE_NOSTRUCT(PlayGuideMap, Show);
NERVES_MAKE_STRUCT(PlayGuideMap, Hide, End);
}  // namespace

PlayGuideMap::PlayGuideMap(const char* name, const al::LayoutInitInfo& info)
    : al::NerveExecutor(name) {
    mLayout = new al::SimpleLayoutAppearWaitEnd("[シーン情報]地図操作説明", "PlayGuideMap", info,
                                                nullptr, false);
    al::killLayoutIfActive(mLayout);
    initNerve(&NrvPlayGuideMap.Hide);
}

void PlayGuideMap::start() {
    if (al::isNerve(this, &NrvPlayGuideMap.End))
        al::setNerve(this, &NrvPlayGuideMap.Hide);
}

void PlayGuideMap::end() {
    if (!al::isNerve(this, &NrvPlayGuideMap.End))
        al::setNerve(this, &NrvPlayGuideMap.End);
}

void PlayGuideMap::endImmediate() {
    al::killLayoutIfActive(mLayout);
    al::setNerve(this, &NrvPlayGuideMap.End);
}

void PlayGuideMap::exeHide() {
    if (rs::isEnableCheckpointWarp(mLayout))
        al::setNerve(this, &Show);
}

void PlayGuideMap::exeShow() {
    if (al::isFirstStep(this))
        mLayout->appear();
}

void PlayGuideMap::exeEnd() {
    if (al::isFirstStep(this))
        mLayout->end();
}
