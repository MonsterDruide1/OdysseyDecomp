#pragma once

#include "Amiibo/HelpAmiiboExecutor.h"

namespace al {
struct NfpInfo;

class LiveActor;
}  // namespace al

class HelpAmiiboDirector;
class ItemAmiiboKoopa;

class HelpAmiiboInvincibleAttack : public HelpAmiiboExecutor {
public:
    HelpAmiiboInvincibleAttack(HelpAmiiboDirector* director, al::LiveActor* actor);

    void initAfterPlacement(const al::ActorInitInfo& initInfo) override;
    bool isTriggerTouch(const al::NfpInfo& nfpInfo) const override;
    void activate() override;
    bool execute() override;

    HelpAmiiboType getType() const override { return HelpAmiiboType::Koopa; }

private:
    ItemAmiiboKoopa* mItemAmiiboKoopa = nullptr;
};
