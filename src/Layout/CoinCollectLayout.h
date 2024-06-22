#pragma once

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
class LiveActor;
}  // namespace al

class CoinCollectLayout : public al::LayoutActor {
public:
    CoinCollectLayout(const al::LayoutInitInfo&);

    void appearCounter(s32, s32, const al::LiveActor*);
    void exeStart();
    void updatePos();
    void exeWait();
    void exeEnd();

private:
    const al::LiveActor* mCoinActor = nullptr;
};
