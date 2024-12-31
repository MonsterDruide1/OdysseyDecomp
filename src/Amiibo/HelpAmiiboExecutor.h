#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"

namespace al {
class NfpInfo;
class LiveActor;
class ActorInitInfo;
} // namespace al

class HelpAmiiboDirector;

class HelpAmiiboExecutor : public al::IUseHioNode {
public:
    HelpAmiiboExecutor(HelpAmiiboDirector*, al::LiveActor*, const char*);

    virtual void initAfterPlacement(const al::ActorInitInfo&);
    virtual bool isTriggerTouch(const al::NfpInfo&) const = 0;
    virtual bool isEnableUse() = 0;
    virtual bool execute() = 0;
    virtual void activate();
    virtual void deactivate();
    virtual s32 getType() const = 0;

    bool tryTouch(const al::NfpInfo&);
    void tryExecute();

protected:
    al::LiveActor* getActor() const {
        return mHelpAmiiboActor;
    }

private:
    HelpAmiiboDirector* mHelpAmiiboDirector = nullptr;
    bool mIsActivated = false;
    bool mIsTouched = false;
    al::LiveActor* mHelpAmiiboActor = nullptr;
};
