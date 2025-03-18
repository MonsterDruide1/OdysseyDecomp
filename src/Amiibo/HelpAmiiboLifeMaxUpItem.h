#pragma once

#include "Amiibo/HelpAmiiboExecutor.h"

class HelpAmiiboDirector;

namespace al {
struct ActorInitInfo;
class LiveActor;
}  // namespace al

class LifeMaxUpItem2D;
class LifeMaxUpItem;

class HelpAmiiboLifeMaxUpItem : public HelpAmiiboExecutor {
public:
    HelpAmiiboLifeMaxUpItem(HelpAmiiboDirector* director, al::LiveActor* amiiboActor);

    void initAfterPlacement(const al::ActorInitInfo& initInfo) override;
    bool isTriggerTouch(const al::NfpInfo& nfpInfo) const override;
    bool isEnableUse() override;
    bool execute() override;
    void activate() override;
    HelpAmiiboType getType() const override;

private:
    LifeMaxUpItem* mItem = nullptr;
    LifeMaxUpItem2D* mItem2D = nullptr;
};
