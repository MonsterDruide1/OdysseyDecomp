#include "Library/Execute/ExecutorListLayoutUpdate.h"

#include "Library/Execute/LayoutExecuteInfo.h"
#include "Library/Layout/LayoutActor.h"

namespace al {
ExecutorListLayoutUpdate::ExecutorListLayoutUpdate(const char* name, s32 size)
    : ExecutorListBase(name), mCapacity(size) {
    mList = new LayoutActor*[size];
    for (s32 i = 0; i < mCapacity; i++)
        mList[i] = nullptr;
}

void ExecutorListLayoutUpdate::registerLayout(LayoutActor* layout) {
    mList[mSize] = layout;
    mSize++;
    layout->getExecuteInfo()->addUpdater(this);
}

void ExecutorListLayoutUpdate::executeList() const {
    for (s32 i = 0; i < mSize; i++) {
        LayoutActor* layout = mList[i];
        if (layout->isAlive()) {
            layout->movement();
            layout->calcAnim(true);
        }
    }
}

}  // namespace al
