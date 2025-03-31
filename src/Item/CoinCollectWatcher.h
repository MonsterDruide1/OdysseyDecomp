#pragma once

#include <basis/seadTypes.h>

#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

namespace al {
struct ActorInitInfo;
class IUseSceneObjHolder;
class LiveActor;
}  // namespace al

class CoinCollectLayout;

class CoinCollectWatcher : public al::ISceneObj {
public:
    static constexpr s32 sSceneObjId = SceneObjID_CoinCollectWatcher;

    CoinCollectWatcher();

    const char* getSceneObjName() const override;
    void initAfterPlacementSceneObj(const al::ActorInitInfo& initInfo) override;

    void registerCoin(bool isCountUpCoin);
    void countup(const al::LiveActor* actor);

private:
    u32 mCoinUpCount = 0;
    u32 mRegisteredCoins = 0;
    CoinCollectLayout* mCoinCollectLayout = nullptr;
};

namespace rs {
CoinCollectWatcher* createCoinCollectWatcher(const al::IUseSceneObjHolder* objHolder);
}  // namespace rs
