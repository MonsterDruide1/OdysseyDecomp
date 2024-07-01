#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
}
class GameDataHolderAccessor;
class GameDataHolder;

class GameSequenceInfo {
public:
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
    bool isShopStatus(s32) const;
    void setShopStatus(s32);
};

namespace rs {
void setSceneStatusBossBattle(const al::LiveActor*);
void setSceneStatusBossBattleEnd(const al::LiveActor*);
void setSceneStatusBossBattleEndForPlayerAnim(const al::LiveActor*);
bool isSceneStatusBossBattle(GameDataHolderAccessor);
bool isSceneStatusBossBattleForPlayerAnim(const al::LiveActor*);
bool isSceneStatusInvalidSave(GameDataHolderAccessor);
void setSceneStatusInvalidSave(GameDataHolderAccessor);
bool isInvalidSaveStage(const char*);
bool isWipeOpenEnd(const al::LiveActor*);
void endWipeOpen(const GameDataHolder*);
void startWipeOpen(const GameDataHolder*);
bool isShopStatusBuyItem(const al::LiveActor*);
bool isShopStatusBuyShine(const al::LiveActor*);
bool isShopStatusBuyShineMany(const al::LiveActor*);
bool isShopStatusBuyMaxLifeUpItem(const al::LiveActor*);
void setShopStatusBuyShine(const al::LiveActor*);
void setShopStatusBuyShineMany(const al::LiveActor*);
void setShopStatusBuyMaxLifeUpItem(const al::LiveActor*);
void setShopStatusNone(const al::LiveActor*);
}  // namespace rs
