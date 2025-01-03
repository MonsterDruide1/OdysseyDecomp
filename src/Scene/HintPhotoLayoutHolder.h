#pragma once
#include "Library/Layout/LayoutInitInfo.h"

class HintPhotoLayoutHolder {
    public:
    bool isInit() const;
    //void init(al::LayoutInitInfo *info);
    long getPhotoLayout() const;
    long getDecideIcon() const;
    ~HintPhotoLayoutHolder();
};