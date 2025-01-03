#include "HintPhotoLayoutHolder.h"

#include "heap/seadHeapMgr.h"

bool HintPhotoLayoutHolder::isInit() const {
    return *(long*)(this + 8) != 0;

}

long HintPhotoLayoutHolder::getPhotoLayout() const {
    return *(long*)(this + 8);
}

long HintPhotoLayoutHolder::getDecideIcon() const {
    return *(long*)(this + 0x10);

}
HintPhotoLayoutHolder::~HintPhotoLayoutHolder() {
    long *Var;

    if (sead::HeapMgr::sInstancePtr == static_cast<sead::HeapMgr*>(nullptr)) {
        free (this);
        return;
    }

    //if (this != )

}


