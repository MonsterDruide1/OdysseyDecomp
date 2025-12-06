#include "Library/Execute/ExecutorListLayoutDraw.h"

#include "Library/Execute/LayoutExecuteInfo.h"
#include "Library/Layout/LayoutActor.h"
#include "Library/Layout/LayoutKeeper.h"
#include "Project/Execute/ExecuteSystemInitInfo.h"

namespace al {
ExecutorListLayoutDrawBase::ExecutorListLayoutDrawBase(const char* name, s32 size,
                                                       const ExecuteSystemInitInfo& initInfo)
    : ExecutorListBase(name), mCapacity(size) {
    mList = new LayoutActor*[size];
    for (s32 i = 0; i < mCapacity; i++)
        mList[i] = nullptr;
    mContext = initInfo.drawCtx;
}

bool ExecutorListLayoutDrawBase::isActive() const {
    return mSize > 0;
}

void ExecutorListLayoutDrawBase::executeList() const {
    if (!ExecutorListLayoutDrawBase::isActive())
        return;

    bool isAlive = false;
    for (s32 i = 0; i < mSize; i++)
        if (mList[i]->isAlive())
            isAlive = true;

    if (!ExecutorListLayoutDrawBase::isActive() || !isAlive)
        return;

    startDraw();
    for (s32 i = 0; i < mSize; i++)
        if (mList[i]->isAlive())
            mList[i]->getLayoutKeeper()->draw();
}

void ExecutorListLayoutDrawBase::registerLayout(LayoutActor* layout) {
    mList[mSize] = layout;
    mSize++;
    layout->getExecuteInfo()->addDrawer(this);
}

}  // namespace al
