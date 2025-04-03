#pragma once

#include <basis/seadTypes.h>

namespace al {
class StageSwitchWatcher;

class StageSwitchWatcherHolder {
public:
    StageSwitchWatcherHolder(s32 capacity);

    void add(StageSwitchWatcher* watcher);
    void movement();

private:
    StageSwitchWatcher** mSwitchWatchers = nullptr;
    s32 mSwitchWatcherCount = 0;
    s32 mSwitchWatcherCapacity;
};
}  // namespace al
