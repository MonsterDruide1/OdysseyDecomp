#include "Library/Execute/LayoutExecuteInfo.h"

namespace al {
LayoutExecuteInfo::LayoutExecuteInfo() {}

void LayoutExecuteInfo::addUpdater(ExecutorListLayoutUpdate* updater) {
    mUpdaters[mUpdaterCount] = updater;
    mUpdaterCount++;
}

void LayoutExecuteInfo::addDrawer(ExecutorListLayoutDrawBase* drawer) {
    mDrawers[mDrawerCount] = drawer;
    mDrawerCount++;
}
}  // namespace al
