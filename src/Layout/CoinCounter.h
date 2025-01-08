#pragma once

#include <basis/seadTypes.h>

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
class LiveActor;
}  // namespace al

class CoinCounter : public al::LayoutActor {
public:
    CoinCounter(const char* name, const al::LayoutInitInfo& initInfo, bool isCoin);

    void kill() override;

    void updatePanel(u32 animationCount, s32 panelType);
    bool isWait() const;
    void tryStart();
    void updateCountImmidiate();
    void tryEnd();
    void startCountAnim(s32 coinNum);
    bool tryUpdateCount();
    s32 getCountFromData() const;
    s32 getCountTotalFromData() const;

    void exeAppear();
    void exeWait();
    void exeEnd();
    void exeAdd();
    void exeSub();
    void exeCountAnimAdd();
    void exeCountAnimSub();

private:
    s32 mPrevCoinCount = 0;
    s32 mCoinNum = 0;
    s32 mTotalCoins = 0;
    s32 mPanelType = -1;
    bool mIsCoin = false;
    const char* mPanelName = nullptr;
    s32 mAnimationCount = 0;
    bool mIsUpdateCount = true;
};
