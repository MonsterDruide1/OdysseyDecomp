#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>

#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

namespace al {
class IUseSceneObjHolder;
class LiveActor;
}  // namespace al

class ShineChipWatcher;

class ShineChipWatcherHolder : public al::ISceneObj {
public:
    static constexpr s32 sSceneObjId = SceneObjID_ShineChipWatcherHolder;

    ShineChipWatcherHolder();

    const char* getSceneObjName() const override { return "シャインチップ全取得監視者 保持"; }

    void entry(ShineChipWatcher* watcher);
    void notify(ShineChipWatcher* watcher);
    bool tryStartAppearShine();
    ShineChipWatcher* getCurrentWatcher() const;
    bool tryUpdateCurrentWatcher(const al::LiveActor* shineChip);

    s32 getCurrentWatcherIndex() const { return mCurrentWatcherIndex; }

private:
    sead::PtrArray<ShineChipWatcher> mWatchers;
    s32 mCurrentWatcherIndex = 0;
};

static_assert(sizeof(ShineChipWatcherHolder) == 0x20);

namespace ShineChipLocalFunction {

bool tryCreateShineChipWatcherHolder(const al::IUseSceneObjHolder* objHolder);
void entryShineChipWatcher(ShineChipWatcher* watcher);
void notifyShineChipGet(ShineChipWatcher* watcher);
void notifyShineChipGet(const al::LiveActor* actor);

}  // namespace ShineChipLocalFunction

namespace rs {

bool isExistShineChipWatcher(const al::IUseSceneObjHolder* objHolder);
bool isCompleteShineChip(const al::IUseSceneObjHolder* objHolder);
s32 getShineChipCount(const al::IUseSceneObjHolder* objHolder);
s32 getCurrentShineChipWatcherIndex(const al::IUseSceneObjHolder* objHolder);
bool isCurrentShineChipWatcherTypeEmpty(const al::IUseSceneObjHolder* objHolder);
bool isAppearedShineChipShine(const al::IUseSceneObjHolder* objHolder);
void addDemoActorShineChipWatcher(const al::IUseSceneObjHolder* objHolder);
bool isEnableStartShineChipCompleteDemo(const al::IUseSceneObjHolder* objHolder);
bool tryStartAppearShineChipShine(const al::IUseSceneObjHolder* objHolder);
bool isInAreaCurrentShineChipWatcher(const al::IUseSceneObjHolder* objHolder);

}  // namespace rs
