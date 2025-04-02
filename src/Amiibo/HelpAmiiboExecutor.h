#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"

namespace al {
struct NfpInfo;
class LiveActor;
struct ActorInitInfo;
}  // namespace al

class HelpAmiiboDirector;

enum class HelpAmiiboType : s64 {
    Mario = 0,
    Peach = 1,
    Koopa = 2,
    Yoshi = 3,
    All = 4,
};

class HelpAmiiboExecutor : public al::IUseHioNode {
public:
    HelpAmiiboExecutor(HelpAmiiboDirector*, al::LiveActor*, const char*);

    virtual void initAfterPlacement(const al::ActorInitInfo&);
    virtual bool isTriggerTouch(const al::NfpInfo&) const = 0;
    virtual bool isEnableUse() = 0;
    virtual bool execute() = 0;
    virtual void activate();
    virtual void deactivate();
    virtual HelpAmiiboType getType() const = 0;

    bool tryTouch(const al::NfpInfo&);
    void tryExecute();

    bool isTouched() const { return mIsTouched; }

    HelpAmiiboDirector* getDirector() const { return mHelpAmiiboDirector; }

    al::LiveActor* getActor() const { return mHelpAmiiboActor; }

private:
    HelpAmiiboDirector* mHelpAmiiboDirector = nullptr;
    bool mIsActivated = false;
    bool mIsTouched = false;
    al::LiveActor* mHelpAmiiboActor = nullptr;
};
