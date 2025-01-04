#include "HintPhotoLayoutHolder.h"
#include "Library/Layout/LayoutActor.h"
#include "Layout/DecideIconLayout.h"
#include "heap/seadHeapMgr.h"

HintPhotoLayoutHolder::HintPhotoLayoutHolder() {
    this->mLayoutActor = nullptr;
    this->decideIconLayout = nullptr;
}

bool HintPhotoLayoutHolder::isInit() const {
    return this->mLayoutActor != nullptr;
}

const al::LayoutActor* HintPhotoLayoutHolder::getPhotoLayout() const {
    return this->mLayoutActor;
}

DecideIconLayout* HintPhotoLayoutHolder::getDecideIcon() const {
    return decideIconLayout;
}

void HintPhotoLayoutHolder::init(const al::LayoutInitInfo& info) {
    if (this->mLayoutActor == nullptr) {
        auto* layoutActor = new al::LayoutActor("ヒント写真");
        this->mLayoutActor = layoutActor;
        al::initLayoutActor(layoutActor, info, "HintPhoto",nullptr);
        auto* decideIconLayout = new DecideIconLayout("決定アイコン", &info);
        this->decideIconLayout = decideIconLayout;
    }
}






/*
mDecideIconLayout = (DecideIconLayout*)operator new(0x150);
DecideIconLayout::DecideIconLayout("決定アイコン", &info);
*/

/*
layoutActor = (al::LayoutActor*)operator new(0x130);
al::LayoutActor::LayoutActor("ヒント写真");
*/