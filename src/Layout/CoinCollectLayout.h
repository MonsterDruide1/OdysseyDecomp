#pragma once

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
class LiveActor;
}  // namespace al

class CoinCollectLayout : public al::LayoutActor {
public:
    CoinCollectLayout(const al::LayoutInitInfo& info);

    void appearCounter(s32 maxCoins, s32 currentCoins, const al::LiveActor* coinActor);
    void exeStart();
    void updatePos();
    void exeWait();
    void exeEnd();

private:
    const al::LiveActor* mCoinActor = nullptr;
};
