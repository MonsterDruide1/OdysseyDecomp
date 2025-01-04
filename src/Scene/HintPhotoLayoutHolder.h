#pragma once
#include "Layout/DecideIconLayout.h"
#include "Library/Layout/LayoutActor.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Scene/ISceneObj.h"

class HintPhotoLayoutHolder : public al::ISceneObj {
    public:
    HintPhotoLayoutHolder();
    void init(const al::LayoutInitInfo& info);
    bool isInit() const;
    const al::LayoutActor* getPhotoLayout() const;
    DecideIconLayout* getDecideIcon() const;
    private:
    const al::LayoutActor* layoutActor;
    DecideIconLayout* decideIconLayout;
};