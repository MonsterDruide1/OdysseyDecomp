#pragma once

namespace al {
class ExecutorListLayoutDrawBase;
class ExecutorListLayoutUpdate;

class LayoutExecuteInfo {
public:
    LayoutExecuteInfo();
    void addUpdater(ExecutorListLayoutUpdate* updater);
    void addDrawer(ExecutorListLayoutDrawBase* drawer);

private:
    s32 mUpdaterCount = 0;
    ExecutorListLayoutUpdate* mUpdaters[2] = {};
    s32 mDrawerCount = 0;
    ExecutorListLayoutDrawBase* mDrawers[4] = {};
};

static_assert(sizeof(LayoutExecuteInfo) == 0x40);

}  // namespace al
