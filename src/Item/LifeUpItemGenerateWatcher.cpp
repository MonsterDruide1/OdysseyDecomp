#include "Item/LifeUpItemGenerateWatcher.h"

#include "Library/Item/ItemUtil.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Scene/SceneObjUtil.h"

#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"

LifeUpItemGenerateWatcher::LifeUpItemGenerateWatcher(const char* name) : al::LiveActor(name) {}

void LifeUpItemGenerateWatcher::tryCreate(const al::LiveActor* actor,
                                          const al::ActorInitInfo& info) {
    if (al::isExistSceneObj<LifeUpItemGenerateWatcher>(actor))
        return;

    LifeUpItemGenerateWatcher* watcher =
        new LifeUpItemGenerateWatcher("ライフアップアイテム放出監視");
    al::initCreateActorNoPlacementInfo(watcher, info);
    al::setSceneObj<LifeUpItemGenerateWatcher>(actor, watcher);
}

void LifeUpItemGenerateWatcher::init(const al::ActorInitInfo& info) {
    al::initActorSceneInfo(this, info);
    al::initActorPoseTRSV(this);
    al::initActorSRT(this, info);
    al::initActorClipping(this, info);
    al::invalidateClipping(this);
    al::initExecutorWatchObj(this, info);
    makeActorAlive();
}

void LifeUpItemGenerateWatcher::control() {
    if (mGeneratedItem && al::isDead(mGeneratedItem))
        mGeneratedItem = nullptr;
}

bool LifeUpItemGenerateWatcher::tryGenerateLifeUpItem(const al::LiveActor* actor) {
    if (mGeneratedItem)
        return false;

    GameDataHolderAccessor accessor(actor);
    if (GameDataFunction::isPlayerHitPointMax(accessor))
        return false;

    s32 interval = mGenerateInterval - 1;
    if (interval >= 0) {
        mGenerateInterval = interval;
        return false;
    }

    mGenerateInterval = 2;
    mGeneratedItem = al::appearItem(actor);
    return true;
}

namespace rs {

void tryCreateLifeUpItemGenerateWatcher(const al::LiveActor* actor, const al::ActorInitInfo& info) {
    LifeUpItemGenerateWatcher::tryCreate(actor, info);
}

bool tryGenerateLifeUpItem(const al::LiveActor* actor) {
    return al::getSceneObj<LifeUpItemGenerateWatcher>(actor)->tryGenerateLifeUpItem(actor);
}

}  // namespace rs

const char* LifeUpItemGenerateWatcher::getSceneObjName() const {
    return "ライフアップアイテム放出監視";
}
