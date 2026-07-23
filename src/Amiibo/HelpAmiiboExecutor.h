#pragma once

#include <basis/seadTypes.h>
#include <prim/seadEnum.h>

#include "Library/HostIO/HioNode.h"

namespace al {
struct NfpInfo;
class LiveActor;
struct ActorInitInfo;
}  // namespace al

class HelpAmiiboDirector;

SEAD_ENUM(HelpAmiiboType,
    Mario,
    Peach,
    Koopa,
    Yoshi,
    All,
)

class HelpAmiiboExecutor : public al::IUseHioNode {
public:
    HelpAmiiboExecutor(HelpAmiiboDirector* director, al::LiveActor* amiiboActor,
                       const char* amiiboName);

    virtual void initAfterPlacement(const al::ActorInitInfo& initInfo);
    virtual bool isTriggerTouch(const al::NfpInfo& nfpInfo) const = 0;
    virtual bool isEnableUse() = 0;
    virtual bool execute() = 0;
    virtual void activate();
    virtual void deactivate();
    virtual HelpAmiiboType getType() const = 0;

    bool tryTouch(const al::NfpInfo& nfpInfo);
    void tryExecute();

    HelpAmiiboDirector* getDirector() const { return mHelpAmiiboDirector; }

    bool isTouched() const { return mIsTouched; }

    al::LiveActor* getActor() const { return mHelpAmiiboActor; }

private:
    HelpAmiiboDirector* mHelpAmiiboDirector = nullptr;
    bool mIsActivated = false;
    bool mIsTouched = false;
    al::LiveActor* mHelpAmiiboActor = nullptr;
};
