#include "Scene/HintPhotoLayoutHolder.h"

#include "Library/Layout/LayoutActor.h"
#include "Library/Layout/LayoutInitInfo.h"

#include "Layout/DecideIconLayout.h"

HintPhotoLayoutHolder::HintPhotoLayoutHolder() = default;

bool HintPhotoLayoutHolder::isInit() const {
    return mLayoutActor != nullptr;
}

const al::LayoutActor* HintPhotoLayoutHolder::getPhotoLayout() const {
    return mLayoutActor;
}

DecideIconLayout* HintPhotoLayoutHolder::getDecideIcon() const {
    return mDecideIconLayout;
}

const char* HintPhotoLayoutHolder::getSceneObjName() const {
    return "ヒント写真レイアウト保持";
}

void HintPhotoLayoutHolder::init(const al::LayoutInitInfo& initInfo) {
    if (mLayoutActor == nullptr) {
        mLayoutActor = new al::LayoutActor("ヒント写真");
        al::initLayoutActor(mLayoutActor, initInfo, "HintPhoto", nullptr);
        mDecideIconLayout = new DecideIconLayout("決定アイコン", initInfo);
    }
}
