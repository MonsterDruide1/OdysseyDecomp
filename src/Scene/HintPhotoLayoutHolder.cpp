#include "HintPhotoLayoutHolder.h"
#include "Library/Layout/LayoutActor.h"
#include "Layout/DecideIconLayout.h"
#include "heap/seadHeapMgr.h"

HintPhotoLayoutHolder::HintPhotoLayoutHolder() {
    this->layoutActor = nullptr;
    this->decideIconLayout = nullptr;
}

bool HintPhotoLayoutHolder::isInit() const {
    return this->layoutActor != (al::LayoutActor*)nullptr;
}

const al::LayoutActor* HintPhotoLayoutHolder::getPhotoLayout() const {
    return this->layoutActor;
}

DecideIconLayout* HintPhotoLayoutHolder::getDecideIcon() const {
    return this->decideIconLayout;
}

void HintPhotoLayoutHolder::init(const al::LayoutInitInfo& info) {
    if (this->layoutActor == nullptr) {
        auto* layoutActor = new al::LayoutActor("ヒント写真");
        this->layoutActor = layoutActor;
        al::initLayoutActor(layoutActor, info, "HintPhoto",nullptr);
        auto* mDecideIconLayout = new DecideIconLayout("決定アイコン", &info);
        this->decideIconLayout = mDecideIconLayout;
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