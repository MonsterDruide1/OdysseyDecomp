#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>

#include "Library/Scene/ISceneObj.h"

namespace al {
class IUseSceneObjHolder;
class LiveActor;
}  // namespace al

class ShineChipWatcher;

class ShineChipWatcherHolder : public al::ISceneObj {
public:
    ShineChipWatcherHolder();

    const char* getSceneObjName() const override;
    ~ShineChipWatcherHolder() override;

    void entry(ShineChipWatcher* watcher);
    void notify(ShineChipWatcher* watcher);
    bool tryStartAppearShine();
    ShineChipWatcher* getCurrentWatcher() const;
    bool tryUpdateCurrentWatcher(const al::LiveActor* actor);

private:
    sead::PtrArray<ShineChipWatcher> mWatchers;
    s32 mCurrentWatcherIndex = 0;
};

static_assert(sizeof(ShineChipWatcherHolder) == 0x20);

namespace ShineChipLocalFunction {
bool tryCreateShineChipWatcherHolder(const al::IUseSceneObjHolder* user);
void entryShineChipWatcher(ShineChipWatcher* watcher);
void notifyShineChipGet(ShineChipWatcher* watcher);
void notifyShineChipGet(const al::LiveActor* actor);
}  // namespace ShineChipLocalFunction

namespace rs {
bool isExistShineChipWatcher(const al::IUseSceneObjHolder* user);
bool isCompleteShineChip(const al::IUseSceneObjHolder* user);
s32 getShineChipCount(const al::IUseSceneObjHolder* user);
s32 getCurrentShineChipWatcherIndex(const al::IUseSceneObjHolder* user);
bool isCurrentShineChipWatcherTypeEmpty(const al::IUseSceneObjHolder* user);
bool isAppearedShineChipShine(const al::IUseSceneObjHolder* user);
void addDemoActorShineChipWatcher(const al::IUseSceneObjHolder* user);
bool isEnableStartShineChipCompleteDemo(const al::IUseSceneObjHolder* user);
bool tryStartAppearShineChipShine(const al::IUseSceneObjHolder* user);
bool isInAreaCurrentShineChipWatcher(const al::IUseSceneObjHolder* user);
}  // namespace rs
