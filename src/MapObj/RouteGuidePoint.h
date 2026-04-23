#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/HostIO/IUseName.h"

class RouteGuidePoint : public virtual al::IUseName {
public:
    RouteGuidePoint();

    virtual const sead::Vector3f& getGuideTrans() const = 0;

    virtual bool isHorizontalGuide() const { return mIsHorizontalGuide; }

    virtual bool isValidateGuide() const { return mIsValidateGuide; }

    void onHorizontalGuide();
    void offHorizontalGuide();
    void validateGuide();
    void invalidateGuide();

private:
    bool mIsValidateGuide = true;
    bool mIsHorizontalGuide = true;
};

static_assert(sizeof(RouteGuidePoint) == 0x10);
