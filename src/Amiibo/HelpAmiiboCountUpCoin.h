#pragma once

#include <container/seadPtrArray.h>

#include "Library/Nerve/IUseNerve.h"

#include "Amiibo/HelpAmiiboExecutor.h"

namespace al {
struct ActorInitInfo;
class IUseNerve;
class LiveActor;
class NerveKeeper;
struct NfpInfo;
}  // namespace al

class HelpAmiiboDirector;
class LifeUpItem;
class LifeUpItem2D;
class Coin;
class Coin2D;

class HelpAmiiboCountUpCoin : public HelpAmiiboExecutor, public al::IUseNerve {
public:
    HelpAmiiboCountUpCoin(HelpAmiiboDirector* director, al::LiveActor* actor);

    void initAfterPlacement(const al::ActorInitInfo& initInfo) override;
    bool isTriggerTouch(const al::NfpInfo& nfpInfo) const override;
    bool isEnableUse() override;
    bool execute() override;
    void activate() override;
    HelpAmiiboType getType() const override;
    al::NerveKeeper* getNerveKeeper() const override;

    void updateItemQT(al::LiveActor* actor);

    void exeEnd();
    void exeDelay();
    void exeAppearCoin();

private:
    al::NerveKeeper* mNerveKeeper = nullptr;
    LifeUpItem* mLifeUpItem = nullptr;
    LifeUpItem2D* mLifeUpItem2D = nullptr;
    sead::PtrArray<Coin> mCoinBuffer;
    sead::PtrArray<Coin2D> mCoin2DBuffer;
    s32 mNumberOfCoins = 4;
    u32 mCoinsSpawned = 0;
};
