#include "Amiibo/HelpAmiiboExecutor.h"

HelpAmiiboExecutor::HelpAmiiboExecutor(HelpAmiiboDirector* director, al::LiveActor* amiiboActor,
                                       const char* amiiboName)
    : mHelpAmiiboDirector(director), mHelpAmiiboActor(amiiboActor) {}

void HelpAmiiboExecutor::initAfterPlacement(const al::ActorInitInfo&) {}

bool HelpAmiiboExecutor::tryTouch(const al::NfpInfo& nfpInfo) {
    if (mIsActivated || isTriggerTouch(nfpInfo) == false)
        return false;

    mIsTouched = true;
    activate();
    return true;
}

void HelpAmiiboExecutor::tryExecute() {
    if (!mIsActivated)
        return;

    mIsTouched = false;
    if (execute())
        return deactivate();
}

void HelpAmiiboExecutor::activate() {
    mIsActivated = true;
}

void HelpAmiiboExecutor::deactivate() {
    mIsActivated = false;
}
