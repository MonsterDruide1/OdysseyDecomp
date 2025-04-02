#pragma once

#include <container/seadPtrArray.h>

#include "Library/Nerve/IUseNerve.h"

#include "Amiibo/HelpAmiiboExecutor.h"

namespace al {
struct NfpInfo;

struct ActorInitInfo;
class LiveActor;
class NerveKeeper;
}  // namespace al

class Coin;
class HelpAmiiboDirector;

class HelpAmiiboFallCoin : public HelpAmiiboExecutor, public al::IUseNerve {
public:
    HelpAmiiboFallCoin(HelpAmiiboDirector* director, al::LiveActor* actor);

    void initAfterPlacement(const al::ActorInitInfo& initInfo) override;
    bool isTriggerTouch(const al::NfpInfo& nfpInfo) const override;
    bool isEnableUse() override;
    void activate() override;
    bool execute() override;
    HelpAmiiboType getType() const override;
    al::NerveKeeper* getNerveKeeper() const override;

    void exeWait();
    void exeFall();

private:
    al::NerveKeeper* mNerveKeeper = nullptr;
    sead::PtrArray<Coin> mCoinBuffer;
};
