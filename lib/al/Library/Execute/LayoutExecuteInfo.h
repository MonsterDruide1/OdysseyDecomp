#pragma once

#include <basis/seadTypes.h>

namespace al {
class ExecutorListLayoutUpdate;
class ExecutorListLayoutDrawBase;

class LayoutExecuteInfo {
public:
    LayoutExecuteInfo();
    void addUpdater(ExecutorListLayoutUpdate*);
    void addDrawer(ExecutorListLayoutDrawBase*);

private:
    s32 mUpdaterCount = 0;
    ExecutorListLayoutUpdate* mUpdaters[2] = {};
    s32 mDrawerCount = 0;
    ExecutorListLayoutDrawBase* mDrawers[4] = {};
};

}  // namespace al
