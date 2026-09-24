#include "Scene/InformationWindowObjectRequester.h"

#include "Library/Nerve/NerveKeeper.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Player/PlayerUtil.h"

#include "Scene/InformationWindowFunction.h"
#include "Scene/InformationWindowRequestHolder.h"
#include "Util/PlayerUtil.h"

namespace {
NERVE_IMPL(InformationWindowObjectRequester, Show);
NERVE_IMPL(InformationWindowObjectRequester, Hide);

NERVES_MAKE_NOSTRUCT(InformationWindowObjectRequester, Show, Hide);
}  // namespace

InformationWindowObjectRequester::InformationWindowObjectRequester(
    InformationWindowRequestHolder* requestHolder, InformationWindowStateHolder* stateHolder,
    const al::PlayerHolder* playerHolder)
    : InformationWindowRequester(requestHolder, stateHolder, playerHolder) {
    mNerveKeeper = new al::NerveKeeper(this, &Show, 0);
}

void InformationWindowObjectRequester::update() {
    getNerveKeeper()->update();
}

void InformationWindowObjectRequester::requestAppearObject(
    const al::IUseSceneObjHolder* sceneObjHolder, const char* name) {
    const al::LiveActor* player = al::getPlayerActor(mPlayerHolder, 0);

    TutorialInfo info;
    info.type = TutorialType_Object;
    info.hasMovieFile = InformationWindowFunction::isExistMovieFile(name);
    info.name.format("%s", name);
    info.sceneObjHolder = sceneObjHolder;
    info.requester = this;
    info.isAutoShow = true;
    volatile u8* updateTextFlag = &info.updateTextFlag;
    *updateTextFlag = 0;

    if (rs::isPlayerHack(player) || rs::isPlayerBinding(player))
        info.isHide = true;

    requestAppear(info);
}

void InformationWindowObjectRequester::requestCloseObject(
    const al::IUseSceneObjHolder* sceneObjHolder, const char* name) {
    TutorialInfo info;
    info.type = TutorialType_Object;
    info.hasMovieFile = InformationWindowFunction::isExistMovieFile(name);
    info.name.format("%s", name);
    info.isAutoShow = true;
    info.sceneObjHolder = sceneObjHolder;
    info.requester = this;
    volatile u8* updateTextFlag = &info.updateTextFlag;
    *updateTextFlag = 0;

    requestCancel(info);
}

void InformationWindowObjectRequester::exeShow() {
    const al::LiveActor* player = al::getPlayerActor(mPlayerHolder, 0);

    if (rs::isPlayerHack(player) || rs::isPlayerBinding(player)) {
        mRequestHolder->hide(TutorialType_Object);
        al::setNerve(this, &Hide);
    }
}

void InformationWindowObjectRequester::exeHide() {
    const al::LiveActor* player = al::getPlayerActor(mPlayerHolder, 0);

    if (rs::isPlayerHack(player) || rs::isPlayerBinding(player))
        return;

    mRequestHolder->show(TutorialType_Object);
    al::setNerve(this, &Show);
}
