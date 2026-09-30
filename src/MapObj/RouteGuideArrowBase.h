#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

class RouteGuideArrowBase {
public:
    virtual const sead::Vector3f& getGuideTrans() const = 0;

    virtual void setGuideAlpha(f32 alpha) {}

    virtual void validateGuide() { mIsValidateGuide = true; }

    virtual void invalidateGuide() { mIsValidateGuide = false; }

    virtual bool isValidateGuide() const { return mIsValidateGuide; }

private:
    bool mIsValidateGuide = true;
};

static_assert(sizeof(RouteGuideArrowBase) == 0x10);
