#include "MapObj/CapMessageAfterInformation.h"

#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"

#include "Util/InputInterruptTutorialUtil.h"
#include "Util/PlayerUtil.h"

namespace {
NERVE_IMPL(CapMessageAfterInformation, Wait);
NERVE_END_IMPL(CapMessageAfterInformation, Show);

NERVES_MAKE_NOSTRUCT(CapMessageAfterInformation, Wait, Show);
}  // namespace

CapMessageAfterInformation::CapMessageAfterInformation(const char* name) : al::LiveActor(name) {}

void CapMessageAfterInformation::makeActorDead() {
    if (!al::isNerve(this, &Wait))
        al::setNerve(this, &Wait);

    al::LiveActor::makeActorDead();
}

void CapMessageAfterInformation::init(const al::ActorInitInfo& info) {
    al::initActorSceneInfo(this, info);
    al::initExecutorUpdate(this, info, "監視オブジェ");
    al::initStageSwitch(this, info);
    mInfoAreaGroup =
        al::createLinkAreaGroup(this, info, "InfoArea", "インフォメーション起動エリアグループ",
                                "インフォメーション起動エリア");
    al::tryGetStringArg(&mLabel, info, "Label");
    al::initNerve(this, &Wait, 0);
    al::tryGetArg(&mHackActorType, info, "HackActorType");

    if (mInfoAreaGroup && mLabel) {
        al::trySyncStageSwitchAppearAndKill(this);
        return;
    }

    makeActorDead();
}

void CapMessageAfterInformation::exeWait() {
    if (al::isInAreaObjPlayerAnyOne(this, mInfoAreaGroup) &&
        rs::isPlayerHackType(this, mHackActorType))
        al::setNerve(this, &Show);
}

bool CapMessageAfterInformation::isEnable() const {
    if (al::isInAreaObjPlayerAnyOne(this, mInfoAreaGroup))
        return rs::isPlayerHackType(this, mHackActorType);

    return false;
}

void CapMessageAfterInformation::exeShow() {
    if (al::isFirstStep(this))
        rs::appearCapMsgTutorial(this, mLabel);

    if (!al::isInAreaObjPlayerAnyOne(this, mInfoAreaGroup) ||
        !rs::isPlayerHackType(this, mHackActorType))
        al::setNerve(this, &Wait);
}

void CapMessageAfterInformation::endShow() {
    rs::closeCapMsgTutorial(this);
}
