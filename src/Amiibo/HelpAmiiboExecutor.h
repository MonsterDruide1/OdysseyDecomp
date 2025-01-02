#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"

namespace al {
class NfpInfo;
class LiveActor;
class ActorInitInfo;
} // namespace al

class HelpAmiiboDirector;

enum class HelpAmiiboType : s32 {
    PlayerInvincible = 0,
    FallCoin = 1,
    LifeMaxUpItem = 1,
    CoinCollect = 2,
    InvincibleAttack = 2,
    NavigateCoinCollect = 2,
    Yoshi = 3,
    CountUpCoin = 4,
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

    al::LiveActor* getActor() const {
        return mHelpAmiiboActor;
    }

private:
    HelpAmiiboDirector* mHelpAmiiboDirector = nullptr;
    bool mIsActivated = false;
    bool mIsTouched = false;
    al::LiveActor* mHelpAmiiboActor = nullptr;
};
