#pragma once

#include "Amiibo/HelpAmiiboExecutor.h"

namespace al {
class NfpInfo;
class LiveActor;
class ActorInitInfo;
}  // namespace al

class HelpAmiiboDirector;
class Yoshi;

class HelpAmiiboYoshi : public HelpAmiiboExecutor {
public:
    HelpAmiiboYoshi(HelpAmiiboDirector* director, al::LiveActor* amiiboActor);

    void initAfterPlacement(const al::ActorInitInfo& actorInitInfo) override;
    bool isTriggerTouch(const al::NfpInfo& nfpInfo) const override;
    bool execute() override;
    void activate() override;
    HelpAmiiboType getType() const override;

private:
    Yoshi* mYoshi = nullptr;
};
