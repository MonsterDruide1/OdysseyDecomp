#pragma once

#include "Library/Scene/ISceneObj.h"

namespace al {
class LayoutActor;
class LayoutInitInfo;
}  // namespace al
class DecideIconLayout;

class HintPhotoLayoutHolder : public al::ISceneObj {
public:
    HintPhotoLayoutHolder();
    void init(const al::LayoutInitInfo& initInfo);
    bool isInit() const;
    const al::LayoutActor* getPhotoLayout() const;
    DecideIconLayout* getDecideIcon() const;
    const char* getSceneObjName() const;

private:
    al::LayoutActor* mLayoutActor = nullptr;
    DecideIconLayout* mDecideIconLayout = nullptr;
};
