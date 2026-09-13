#include "Item/ShineChipWatcherHolder.h"

#include "Library/Scene/SceneObjUtil.h"

#include "Item/Shine.h"
#include "Item/ShineChipWatcher.h"

ShineChipWatcherHolder::ShineChipWatcherHolder() {
    mWatchers.allocBuffer(8, nullptr);
}

void ShineChipWatcherHolder::entry(ShineChipWatcher* watcher) {
    mWatchers.pushBack(watcher);
}

void ShineChipWatcherHolder::notify(ShineChipWatcher* watcher) {
    mCurrentWatcherIndex = mWatchers.indexOf(watcher);
}

bool ShineChipWatcherHolder::tryStartAppearShine() {
    return getCurrentWatcher()->tryStartAppearShine();
}

ShineChipWatcher* ShineChipWatcherHolder::getCurrentWatcher() const {
    return mWatchers[mCurrentWatcherIndex];
}

bool ShineChipWatcherHolder::tryUpdateCurrentWatcher(const al::LiveActor* shineChip) {
    s32 watcherCountMax = mWatchers.size();

    for (s32 i = 0; i < watcherCountMax; i++) {
        ShineChipWatcher* watcher = mWatchers[i];
        if (watcher->isWatch(shineChip)) {
            watcher->updateCount();
            mCurrentWatcherIndex = i;
            return true;
        }
    }
    return false;
}

namespace ShineChipLocalFunction {

bool tryCreateShineChipWatcherHolder(const al::IUseSceneObjHolder* objHolder) {
    if (al::isExistSceneObj<ShineChipWatcherHolder>(objHolder))
        return false;

    al::setSceneObj(objHolder, new ShineChipWatcherHolder());
    return true;
}

void entryShineChipWatcher(ShineChipWatcher* watcher) {
    al::tryGetSceneObj<ShineChipWatcherHolder>(watcher)->entry(watcher);
}

void notifyShineChipGet(ShineChipWatcher* watcher) {
    al::tryGetSceneObj<ShineChipWatcherHolder>(watcher)->notify(watcher);
}

void notifyShineChipGet(const al::LiveActor* shineChip) {
    al::tryGetSceneObj<ShineChipWatcherHolder>(shineChip)->tryUpdateCurrentWatcher(shineChip);
}

}  // namespace ShineChipLocalFunction

namespace rs {

bool isExistShineChipWatcher(const al::IUseSceneObjHolder* objHolder) {
    return al::tryGetSceneObj<ShineChipWatcherHolder>(objHolder) != nullptr;
}

bool isCompleteShineChip(const al::IUseSceneObjHolder* objHolder) {
    return ShineChipWatcher::getCompleteNum() <= getShineChipCount(objHolder);
}

s32 getShineChipCount(const al::IUseSceneObjHolder* objHolder) {
    return al::tryGetSceneObj<ShineChipWatcherHolder>(objHolder)->getCurrentWatcher()->getCount();
}

s32 getCurrentShineChipWatcherIndex(const al::IUseSceneObjHolder* objHolder) {
    return al::tryGetSceneObj<ShineChipWatcherHolder>(objHolder)->getCurrentWatcherIndex();
}

bool isCurrentShineChipWatcherTypeEmpty(const al::IUseSceneObjHolder* objHolder) {
    return al::tryGetSceneObj<ShineChipWatcherHolder>(objHolder)
        ->getCurrentWatcher()
        ->getShine()
        ->isGot();
}

bool isAppearedShineChipShine(const al::IUseSceneObjHolder* objHolder) {
    return al::tryGetSceneObj<ShineChipWatcherHolder>(objHolder)
        ->getCurrentWatcher()
        ->isAppearedShine();
}

void addDemoActorShineChipWatcher(const al::IUseSceneObjHolder* objHolder) {
    al::tryGetSceneObj<ShineChipWatcherHolder>(objHolder)
        ->getCurrentWatcher()
        ->addDemoActorWithChips();
}

bool isEnableStartShineChipCompleteDemo(const al::IUseSceneObjHolder* objHolder) {
    if (!isExistShineChipWatcher(objHolder))
        return false;

    return isCompleteShineChip(objHolder) && !isAppearedShineChipShine(objHolder);
}

bool tryStartAppearShineChipShine(const al::IUseSceneObjHolder* objHolder) {
    return al::tryGetSceneObj<ShineChipWatcherHolder>(objHolder)
        ->getCurrentWatcher()
        ->tryStartAppearShine();
}

bool isInAreaCurrentShineChipWatcher(const al::IUseSceneObjHolder* objHolder) {
    return al::tryGetSceneObj<ShineChipWatcherHolder>(objHolder)->getCurrentWatcher()->isInArea();
}

}  // namespace rs
