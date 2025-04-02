#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
}
class GameDataHolderAccessor;
class GameDataHolder;

class GameSequenceInfo {
public:
    enum ShopStatus : s32 {
        ShopStatus_None = 0,
        ShopStatus_BuyShine = 1,
        ShopStatus_BuyShineMany = 2,
        ShopStatus_BuyMaxLifeUpItem = 3
    };

    enum SceneStatus : s32 {
        SceneStatus_None = 0,
        SceneStatus_BossBattle = 1,
    };

    GameSequenceInfo();

    void init();
    void resetSceneStatus();
    void setSceneStatusBossBattle();
    void setSceneStatusBossBattleEnd();
    void setSceneStatusBossBattleEndForPlayerAnim();
    void setSceneStatusInvalidSave();
    bool isSceneStatusBossBattle() const;
    bool isSceneStatusBossBattleForPlayerAnim() const;
    bool isSceneStatusInvalidSave() const;
    bool isShopStatus(s32 shopStatus) const;
    void setShopStatus(s32 shopStatus);

    bool isWipeOpenEnd() const { return mIsWipeOpenEnd; }

    void setWipeOpenEnd(bool isEnd) { mIsWipeOpenEnd = isEnd; }

private:
    SceneStatus mSceneStatus;
    bool mIsStatusBossBattle;
    bool mIsStatusInvalidSave;
    bool mIsWipeOpenEnd;
    ShopStatus mShopStatus;
};

namespace rs {
void setSceneStatusBossBattle(const al::LiveActor* actor);
void setSceneStatusBossBattleEnd(const al::LiveActor* actor);
void setSceneStatusBossBattleEndForPlayerAnim(const al::LiveActor* actor);
bool isSceneStatusBossBattle(GameDataHolderAccessor accessor);
bool isSceneStatusBossBattleForPlayerAnim(const al::LiveActor* actor);
bool isSceneStatusInvalidSave(GameDataHolderAccessor accessor);
void setSceneStatusInvalidSave(GameDataHolderAccessor accessor);
bool isInvalidSaveStage(const char* stageName);
bool isWipeOpenEnd(const al::LiveActor* actor);
void endWipeOpen(const GameDataHolder* holder);
void startWipeOpen(const GameDataHolder* holder);
bool isShopStatusBuyItem(const al::LiveActor* actor);
bool isShopStatusBuyShine(const al::LiveActor* actor);
bool isShopStatusBuyShineMany(const al::LiveActor* actor);
bool isShopStatusBuyMaxLifeUpItem(const al::LiveActor* actor);
void setShopStatusBuyShine(const al::LiveActor* actor);
void setShopStatusBuyShineMany(const al::LiveActor* actor);
void setShopStatusBuyMaxLifeUpItem(const al::LiveActor* actor);
void setShopStatusNone(const al::LiveActor* actor);
}  // namespace rs
