#pragma once

#include "Library/HostIO/HioNode.h"
#include "Library/LiveActor/ActorInitInfo.h"

namespace al {
class NfpInfo;
}

class HelpAmiiboDirector;

class HelpAmiiboExecutor : public al::IUseHioNode {
public:
    HelpAmiiboExecutor(HelpAmiiboDirector*, al::LiveActor*, const char*);
    virtual void initAfterPlacement(const al::ActorInitInfo&);
    bool tryTouch(const al::NfpInfo&);
    void tryExecute();
    virtual bool isTriggerTouch(const al::NfpInfo&) = 0;
    virtual bool isEnableUse() = 0;
    virtual bool execute() = 0;
    virtual void activate();
    virtual void deactivate();
    virtual s32 getType() = 0;

private:
    HelpAmiiboDirector* mHelpAmiiboDirector = nullptr;
    char mIsActivated = false;
    char mIsTouched = false;
    al::LiveActor* mHelpAmiiboActor = nullptr;
};
