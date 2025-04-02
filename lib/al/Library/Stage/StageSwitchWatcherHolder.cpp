#include "Library/Stage/StageSwitchWatcherHolder.h"

#include "Library/Stage/StageSwitchWatcher.h"

namespace al {
StageSwitchWatcherHolder::StageSwitchWatcherHolder(s32 capacity)
    : mSwitchWatcherCapacity(capacity) {
    mSwitchWatchers = new StageSwitchWatcher*[capacity];
}

void StageSwitchWatcherHolder::add(StageSwitchWatcher* watcher) {
    mSwitchWatchers[mSwitchWatcherCount] = watcher;
    mSwitchWatcherCount++;
}

void StageSwitchWatcherHolder::movement() {
    for (s32 i = 0; i < mSwitchWatcherCount; i++)
        mSwitchWatchers[i]->update();
}
}  // namespace al
