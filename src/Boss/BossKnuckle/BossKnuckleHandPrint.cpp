#include "Boss/BossKnuckle/BossKnuckleHandPrint.h"

#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(BossKnuckleHandPrint, Appear);
NERVE_IMPL(BossKnuckleHandPrint, Disappear);
NERVES_MAKE_NOSTRUCT(BossKnuckleHandPrint, Appear, Disappear);
}  // namespace

BossKnuckleHandPrint::BossKnuckleHandPrint(const char* name) : al::LiveActor(name) {}

void BossKnuckleHandPrint::initWithArchiveName(const al::ActorInitInfo& initInfo,
                                               const char* archiveName) {
    al::initActorWithArchiveName(this, initInfo, archiveName, nullptr);
    al::initNerve(this, &Appear, 0);
    makeActorDead();
}

void BossKnuckleHandPrint::appear() {
    al::LiveActor::appear();
    al::setNerve(this, &Appear);
    al::invalidateClipping(this);
}

void BossKnuckleHandPrint::startDisappearIfAppeard() {
    if (al::isNerve(this, &Appear))
        al::setNerve(this, &Disappear);
}

void BossKnuckleHandPrint::exeAppear() {
    if (al::isFirstStep(this)) {
        al::startMclAnim(this, "Appear");
        if (al::isMtsAnimExist(this, "LeftRightChange")) {
            u8 frame = mIsLeft ? 0 : 1;
            al::startMtsAnimAndSetFrameAndStop(this, "LeftRightChange", frame);
        }
    }
}

void BossKnuckleHandPrint::exeDisappear() {
    if (al::isFirstStep(this))
        al::startMclAnim(this, "Disappear");
    if (al::isMclAnimEnd(this)) {
        al::validateClipping(this);
        kill();
    }
}
