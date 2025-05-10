#pragma once

#include "Amiibo/HelpAmiiboExecutor.h"

namespace al {
struct NfpInfo;

class LiveActor;
}  // namespace al

class HelpAmiiboDirector;

class HelpAmiiboPlayerInvincible : public HelpAmiiboExecutor {
public:
    HelpAmiiboPlayerInvincible(HelpAmiiboDirector* director, al::LiveActor* actor);

    bool isTriggerTouch(const al::NfpInfo& nfpInfo) const override;
    bool isEnableUse() override;
    bool execute() override;

    HelpAmiiboType getType() const override { return HelpAmiiboType::Mario; }
};
