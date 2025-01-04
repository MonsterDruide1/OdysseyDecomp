#pragma once
#include "Library/Layout/LayoutActor.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Scene/ISceneObj.h"

class DecideIconLayout;

class HintPhotoLayoutHolder : public al::ISceneObj {
    public:
    HintPhotoLayoutHolder();
    void init(const al::LayoutInitInfo& info);
    bool isInit() const;
    const al::LayoutActor* getPhotoLayout() const;
    DecideIconLayout* getDecideIcon() const;
    private:
    al::LayoutActor* mLayoutActor;
    DecideIconLayout* decideIconLayout;
};