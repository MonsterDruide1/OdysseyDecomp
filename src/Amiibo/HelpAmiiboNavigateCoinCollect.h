#pragma once

#include "Amiibo/HelpAmiiboExecutor.h"

namespace al {
class LiveActor;
}  // namespace al

class CoinCollect;
class HelpAmiiboDirector;
class TimeBalloonHintArrow;

class HelpAmiiboNavigateCoinCollect : public HelpAmiiboExecutor {
public:
    HelpAmiiboNavigateCoinCollect(HelpAmiiboDirector* director, al::LiveActor* actor);

    void initAfterPlacement(const al::ActorInitInfo& initInfo) override;
    bool isTriggerTouch(const al::NfpInfo& nfpInfo) const override;
    void activate() override;
    bool execute() override;
    HelpAmiiboType getType() const override;

private:
    CoinCollect* mHintedCoinCollect = nullptr;
    TimeBalloonHintArrow* mTimeBalloonHintArrow = nullptr;
};
