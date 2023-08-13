#pragma once

namespace al {
class ExecutorListLayoutUpdate;
class ExecutorListLayoutDrawBase;

class LayoutExecuteInfo {
public:
    LayoutExecuteInfo();
    void addUpdater(ExecutorListLayoutUpdate*);
    void addDrawer(ExecutorListLayoutDrawBase*);
    
private:
    int mUpdaterCount = 0;
    ExecutorListLayoutUpdate* mUpdaters[2] = {};
    int mDrawerCount = 0;
    ExecutorListLayoutDrawBase* mDrawers[4] = {};
};

}  // namespace al
