#include "Layout/LoadLayoutCtrl.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Play/Layout/SimpleLayoutAppearWaitEnd.h"

namespace {
NERVE_IMPL(LoadLayoutCtrl, EndLoad);
NERVE_IMPL(LoadLayoutCtrl, WaitCount);
NERVE_IMPL(LoadLayoutCtrl, WaitLoad);

NERVES_MAKE_NOSTRUCT(LoadLayoutCtrl, WaitLoad);
NERVES_MAKE_STRUCT(LoadLayoutCtrl, EndLoad, WaitCount);
}  // namespace

LoadLayoutCtrl::LoadLayoutCtrl(const al::LayoutInitInfo& info) : NerveExecutor("LoadLayoutCtrl") {
    initNerve(&NrvLoadLayoutCtrl.EndLoad, 0);
    mSaveMessageLayout =
        new al::SimpleLayoutAppearWaitEnd("ロードレイアウト", "SaveMessage", info, nullptr, false);
    mSaveMessageLayout->kill();
}

void LoadLayoutCtrl::startWaitLoad() {
    if (al::isNerve(this, &NrvLoadLayoutCtrl.EndLoad))
        al::setNerve(this, &NrvLoadLayoutCtrl.WaitCount);
}

void LoadLayoutCtrl::endLoad() {
    if (!al::isNerve(this, &NrvLoadLayoutCtrl.EndLoad))
        al::setNerve(this, &NrvLoadLayoutCtrl.EndLoad);
}

void LoadLayoutCtrl::exeWaitCount() {
    if (al::isGreaterEqualStep(this, 240))
        al::setNerve(this, &WaitLoad);
}

void LoadLayoutCtrl::exeWaitLoad() {
    if (al::isFirstStep(this)) {
        mSaveMessageLayout->appear();
        al::startAction(mSaveMessageLayout, "Loop", "Loop");

        hideTxtPane();
    }
}

void LoadLayoutCtrl::hideTxtPane() {
    al::hidePane(mSaveMessageLayout, "TxtSave");
    al::hidePane(mSaveMessageLayout, "TxtSaveSh");
}

void LoadLayoutCtrl::exeEndLoad() {
    if (al::isFirstStep(this))
        al::killLayoutIfActive(mSaveMessageLayout);
    if (al::isActive(mSaveMessageLayout))
        hideTxtPane();
}
