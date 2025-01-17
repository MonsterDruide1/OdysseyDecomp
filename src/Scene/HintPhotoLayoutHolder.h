#pragma once

#include "Library/Scene/ISceneObj.h"

#include "Layout/DecideIconLayout.h"

class DecideIconLayout;

class HintPhotoLayoutHolder : public al::ISceneObj {
public:
    HintPhotoLayoutHolder();
    void init(const al::LayoutInitInfo& info);
    bool isInit() const;
    const al::LayoutActor* getPhotoLayout() const;
    DecideIconLayout* getDecideIcon() const;
    const char* getSceneObjName() const;

private:
    al::LayoutActor* mLayoutActor = nullptr;
    DecideIconLayout* mDecideIconLayout = nullptr;
};
