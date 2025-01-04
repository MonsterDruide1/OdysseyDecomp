#include "Scene/HintPhotoLayoutHolder.h"

#include "Library/Layout/LayoutActor.h"

#include "Layout/DecideIconLayout.h"

HintPhotoLayoutHolder::HintPhotoLayoutHolder() {
    mLayoutActor = nullptr;
    mDecideIconLayout = nullptr;
}

bool HintPhotoLayoutHolder::isInit() const {
    return mLayoutActor != nullptr;
}

const al::LayoutActor* HintPhotoLayoutHolder::getPhotoLayout() const {
    return mLayoutActor;
}

DecideIconLayout* HintPhotoLayoutHolder::getDecideIcon() const {
    return mDecideIconLayout;
}

void HintPhotoLayoutHolder::init(const al::LayoutInitInfo& info) {
    if (mLayoutActor == nullptr) {
        mLayoutActor = new al::LayoutActor("ヒント写真");
        al::initLayoutActor(mLayoutActor, info, "HintPhoto",nullptr);
        mDecideIconLayout = new DecideIconLayout("決定アイコン", &info);
    }
}
